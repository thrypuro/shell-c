#ifndef PATH_H
#define PATH_H

#include "types.h"
#include <sys/stat.h>

#define X_OK 0x01

typedef struct {
    String aPath[256];
    u32 aPathCount;
} Paths;

extern Paths sSystemPaths;

void parsePath(byte *pPath);
int findExecutable(const byte *name, byte *outFullPath, u32 outSize);

#endif
