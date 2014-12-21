#include "action.h"

#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <linux/input.h>

struct Action parseActionString(char* actionString)
{
	struct Action action = {{0}, 0};
	uint8_t i;
	for (i = 0; i < strlen(actionString); i++)
	{
		if (actionString[i] == '<')
		{   // special key enclosed in <>
			// get word
			char word[16] = {0};
			uint8_t j;
			for (j = 0; actionString[i + j + 1] != '>'; j++) // if no '>', fail
			{
				word[j] = actionString[i + j + 1];
			}
			i += j + 1;
//			printf("%s\n", word);

			// match word to key
			if (strcmp(word, "esc") == 0)
				action.commands[action.numCommands] = KEY_ESC;
			else if (strcmp(word, "ctrl") == 0)
				action.commands[action.numCommands] = KEY_LEFTCTRL;
			else if (strcmp(word, "space") == 0)
				action.commands[action.numCommands] = KEY_SPACE;
			else if (strcmp(word, "backspace") == 0)
				action.commands[action.numCommands] = KEY_BACKSPACE;
			else if (strcmp(word, "enter") == 0)
				action.commands[action.numCommands] = KEY_ENTER;
			else if (strcmp(word, "tab") == 0)
				action.commands[action.numCommands] = KEY_TAB;
			else if (strcmp(word, "alt") == 0)
				action.commands[action.numCommands] = KEY_LEFTALT;
			else if (strcmp(word, "shift") == 0)
				action.commands[action.numCommands] = KEY_LEFTSHIFT;
			else if (strcmp(word, "capslock") == 0)
				action.commands[action.numCommands] = KEY_CAPSLOCK;
			else if (strcmp(word, "numlock") == 0)
				action.commands[action.numCommands] = KEY_NUMLOCK;
			else if (strcmp(word, "scrolllock") == 0)
				action.commands[action.numCommands] = KEY_SCROLLLOCK;
		}
		else
		{
			// match character to key
			switch (actionString[i])
			{
				case 'a': action.commands[action.numCommands] = KEY_A; break;
				case 'b': action.commands[action.numCommands] = KEY_B; break;
				case 'c': action.commands[action.numCommands] = KEY_C; break;
				case 'd': action.commands[action.numCommands] = KEY_D; break;
				case 'e': action.commands[action.numCommands] = KEY_E; break;
				case 'f': action.commands[action.numCommands] = KEY_F; break;
				case 'g': action.commands[action.numCommands] = KEY_G; break;
				case 'h': action.commands[action.numCommands] = KEY_H; break;
				case 'i': action.commands[action.numCommands] = KEY_I; break;
				case 'j': action.commands[action.numCommands] = KEY_J; break;
				case 'k': action.commands[action.numCommands] = KEY_K; break;
				case 'l': action.commands[action.numCommands] = KEY_L; break;
				case 'm': action.commands[action.numCommands] = KEY_M; break;
				case 'n': action.commands[action.numCommands] = KEY_N; break;
				case 'o': action.commands[action.numCommands] = KEY_O; break;
				case 'p': action.commands[action.numCommands] = KEY_P; break;
				case 'q': action.commands[action.numCommands] = KEY_Q; break;
				case 'r': action.commands[action.numCommands] = KEY_R; break;
				case 's': action.commands[action.numCommands] = KEY_S; break;
				case 't': action.commands[action.numCommands] = KEY_T; break;
				case 'u': action.commands[action.numCommands] = KEY_U; break;
				case 'v': action.commands[action.numCommands] = KEY_V; break;
				case 'w': action.commands[action.numCommands] = KEY_W; break;
				case 'x': action.commands[action.numCommands] = KEY_X; break;
				case 'y': action.commands[action.numCommands] = KEY_Y; break;
				case 'z': action.commands[action.numCommands] = KEY_Z; break;
				case '0': action.commands[action.numCommands] = KEY_0; break;
				case '1': action.commands[action.numCommands] = KEY_1; break;
				case '2': action.commands[action.numCommands] = KEY_2; break;
				case '3': action.commands[action.numCommands] = KEY_3; break;
				case '4': action.commands[action.numCommands] = KEY_4; break;
				case '5': action.commands[action.numCommands] = KEY_5; break;
				case '6': action.commands[action.numCommands] = KEY_6; break;
				case '7': action.commands[action.numCommands] = KEY_7; break;
				case '8': action.commands[action.numCommands] = KEY_8; break;
				case '9': action.commands[action.numCommands] = KEY_9; break;
				case '`': action.commands[action.numCommands] = KEY_GRAVE; break;
				case '-': action.commands[action.numCommands] = KEY_MINUS; break;
				case '=': action.commands[action.numCommands] = KEY_EQUAL; break;
				case '[': action.commands[action.numCommands] = KEY_LEFTBRACE; break;
				case ']': action.commands[action.numCommands] = KEY_RIGHTBRACE; break;
				case '\\': action.commands[action.numCommands] = KEY_BACKSLASH; break;
				case ';': action.commands[action.numCommands] = KEY_SEMICOLON; break;
				case '\'': action.commands[action.numCommands] = KEY_APOSTROPHE; break;
				case ',': action.commands[action.numCommands] = KEY_COMMA; break;
				case '.': action.commands[action.numCommands] = KEY_DOT; break;
				case '/': action.commands[action.numCommands] = KEY_SLASH; break;
			}
		}
		action.numCommands++;
	}
	return action;
}

void doAction(int outputFile, struct Action action)
{
	int i;
	for (i = 0; i < action.numCommands; i++)
	{
		// print
		// create event
		struct input_event event;
		gettimeofday(&event.time, NULL);
		event.type = EV_KEY;
		event.code = action.commands[i];
		event.value = 1;
		// send event
		write(outputFile, &event, sizeof(event));

		// release key
		event.value = 0;
		write(outputFile, &event, sizeof(event));
	}
}
