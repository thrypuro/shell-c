#include "command.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

static char *cBuiltIn[] = {
    "exit", "echo", "type" ,"pwd", "cd"
};
static commandType cBuiltInEnum[] = {
    EXIT,
    ECHO,
    TYPE,
    PWD,
    CD,
};
#define TOTAL_BUILTIN (sizeof(cBuiltInEnum) / sizeof(commandType))

static Command sCommandHistory[MAX_COMMAND_HISTORY];
static u32 sCmdIndex = 0;

static void setArguments(byte *aArg, Command *aCurComm) {
    byte *aToken = strtok(aArg, " ");
    i32 num = 0;
    while (aToken != NULL) {
        i32 pNotfound = 1;
        CommandArgument *Com = &aCurComm->aArguments[num];
        memcpy(Com->aArg, aToken, strlen(aToken));
        for (int i = 0; i < TOTAL_BUILTIN; i++) {
            if (strcmp(aToken, cBuiltIn[i]) == 0) {
                pNotfound = 0;
                Com->aType = BUILT_IN;
                break;
            }
        }

        if (pNotfound) {
            if (findExecutable(aToken, Com->aFullPath, sizeof(Com->aFullPath))) {
                pNotfound = 0;
                Com->aType = LIBRARY;
            }
        }
        if (pNotfound) {
            Com->aType = INVALID;
        }
        aToken = strtok(NULL, " ");
        num += 1;
    }
    aCurComm->aArgCount = num;
}

Command parseCommand(byte *pInp) {
    Command aCurComm = sCommandHistory[sCmdIndex];
    byte *aSrc = pInp;
    byte *aToken = strtok_r(aSrc, " ", &aSrc);
    memcpy(aCurComm.aCom, aToken, strlen(aToken));
    for (int i = 0; i < TOTAL_BUILTIN; i++) {
        if (strcmp(aToken, cBuiltIn[i]) == 0) {
            aCurComm.aComType = cBuiltInEnum[i];
            if (aSrc != NULL) {
                memcpy(aCurComm.aArgString, aSrc, strlen(aSrc));
                setArguments(aSrc, &aCurComm);
            }
            sCmdIndex++;
            return aCurComm;
        }
    }

    if (findExecutable(aToken, aCurComm.aFullPath, sizeof(aCurComm.aFullPath))) {
        aCurComm.aComType = COMMAND;
        if (aSrc != NULL) {
            memcpy(aCurComm.aArgString, aSrc, strlen(aSrc));
            setArguments(aSrc, &aCurComm);
        }
        return aCurComm;
    }

    aCurComm.aComType = NONE;
    sCmdIndex++;
    return aCurComm;
}

void runCommand(Command aCommand) {
    char *args[MAX_ARGUMENTS];
    args[0] = aCommand.aCom;
    for (u32 i = 0; i < aCommand.aArgCount; i++) {
        args[i + 1] = aCommand.aArguments[i].aArg;
    }
    args[aCommand.aArgCount + 1] = NULL;

    pid_t pid = fork();
    if (pid == 0) {
        execvp(args[0], args);
        printf("%s: command not found\n", aCommand.aFullPath);
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        int status;
        waitpid(pid, &status, 0);
    } else {
        perror("fork");
    }
}
