#include "path.h"
#include <string.h>
#include <stdio.h>

Paths sSystemPaths = {0};

void parsePath(byte *pPath) {
    byte *aToken = strtok(pPath, ":");
    u32 i = 0;
    while (aToken != NULL) {
        String aStr = {
            .data = aToken,
            .length = strlen(aToken)
        };
        sSystemPaths.aPath[i] = aStr;
        aToken = strtok(NULL, ":");
        i += 1;
    }
    sSystemPaths.aPathCount = i;
}

int findExecutable(const byte *name, byte *outFullPath, u32 outSize) {
    struct stat stats;
    for (u32 i = 0; i < sSystemPaths.aPathCount; i++) {
        char tempFullPath[MAX_PATH_SIZE] = {0};
        snprintf(tempFullPath, sizeof(tempFullPath), "%s/%s", sSystemPaths.aPath[i].data, name);
        if (stat(tempFullPath, &stats) == 0) {
            if (!(stats.st_mode & X_OK)) {
                continue;
            }
            snprintf(outFullPath, outSize, "%s/%s", sSystemPaths.aPath[i].data, name);
            return 1;
        }
    }
    return 0;
}
