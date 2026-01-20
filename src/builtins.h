#ifndef BUILTINS_H
#define BUILTINS_H

#include "command.h"
#include "types.h"
#include <limits.h>
#include <unistd.h>

void printType(argumentType t, byte pArg[MAX_COMMAND_SIZE], byte pFullPath[MAX_COMMAND_SIZE]);
void printCurrentDirectory();

#endif
