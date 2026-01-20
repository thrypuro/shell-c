#include "types.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#define MAX_COMMAND_SIZE 256
#define MAX_PATH_SIZE 1024
#define MAX_ARGUMENTS 128
#define MAX_SHELL_INPUT 2046
#define MAX_COMMAND_HISTORY 1337

typedef enum {
  EXIT,
  ECHO,
  TYPE,
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
  CommandArgument aArguments[MAX_ARGUMENTS];
} Command;

typedef struct {
    String aPath[256];
    u32 aPathCount;
} Paths;

static Paths sSystemPaths = {0};


