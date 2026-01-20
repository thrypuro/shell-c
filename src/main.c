#include "main.h"

int main() {
    setbuf(stdout, NULL);
    parsePath(getenv("PATH"));

    i32 aContinue = 1;
    while (aContinue) {
        byte aBuffer[MAX_SHELL_INPUT] = {0};
        printf("$ ");
        fgets(aBuffer, MAX_SHELL_INPUT - 1, stdin);
        u32 aSize = strlen(aBuffer);
        if (aBuffer[aSize - 1] == '\n') {
            aBuffer[aSize - 1] = 0;
        }
        byte *aBufPtr = aBuffer;
        while (*aBufPtr && *aBufPtr == ' ') {
            aBufPtr++;
            aSize--;
        }
        Command aCommand = parseCommand(aBufPtr);
        switch (aCommand.aComType) {
            case EXIT:
                aContinue = 0;
                break;
            case ECHO:
                printf("%s\n", aCommand.aArgString);
                break;
            case TYPE:
                printType(aCommand.aArguments->aType,
                    aCommand.aArguments->aArg, aCommand.aArguments->aFullPath);
                break;
            case PWD:
                printCurrentDirectory();
                break;
            case CD:
                changeDirectory(aCommand.aArguments->aArg);
                break;
            case COMMAND:
                runCommand(aCommand);
                break;
            default:
                printf("%s: command not found\n", aBuffer);
        }
    }
    return 0;
}
