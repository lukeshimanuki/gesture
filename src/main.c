#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <math.h>

#include <string.h>

#include <linux/uinput.h>
#include <linux/input.h>

#include "gesture.h"
#include "action.h"

enum Mode
{
	LISTEN = 0,
	PASS = 1
};

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
	struct Gesture gestures[128];
	struct Action actions[128];
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

	// initialize output file
	int outputFile = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
	ioctl(outputFile, UI_SET_EVBIT, EV_KEY);
	ioctl(outputFile, UI_SET_EVBIT, EV_ABS);
	// enable all keys
	ioctl(outputFile, UI_SET_KEYBIT, KEY_A);
	ioctl(outputFile, UI_SET_KEYBIT, KEY_B);
	ioctl(outputFile, UI_SET_KEYBIT, KEY_C);
	ioctl(outputFile, UI_SET_KEYBIT, KEY_D);
	ioctl(outputFile, UI_SET_KEYBIT, KEY_E);
	ioctl(outputFile, UI_SET_KEYBIT, KEY_F);
	ioctl(outputFile, UI_SET_KEYBIT, KEY_G);
	ioctl(outputFile, UI_SET_KEYBIT, KEY_H);
	ioctl(outputFile, UI_SET_KEYBIT, KEY_I);
	ioctl(outputFile, UI_SET_KEYBIT, KEY_J);
	ioctl(outputFile, UI_SET_KEYBIT, KEY_K);
	ioctl(outputFile, UI_SET_KEYBIT, KEY_L);
	ioctl(outputFile, UI_SET_KEYBIT, KEY_M);
	ioctl(outputFile, UI_SET_KEYBIT, KEY_N);
	ioctl(outputFile, UI_SET_KEYBIT, KEY_O);
	ioctl(outputFile, UI_SET_KEYBIT, KEY_P);
	ioctl(outputFile, UI_SET_KEYBIT, KEY_Q);
	ioctl(outputFile, UI_SET_KEYBIT, KEY_R);
	ioctl(outputFile, UI_SET_KEYBIT, KEY_S);
	ioctl(outputFile, UI_SET_KEYBIT, KEY_T);
	ioctl(outputFile, UI_SET_KEYBIT, KEY_U);
	ioctl(outputFile, UI_SET_KEYBIT, KEY_V);
	ioctl(outputFile, UI_SET_KEYBIT, KEY_W);
	ioctl(outputFile, UI_SET_KEYBIT, KEY_X);
	ioctl(outputFile, UI_SET_KEYBIT, KEY_Y);
	ioctl(outputFile, UI_SET_KEYBIT, KEY_Z);
	ioctl(outputFile, UI_SET_KEYBIT, KEY_0);
	ioctl(outputFile, UI_SET_KEYBIT, KEY_1);
	ioctl(outputFile, UI_SET_KEYBIT, KEY_2);
	ioctl(outputFile, UI_SET_KEYBIT, KEY_3);
	ioctl(outputFile, UI_SET_KEYBIT, KEY_4);
	ioctl(outputFile, UI_SET_KEYBIT, KEY_5);
	ioctl(outputFile, UI_SET_KEYBIT, KEY_6);
	ioctl(outputFile, UI_SET_KEYBIT, KEY_7);
	ioctl(outputFile, UI_SET_KEYBIT, KEY_8);
	ioctl(outputFile, UI_SET_KEYBIT, KEY_9);
	ioctl(outputFile, UI_SET_KEYBIT, KEY_GRAVE);
	ioctl(outputFile, UI_SET_KEYBIT, KEY_MINUS);
	ioctl(outputFile, UI_SET_KEYBIT, KEY_EQUAL);
	ioctl(outputFile, UI_SET_KEYBIT, KEY_LEFTBRACE);
	ioctl(outputFile, UI_SET_KEYBIT, KEY_RIGHTBRACE);
	ioctl(outputFile, UI_SET_KEYBIT, KEY_BACKSLASH);
	ioctl(outputFile, UI_SET_KEYBIT, KEY_SEMICOLON);
	ioctl(outputFile, UI_SET_KEYBIT, KEY_APOSTROPHE);
	ioctl(outputFile, UI_SET_KEYBIT, KEY_COMMA);
	ioctl(outputFile, UI_SET_KEYBIT, KEY_DOT);
	ioctl(outputFile, UI_SET_KEYBIT, KEY_SLASH);
	ioctl(outputFile, UI_SET_KEYBIT, KEY_ESC);

	ioctl(outputFile, UI_SET_KEYBIT, KEY_LEFTCTRL);
	ioctl(outputFile, UI_SET_KEYBIT, KEY_SPACE);
	ioctl(outputFile, UI_SET_KEYBIT, KEY_BACKSPACE);
	ioctl(outputFile, UI_SET_KEYBIT, KEY_ENTER);
	ioctl(outputFile, UI_SET_KEYBIT, KEY_TAB);
	ioctl(outputFile, UI_SET_KEYBIT, KEY_LEFTALT);
	ioctl(outputFile, UI_SET_KEYBIT, KEY_LEFTSHIFT);
	ioctl(outputFile, UI_SET_KEYBIT, KEY_CAPSLOCK);
	ioctl(outputFile, UI_SET_KEYBIT, KEY_NUMLOCK);
	ioctl(outputFile, UI_SET_KEYBIT, KEY_SCROLLLOCK);
	// set device info
	struct uinput_user_dev uidev;
	snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "gesture");
	uidev.id.bustype = BUS_USB;
	uidev.id.vendor = 0;
	uidev.id.product = 0;
	uidev.id.version = 1;
	// send info
	write(outputFile, &uidev, sizeof(uidev));
	ioctl(outputFile, UI_DEV_CREATE);

	int input;
	struct input_event event;

	// open input stream
	if((input = open(inputFile, O_RDONLY)) == -1)
	{
		perror("opening input stream");
		exit(EXIT_FAILURE);
	}

	enum Mode mode = LISTEN;
	struct Gesture gesture = {1, {0}, -1, -1};
	uint8_t numConsecutiveGestures = 0;
	int touching = 0;
	uint32_t x, y;

	// process each entry in input stream
	while(read(input, &event, sizeof(struct input_event)))
	{
		// if pass, then just send the event directly
		if (mode == PASS)
		{
			// if released, then begin listening again
			if (event.type == EV_KEY && event.code == BTN_TOUCH && event.value == 0)
			{
				mode = LISTEN;
			}
			write(outputFile, &event, sizeof(event));
			continue;
		}
		// otherwise, process it
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

								// if no gesture, then pass next touch
								if (numConsecutiveGestures == 0 && gesture.size == 1) mode = PASS;
								break;
							}
							case 1: // press
								numConsecutiveGestures = 0;
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

		// find region
		int16_t dx = x - gesture.x;
		int16_t dy = gesture.y - y;
		int16_t angle = atan2(dy, dx) * 180 / M_PI;
		if (angle < 0) angle += 360;
		int8_t region = getRegion(sqrt(dx * dx + dy * dy), angle, gesture.sequence[gesture.size - 1]);

		// if it didn't match any, it is in the middle zone
		// so treat it as if it is in the same region as before
		if (region == -1)
		{
			region = gesture.sequence[gesture.size - 1];
		}
		if (gesture.size > 1 && region == REGION_CENTER)
		{
			// process gesture
//			printGesture(stderr, gesture);
			// test if any gesture is equivalent to this one
			uint8_t i;
			for (i = 0; i < numGestures; i++)
			{
				if (compareGestures(gesture, gestures[i]))
				{ // if equivalent, perform action
					doAction(outputFile, actions[i]);
					break;
				}
			}

			// reset gesture
			gesture.size = 1;
			gesture.sequence[0] = 0;
			gesture.x = -1;
			gesture.y = -1;

			numConsecutiveGestures++;
		}
		if ((gesture.x != -1 && gesture.y != -1) && region != gesture.sequence[gesture.size - 1])
		{
			gesture.sequence[gesture.size] = region;
			gesture.size++;
		}
	}

	// close output streams
	ioctl(outputFile, UI_DEV_DESTROY);
	close(outputFile);

	return 0;
}
