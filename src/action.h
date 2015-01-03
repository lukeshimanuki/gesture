/* ========================================================================*//**
 * @file action.h
 * @author Luke Shimanuki
 * @date 2 Jan 2015
 * @brief Definition of the action container and related functions.
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

#ifndef __ACTION_H__
#define __ACTION_H__

#include <stdint.h>

/***************************************************************************//**
 * @brief an action to be performed
 *
 * This structure stores a series of keypresses that can be simulated.
 ******************************************************************************/
struct Action
{
	uint16_t commands[32]; /**< Series of keypresses, identified in linux/input.h */
	uint8_t numCommands;   /**< The number of keypresses in this action. */
};

/***************************************************************************//**
 * @brief interprets action setting in config
 *
 * @param actionString A string that contains the sequence of actions.
 *
 * @return An Action object containing the sequence.
 ******************************************************************************/
struct Action parseActionString(char* actionString);

/***************************************************************************//**
 * @brief performs the desired action
 *
 * @param outputFile The simulated linux event stream to write to.
 *
 * @param action The action to perform.
 ******************************************************************************/
void doAction(int outputFile, struct Action action);

#endif /* __ACTION_H__ */

