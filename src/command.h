#ifndef COMMAND_H
#define COMMAND_H

#include "types.h"
#include "path.h"

#define MAX_COMMAND_SIZE 256
#define MAX_ARGUMENTS 128
#define MAX_COMMAND_HISTORY 1337

typedef enum {
    EXIT,
    ECHO,
    TYPE,
    COMMAND,
    PWD,
    CD,
    NONE,
} commandType;

typedef enum {
    BUILT_IN,
    LIBRARY,
    INVALID
} argumentType;

typedef struct {
    byte aArg[MAX_COMMAND_SIZE];
    byte aFullPath[MAX_PATH_SIZE];
    argumentType aType;
} CommandArgument;

typedef struct {
    commandType aComType;
    byte aCom[MAX_COMMAND_SIZE];
    byte aFullPath[MAX_PATH_SIZE];
    byte aArgString[MAX_PATH_SIZE];
    CommandArgument aArguments[MAX_ARGUMENTS];
    u32 aArgCount;
} Command;

Command parseCommand(byte *pInp);
void runCommand(Command aCommand);

#endif
