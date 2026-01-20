#ifndef BUILTINS_H
#define BUILTINS_H

#include "command.h"
#include "types.h"
#include "string.h"
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>


void printType(argumentType t, byte pArg[MAX_COMMAND_SIZE], byte pFullPath[MAX_COMMAND_SIZE]);
void printCurrentDirectory();
void changeDirectory(const byte *path);
#endif
