#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include <linux/input.h>

#define CENTER_RADIUS 800
#define CORNER_RADIUS 500

#define REG_CENTER 0
#define REG_UP 1
#define REG_RIGHT 2
#define REG_DOWN 3
#define REG_LEFT 4
#define REG_UP_RIGHT 5
#define REG_DOWN_RIGHT 6
#define REG_DOWN_LEFT 7
#define REG_UP_LEFT 8
// 0 is center
// 1-4 is up right down left
// 5-8 is ur, dr, dl, ul
int getRegion(int dx, int dy)
{
	// if it is within inner circle, it is part of the center
	if (dx * dx + dy * dy < CENTER_RADIUS * CENTER_RADIUS)
	{
		return REG_CENTER;
	}
	// ignore corners for now
	if (dy < 0 && abs(dy) >= abs(dx))
	{
		return REG_UP;
	}
	if (dx > 0 && abs(dx) >= abs(dy))
	{
		return REG_RIGHT;
	}
	if (dy > 0 && abs(dy) >= abs(dx))
	{
		return REG_DOWN;
	}
	if (dx < 0 && abs(dx) >= abs(dy))
	{
		return REG_LEFT;
	}
}

int main()
{
	int file;
	struct input_event event;

	if((file = open("/dev/input/event6", O_RDONLY)) == -1)
	{
		perror("opening device");
		exit(EXIT_FAILURE);
	}

	int touching = 0;
	int xi = -1, yi = -1;
	int x, y;
	// always start at 0 to simplify parsing
	int sequence[20] = {0};
	int sequenceSize = 1;

	while(read(file, &event, sizeof(struct input_event)))
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
							case 0:
							{
								int i;
								for (i = 1; i < sequenceSize; i++)
								{
									printf("%i ", sequence[i]);
								}
								printf("\n");
								break;
							}
							case 1:
								xi = -1;
								yi = -1;
								sequenceSize = 1;
								sequence[0] = 0; // just in case...
								break;
						}
//						printf("EV_KEY: BTN_TOUCH: %i\n", event.value);
						break;
					default:
						break;
				}
				break;
			case EV_ABS:
				switch (event.code)
				{
					case ABS_X:
						x = event.value;
						if (xi == -1)
						{
							xi = x;
						}
//						printf("EV_ABS: ABS_X: %i\n", event.value);
						break;
					case ABS_Y:
						y = event.value;
						if (yi == -1)
						{
							yi = y;
						}
//						printf("EV_ABS: ABS_Y: %i\n", event.value);
						break;
					default:
						break;
				}
				break;
			default:
				break;
		}
		int region = getRegion(x - xi, y - yi);
		if (sequenceSize > 1 && region == 0)
		{
			int i;
			for (i = 1; i < sequenceSize; i++)
			{
				printf("%i ", sequence[i]);
			}
			printf("\n");
			sequenceSize = 1;
			sequence[0] = 0;
		}
		if ((xi != -1 && yi != -1) && region != sequence[sequenceSize - 1])
		{
			sequence[sequenceSize] = region;
			sequenceSize++;
		}
	}

	return 0;
}
