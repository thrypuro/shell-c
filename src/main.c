#include <stdio.h>
#include <stdlib.h>

#define MAX_SHELL_INPUT 128

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);

  while (1){
    char aBuffer[MAX_SHELL_INPUT];
    // TODO: Uncomment the code below to pass the first stage
    printf("$ ");
    scanf("%127s",aBuffer);
    printf("%s: command not found\n",aBuffer);
  }
  return 0;
}
