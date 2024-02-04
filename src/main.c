#include "compression.h"
#include "headers.h"

int main(int argc, char **argv) {

  int numCpu = getNumCPUs();
  printf("you have %d cores on your machine\n", numCpu);

  if (argc < 2) {
    printf("provide a folder name after the command to test file listing\n");
    exit(0);
  }

  char cleanNameCommand[100];
  sprintf(cleanNameCommand, "./scripts/setup_environment.sh %s", argv[1]);
  system(cleanNameCommand);
  

  int sequencialTimeSplit[3];
  int *temp;
  int filesCount = countFiles(argv[1]);
  char **filesList = listFiles(argv[1]);

  char path[100];
  sprintf(path, "%s", filesList[0]);
  int fileSize = getFileSize(path);
  char info[100];

  resetMarkdown("docs/results.md",
                "# Compression comparison\n\n| Compression Type | Number of "
                "Files | Size Per File | Time Taken             "
                "|\n|-----------------|-----------------|---------------|------"
                "------------------|");
  int sequencialTime = compressFilesSequencially(argv[1]);
  temp = secondsToHMS(sequencialTime);
  memcpy(sequencialTimeSplit, temp, 3 * sizeof(int));
  free(temp);

  sprintf(info, "|Sequential|%d files|%d MB|%d minutes and %d seconds|",
          filesCount, fileSize, sequencialTimeSplit[1], sequencialTimeSplit[2]);
  appendToMarkdown("docs/results.md", info);

  int nThreadsTime = compressWithNThreads(argv[1]);
  int nThreadsTimeSplit[3];
  temp = secondsToHMS(nThreadsTime);
  memcpy(nThreadsTimeSplit, temp, 3 * sizeof(int));
  free(temp);

  sprintf(info, "|N threads|%d files|%d MB|%d minutes and %d seconds|",
          filesCount, fileSize, nThreadsTimeSplit[1], nThreadsTimeSplit[2]);

  appendToMarkdown("docs/results.md", info);

  int batchedProcessingTime;
  for (int i = 1; i <= numCpu; i++) {
    batchedProcessingTime = batchedParallelCompression(argv[1], i);
    int batchedProcessingTimeSplit[3];
    temp = secondsToHMS(batchedProcessingTime);
    memcpy(batchedProcessingTimeSplit, temp, 3 * sizeof(int));
    free(temp);

    sprintf(info,
            "|Batched with %d threads|%d files|%d MB|%d minutes and %d "
            "seconds|",
            i, filesCount, fileSize, batchedProcessingTimeSplit[1],
            batchedProcessingTimeSplit[2]);
    appendToMarkdown("docs/results.md", info);
  }

  int nbCoresTasksWithMultFileAssignmentTime;
  for (int i = 1; i <= numCpu; i++) {
    nbCoresTasksWithMultFileAssignmentTime =
        nbCoresTasksWithMultFileAssignment(argv[1], i);
    int nbCoresTasksWithMultFileAssignmentTimeSplit[3];
    temp = secondsToHMS(nbCoresTasksWithMultFileAssignmentTime);
    memcpy(nbCoresTasksWithMultFileAssignmentTimeSplit, temp, 3 * sizeof(int));
    free(temp);

    sprintf(info,
            "|Nb cores tasks with multiple file assignment with %d "
            "threads|%d files|%d MB|%d minutes and %d seconds|",
            i, filesCount, fileSize,
            nbCoresTasksWithMultFileAssignmentTimeSplit[1],
            nbCoresTasksWithMultFileAssignmentTimeSplit[2]);
    appendToMarkdown("docs/results.md", info);
  }

  return 0;
}