#include "builtins.h"
#include <stdio.h>

void printType(argumentType t, byte pArg[MAX_COMMAND_SIZE], byte pFullPath[MAX_COMMAND_SIZE]) {
    switch (t) {
        case BUILT_IN:
            printf("%s is a shell builtin\n", pArg);
            break;
        case LIBRARY:
            printf("%s is %s\n", pArg, pFullPath);
            break;
        default:
            printf("%s: not found\n", pArg);
            break;
    }
}
void printCurrentDirectory() {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
       printf("%s\n", cwd);
    } else {
        perror("getcwd() error");
    }
}