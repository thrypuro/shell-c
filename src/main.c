#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SHELL_INPUT 128
#define EXIT_STRING "exit"

typedef enum commands {
  EXIT,
  INVALID
} commands;

commands parse(char pInp[MAX_SHELL_INPUT]){
  int aBool = strcmp(pInp,EXIT_STRING);
  if (aBool == 0){
    return EXIT;
  }
  return INVALID;
}

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);

  while (1){
    char aBuffer[MAX_SHELL_INPUT];
    // TODO: Uncomment the code below to pass the first stage
    printf("$ ");
    scanf("%127s",aBuffer);
    commands aParsed = parse(aBuffer);
    if (aParsed == EXIT) {
      break;
    }
    printf("%s: command not found\n",aBuffer);
  }
  return 0;
}
