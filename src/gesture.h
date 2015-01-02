#ifndef GESTURE_H
#define GESTURE_H

#include <stdint.h>

#define MAX_SEQUENCE_SIZE 32

enum
{
	REGION_CENTER,
	REGION_UP,
	REGION_RIGHT,
	REGION_DOWN,
	REGION_LEFT,
	REGION_UP_RIGHT,
	REGION_DOWN_RIGHT,
	REGION_DOWN_LEFT,
	REGION_UP_LEFT
};

struct Gesture
{
	uint8_t size;
	uint8_t sequence[MAX_SEQUENCE_SIZE];

	int32_t x;
	int32_t y;
};

// 0 is center
// 1-4 is up right down left
// 5-8 is ur, dr, dl, ul
int8_t getRegion(uint16_t radius, uint16_t angle, uint8_t previous);

// 0: !=; 1: ==
int8_t compareGestures(const struct Gesture a, const struct Gesture b);

#endif
