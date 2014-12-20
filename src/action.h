#ifndef ACTION_H
#define ACTION_H

struct Action
{
	char string[64]; // TODO: replace with more useful format
};

struct Action parseActionString(char* actionString);

void doAction(struct Action action);

#endif
