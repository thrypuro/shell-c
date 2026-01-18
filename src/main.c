#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_SHELL_INPUT 2046
#define MAX_COMMAND_HISTORY 1337

typedef enum commands {
  EXIT,
  ECHO,
  INVALID
} commands;

typedef struct Command {
  commands aCommand;
  char aArguments[MAX_SHELL_INPUT];
  uint32_t aArgLen;
} Command;

const char *cBuiltIn[] = {
  "exit","echo"
};
const commands cBuiltInEnum[] =  {
  EXIT,
  ECHO
};

#define TOTAL_BUILTIN (sizeof(cBuiltInEnum))/(sizeof(commands))

static Command sCommandHistory[MAX_COMMAND_HISTORY];
static uint32_t sCmdIndex = 0;

Command parse(char * pInp){
  Command aCurComm = sCommandHistory[sCmdIndex];

  char *aSrc = pInp;
  
  char * aToken = strtok_r(aSrc," ",&aSrc);
 
  for (int i = 0; i < TOTAL_BUILTIN; i ++ ){
    // printf("Built in debug %s and %s\n", aToken, cBuiltIn[i]);
    if (strcmp(aToken, cBuiltIn[i]) == 0){
      aCurComm.aCommand = cBuiltInEnum[i];
      // printf(" I am inside the rest is : %s",aSrc);

      if (aSrc != NULL){
        memcpy(aCurComm.aArguments,aSrc,strlen(aSrc));
      } 
      sCmdIndex++;
      return aCurComm;
    }
  }
  aCurComm.aCommand = INVALID;
  sCmdIndex++;
  return aCurComm;
}

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);
  int aContinue = 1;
  while (aContinue) {
    char aBuffer[MAX_SHELL_INPUT] = {0};
    printf("$ ");
    fgets(aBuffer,MAX_SHELL_INPUT-1,stdin);
    uint32_t aSize = strlen(aBuffer);
    if (aBuffer[aSize-1] == '\n'){
      aBuffer[aSize-1] = 0;
    }
    char * aBufPtr = aBuffer;
    while (*aBufPtr && *aBufPtr == ' '){
        aBufPtr++;
    }
    Command aCommand = parse(aBufPtr);
    switch (aCommand.aCommand){
      case EXIT:
        aContinue = 0;
        break;
      case ECHO:
        // printf("Baby baby baby");
        printf("%s\n", aCommand.aArguments);
        break;
      default:
        printf("%s: command not found\n",aBuffer);
    }
  }
  return 0;
}
