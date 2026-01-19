#include "main.h"
#include <stdio.h>

static char *cBuiltIn[] = {
  "exit","echo", "type"
};
static commandType cBuiltInEnum[] =  {
  EXIT,
  ECHO,
  TYPE
};
#define TOTAL_BUILTIN (sizeof(cBuiltInEnum)/ sizeof(commandType))

static Command sCommandHistory[MAX_COMMAND_HISTORY];
static u32 sCmdIndex = 0;

Command parse(char * pInp){
  Command aCurComm = sCommandHistory[sCmdIndex];
  char *aSrc = pInp;
  char * aToken = strtok_r(aSrc," ",&aSrc);
 
  for (i32 i = 0; i < TOTAL_BUILTIN; i ++ ){
    // printf("Built in debug %s and %s\n", aToken, cBuiltIn[i]);
    if (strcmp(aToken, cBuiltIn[i]) == 0){
      aCurComm.aComType = cBuiltInEnum[i];
      if (aSrc != NULL){
        memcpy(aCurComm.aArguments,aSrc,strlen(aSrc));
      } 
      sCmdIndex++;
      return aCurComm;
    }
  }
  aCurComm.aComType = NONE;
  sCmdIndex++;
  return aCurComm;
}

void getType(byte * pCom){
  byte *aSrc = pCom;
  byte * aToken = strtok(aSrc," ");
  while(aToken != NULL){
    // printf("Token is %s",aToken);
    i32 pNotfound = 1;
    for (i32 i = 0; i < TOTAL_BUILTIN; i++) {
        if (strcmp(aToken,cBuiltIn[i]) == 0){
          printf("%s is a shell builtin\n", aToken);
          pNotfound = 0;
          break;
        }
    }
    if (pNotfound){
      printf("%s: not found\n",aToken);
    }
    aToken = strtok(NULL, " ");
  }
}

void parsePath(byte * pPath){
  byte * aToken = strtok(pPath,":");
  u32 i = 0;
  while(aToken != NULL){
    String aStr =  {
      .data = aToken,
      .length = strlen(aToken)
    };
    sSystemPaths.aPath[i] = aStr;
    aToken = strtok(NULL, ":");
    i +=1;
  }
  sSystemPaths.aPathCount = i;
}

int main() {
  // Flush after every printf
  setbuf(stdout, NULL);
  // Read "PATH" variable  
  parsePath(getenv("PATH"));
  
  i32 aContinue = 1;
  while (aContinue) {
    byte aBuffer[MAX_SHELL_INPUT] = {0};
    printf("$ ");
    fgets(aBuffer,MAX_SHELL_INPUT-1,stdin);
    u32 aSize = strlen(aBuffer);
    if (aBuffer[aSize-1] == '\n'){
      aBuffer[aSize-1] = 0;
    }
    byte * aBufPtr = aBuffer;
    while (*aBufPtr && *aBufPtr == ' '){
        aBufPtr++;
        aSize--;
    }
    Command aCommand = parse(aBufPtr);
    switch (aCommand.aComType){
      case EXIT:
        aContinue = 0;
        break;
      case ECHO:
        // printf("Baby baby baby");
        printf("%s\n", aCommand.aArguments);
        break;
      case TYPE:
        getType(aCommand.aArguments);
        break;
      default:
        printf("%s: command not found\n",aBuffer);
    }
  }
  return 0;
}
