#ifndef ACTION_H
#define ACTION_H

#include <stdint.h>

struct Action
{
	uint16_t commands[32]; // one of the keycodes in input.h
	uint8_t numCommands;
};

struct Action parseActionString(char* actionString);

void doAction(int outputFile, struct Action action);

#endif
