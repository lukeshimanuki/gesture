#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include <string.h>

#include <linux/input.h>

#include "gesture.h"
#include "action.h"

void printGesture(FILE* stream, struct Gesture gesture)
{
	int i;
	for (i = 0; i < gesture.size; i++)
	{
		fprintf(stream, "%i ", gesture.sequence[i]);
	}
	fprintf(stream, "\n");
}

int main()
{
	FILE* config = stdin;

	// find input file path, should be first string
	char inputFile[64];
	fscanf(config, "%s", inputFile);

	// read configured gestures
	struct Gesture gestures[32];
	struct Action actions[32];
	uint8_t numGestures = 0;
	while (1)
	{
		// read sequence
		char sequence[16];
		char action[64];
		int bytesRead = fscanf(config, "%s %s", sequence, action);
		// if nothing happened or ran out of bytes
		if (bytesRead == 0 || bytesRead == EOF)
		{
			break;
		}
		// store sequence
		gestures[numGestures].size = strlen(sequence);
		uint8_t i;
		for (i = 0; i < gestures[numGestures].size; i++)
		{
			gestures[numGestures].sequence[i] = sequence[i] - 48; // reduce ascii to int
		}

		// process action
		actions[numGestures] = parseActionString(action);

		numGestures++;
	}

	int input;
	struct input_event event;

	// open input stream
	if((input = open(inputFile, O_RDONLY)) == -1)
	{
		perror("opening input stream");
		exit(EXIT_FAILURE);
	}

	struct Gesture gesture = {1, {0}, -1, -1};

	int touching = 0;
	uint32_t x, y;

	// process each entry in input stream
	while(read(input, &event, sizeof(struct input_event)))
	{
		switch (event.type)
		{
			case EV_KEY:
				switch (event.code)
				{
					case BTN_TOUCH:
						touching = event.value;
						switch (event.value)
						{
							case 0: // release
							{
								// jump to center to end gesture
								x = gesture.x;
								y = gesture.y;
								break;
							}
							case 1: // press
								break;
						}
						break;
					default:
						break;
				}
				break;
			case EV_ABS:
				switch (event.code)
				{
					case ABS_X:
						if (touching)
						{
							x = event.value;
							if (gesture.x == -1)
							{
								gesture.x = x;
							}
						}
						break;
					case ABS_Y:
						if (touching)
						{
							y = event.value;
							if (gesture.y == -1)
							{
								gesture.y = y;
							}
						}
						break;
					default:
						break;
				}
				break;
			default:
				break;
		}
		int8_t region = getRegion(x - gesture.x, y - gesture.y);
		// if it didn't match any, it is in the middle zone
		// so treat it as if it is in the same region as before
		if (region == -1)
		{
			region = gesture.sequence[gesture.size - 1];
		}
		if (gesture.size > 1 && region == REGION_CENTER)
		{
			// process gesture
			printGesture(stderr, gesture);
			// test if any gesture is equivalent to this one
			uint8_t i;
			for (i = 0; i < numGestures; i++)
			{
				if (compareGestures(gesture, gestures[i]))
				{ // if equivalent, perform action
					doAction(actions[i]);
					printf("gesture %i\n", i);
					break;
				}
			}

			// reset gesture
			gesture.size = 1;
			gesture.sequence[0] = 0;
		}
		if ((gesture.x != -1 && gesture.y != -1) && region != gesture.sequence[gesture.size - 1])
		{
			gesture.sequence[gesture.size] = region;
			gesture.size++;
		}
	}

	return 0;
}
