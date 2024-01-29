#include "headers.h"
#include "compression.h"

int main(int argc, char **argv) {

  int numCpu = getNumCPUs();
  printf("you have %d cores on your machine\n", numCpu);

  if (argc < 2) {
    printf("provide a folder name after the command to test file listing\n");
    exit(0);
  }

  // compressFilesSequencially(argv[1]);
  // compressWithNThreads(argv[1]);
  // batchedParallelCompression(argv[1], numCpu);
  nbCoresTasksWithMultFileAssignment(argv[1], numCpu);
  return 0;
}