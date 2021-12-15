//
// Created by lmfaoown on 10/7/21.
//
#include "internal_commands.h"

void InternalCommand::environ() {
	extern char **environ;

	char **s = environ;

	for (; *s; s++) {
		printf("%s\n", *s);
	}
}