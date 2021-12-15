//
// Created by lmfaoown on 10/7/21.
//
#include "internal_commands.h"

void InternalCommand::pause() {
	while (getchar() != '\n');
}