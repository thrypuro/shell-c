#include "main.h"

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

void setArguments(byte * aArg, Command * aCurComm){
  byte * aToken = strtok(aArg," ");
  i32 num = 0;
  while(aToken != NULL){
    i32 pNotfound = 1;
    CommandArgument * Com = &aCurComm->aArguments[num];
    memcpy(Com->aArg, aToken, strlen(aToken));
    for (int i = 0; i < TOTAL_BUILTIN; i++) {
        if (strcmp(aToken,cBuiltIn[i]) == 0){
          pNotfound = 0;
          Com->aType = BUILT_IN;
  
          break;
        }
    }
    
    if (pNotfound){
      struct stat stats;
      for (int i=0; i < sSystemPaths.aPathCount; i++){
        char tempFullPath[MAX_PATH_SIZE] = {0};
        snprintf(tempFullPath, sizeof(tempFullPath), "%s/%s", sSystemPaths .aPath[i].data, aToken);
        if (stat(tempFullPath, &stats) == 0 ){
          if(!(stats.st_mode & X_OK)){
            continue;
          }
          snprintf(Com->aFullPath, 
            sizeof(Com->aFullPath), "%s/%s", sSystemPaths.aPath[i].data, aToken);
          pNotfound = 0;
          Com->aType = LIBRARY;
          break;
        }
      }
     
    }
    if (pNotfound){
      Com->aType = INVALID;
    }
      // printf("%s: not found\n",aToken);
    aToken = strtok(NULL, " ");
    num +=1;
  }
  aCurComm->aArgCount = num;
}

Command parseCommand(byte * pInp){
  Command aCurComm = sCommandHistory[sCmdIndex];
  byte *aSrc = pInp;
  byte * aToken = strtok_r(aSrc," ",&aSrc);
  memcpy(aCurComm.aCom,aToken,strlen(aToken));
  for (int i = 0; i < TOTAL_BUILTIN; i ++ ){
    // printf("Built in debug %s and %s\n", aToken, cBuiltIn[i]);
    if (strcmp(aToken, cBuiltIn[i]) == 0){
      aCurComm.aComType = cBuiltInEnum[i];
      if (aSrc != NULL){
        memcpy(aCurComm.aArgString,aSrc,strlen(aSrc));
        setArguments(aSrc, &aCurComm);
      } 
      sCmdIndex++;
      return aCurComm;
    }
  }
  struct stat stats;
  for (int i=0; i < sSystemPaths.aPathCount; i++){
    char tempFullPath[MAX_PATH_SIZE] = {0};
    snprintf(tempFullPath, sizeof(tempFullPath), "%s/%s", sSystemPaths .aPath[i].data, aToken);
    if (stat(tempFullPath, &stats) == 0 ){
      if(!(stats.st_mode & X_OK)){
        continue;
      }
      snprintf(aCurComm.aFullPath, 
            sizeof(aCurComm.aFullPath), "%s/%s", sSystemPaths.aPath[i].data, aToken);
      aCurComm.aComType = COMMAND;
      if (aSrc != NULL){
        memcpy(aCurComm.aArgString,aSrc,strlen(aSrc));
        setArguments(aSrc, &aCurComm);
      } 
      return aCurComm;
    }
  }
  aCurComm.aComType = NONE;
  sCmdIndex++;
  return aCurComm;
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

void printType(argumentType t,byte pArg[MAX_COMMAND_SIZE],byte pFullPath[MAX_COMMAND_SIZE]){
  switch (t){
    case BUILT_IN:
      printf("%s is a shell builtin\n",pArg);
      break;
    case LIBRARY:
      printf("%s is %s\n",pArg,pFullPath);
      break;
    default:
      printf("%s: not found\n",pArg);
      break;
  }
  
}

void runCommand(Command aCommand){
  // print all arguments
  char *args[64];
  args[0] = aCommand.aCom;
  for (u32 i = 0; i < aCommand.aArgCount; i++){
    args[i+1] = aCommand.aArguments[i].aArg;
  }
  pid_t pid = fork();
  if (pid == 0) { // child
    execvp(args[0],args);
    printf("%s: command not found\n", aCommand.aFullPath);
    exit(EXIT_FAILURE);
  }else if (pid > 0) {
    int status;
    waitpid(pid, &status, 0);
  } else {
    perror("fork");
  }
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
    Command aCommand = parseCommand(aBufPtr);
    switch (aCommand.aComType){
      case EXIT:
        aContinue = 0;
        break;
      case ECHO:
        // printf("Baby baby baby");
        printf("%s\n", aCommand.aArgString);
        break;
      case TYPE:
        printType(aCommand.aArguments->aType
          ,aCommand.aArguments->aArg,aCommand.aArguments->aFullPath);
        break;
      case COMMAND:
        runCommand(aCommand);
        break;
      default:
        printf("%s: command not found\n",aBuffer);
    }
  }
  return 0;
}
