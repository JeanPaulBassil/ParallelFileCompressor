#include "headers.h"
#include "compression.h"

int main(int argc, char **argv) {

  printf("you have %d cores on your machine\n", getNumCPUs());

  if (argc < 2) {
    printf("provide a folder name after the command to test file listing\n");
    exit(0);
  }

  // char **filesList = listFiles(argv[1]);

  // while (*filesList != NULL) {
  //   printf("%s\n", *filesList);
  //   filesList++;
  // }

  compressFilesSequencially(argv[1]);
  return 0;
}