#include "gesture.h"

#define CENTER_RADIUS 600
#define OUTER_CENTER_RADIUS 700
#define OVERSHOOT 15

// 0 <= angle <= 360
int8_t getRegion(uint16_t radius, uint16_t angle, uint8_t previous)
{
	// if it is within circle, it is part of the center
	if (radius < CENTER_RADIUS)
		return REGION_CENTER;
	// if already in center, has to past outer circle to leave
	if (previous == REGION_CENTER && radius < OUTER_CENTER_RADIUS)
		return -1;
	// sectors
	if (angle >= 45 + OVERSHOOT && angle <= 135 - OVERSHOOT)
		return REGION_UP;
	if (angle >= 135 + OVERSHOOT && angle <= 225 - OVERSHOOT)
		return REGION_LEFT;
	if (angle >= 225 + OVERSHOOT && angle <= 315 - OVERSHOOT)
		return REGION_DOWN;
	// for right, check if not in the others
	if (!(angle >= 45 - OVERSHOOT && angle <= 315 + OVERSHOOT))
		return REGION_RIGHT;
	// if not in a sector, then in middle ground
	return -1;
}

int8_t compareGestures(const struct Gesture a, const struct Gesture b)
{
	if (a.size == b.size)
	{
		uint8_t i;
		for (i = 0; i < a.size; i++)
		{
			if (a.sequence[i] != b.sequence[i])
				return 0;
		}
		return 1;
	}
	return 0;
}

