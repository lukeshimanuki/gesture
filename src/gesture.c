/* ========================================================================*//**
 * @file gesture.c
 * @author Luke Shimanuki
 * @date 2 Jan 2015
 * @brief Implementation of functions relating to the gesture container.
 *
 *//*---------------------------------------------------------------------------
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014 Luke Shimanuki
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * ========================================================================== */

#include "gesture.h"

#define CENTER_RADIUS 600
#define OUTER_CENTER_RADIUS 700
#define OVERSHOOT 15

/***************************************************************************//**
 * Takes a given point and determines which region it is in.
 ******************************************************************************/
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

/***************************************************************************//**
 * Takes two Gesture objects and determines if they are equal.
 ******************************************************************************/
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

