/* ========================================================================*//**
 * @file gesture.h
 * @author Luke Shimanuki
 * @date 2 Jan 2015
 * @brief Definition of the gesture container and related functions.
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
 *MaMaMaMaMaMaM ‰Z THE SOFTWARE.
 * ========================================================================== */

#ifndef __GESTURE_H__
#define __GESTURE_H__

#include <stdint.h>

#define MAX_SEQUENCE_SIZE 32 /**< The maximum number of regions in a sequence. */

/***************************************************************************//**
 * @brief identifies the region
 *
 * There are 5 possible regions for a point: the 4 directions and the center.
 ******************************************************************************/
enum Region
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

/***************************************************************************//**
 * @brief keeps track of touch movements
 *
 * This structure contains the sequence of regions a pointer moves during a
 * touch event.
 ******************************************************************************/
struct Gesture
{
	uint8_t size;                        /**< The number of regions in the sequence. */
	uint8_t sequence[MAX_SEQUENCE_SIZE]; /**< The sequence of regions. */

	int32_t x;                           /**< The initial x coordinate. */
	int32_t y;                           /**< The initial y coordinate. */
};

/***************************************************************************//**
 * @brief identifies the region of a point
 *
 * @param radius The distance between the point and the center.
 *
 * @param angle The counter-clockwise angle between the center-point vector
 * and the x-axis. This value must be between 0 and 360, inclusive.
 *
 * @param previous The previously recognized region for the pointer.
 *
 * @return The identifier for the region.
 ******************************************************************************/
int8_t getRegion(uint16_t radius, uint16_t angle, uint8_t previous);

/***************************************************************************//**
 * @brief compares gesture objects
 *
 * @param a The first object to compare.
 *
 * @param b The object to compare with b.
 *
 * @return The boolean value for equality: a 1 indicates equality; a 0
 * indicates inequality.
 ******************************************************************************/
int8_t compareGestures(const struct Gesture a, const struct Gesture b);

#endif /* __GESTURE_H__ */

