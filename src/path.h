#ifndef PATH_H
#define PATH_H

#include "types.h"
#include <sys/stat.h>

#define MAX_PATH_SIZE 1024
#define X_OK 0x01

typedef struct {
    String aPath[256];
    u32 aPathCount;
} Paths;

extern Paths sSystemPaths;

void parsePath(byte *pPath);
i32 findExecutable(const byte *name, byte *outFullPath, u32 outSize);

#endif
