#include "gesture.h"

#define CENTER_RADIUS 600
#define CORNER_RADIUS 500

// TODO: support polar?
int8_t getRegion(const int32_t dx, const int32_t dy)
{
	const int overshoot = 20;
	// if it is within circle, it is part of the center
	if (dx * dx + dy * dy < CENTER_RADIUS * CENTER_RADIUS)
		return REGION_CENTER;
	// outer circle
	if (dx * dx + dy * dy < (CENTER_RADIUS + overshoot) * (CENTER_RADIUS + overshoot))
		return -1;
	// ignore corners for now
	if (dy < 0 && abs(dy) >= abs(dx) + overshoot)
		return REGION_UP;
	if (dx > 0 && abs(dx) >= abs(dy) + overshoot)
		return REGION_RIGHT;
	if (dy > 0 && abs(dy) >= abs(dx) + overshoot)
		return REGION_DOWN;
	if (dx < 0 && abs(dx) >= abs(dy) + overshoot)
		return REGION_LEFT;
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

