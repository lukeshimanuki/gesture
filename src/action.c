#include "action.h"

#include <stdio.h>
#include <string.h>

struct Action parseActionString(char* actionString)
{
	struct Action action;
	strcpy(action.string, actionString);
	return action;
}

void doAction(struct Action action)
{
	printf("%s\n", action.string);
}
