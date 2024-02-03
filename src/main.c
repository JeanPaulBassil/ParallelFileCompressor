#include "compression.h"
#include "headers.h"

int main(int argc, char **argv) {

  int numCpu = getNumCPUs();
  printf("you have %d cores on your machine\n", numCpu);

  if (argc < 2) {
    printf("provide a folder name after the command to test file listing\n");
    exit(0);
  }

  int sequencialTimeSplit[3];
  int *temp;

  // int sequencialTime = compressFilesSequencially(argv[1]);
  // temp = secondsToHMS(sequencialTime);
  // memcpy(sequencialTimeSplit, temp, 3 * sizeof(int));
  // free(temp);

  // printf("Sequencial compression took %d hours, %d minutes and %d seconds\n",
  //        sequencialTimeSplit[0], sequencialTimeSplit[1],
  //        sequencialTimeSplit[2]);

  // int nThreadsTime = compressWithNThreads(argv[1]);
  // int nThreadsTimeSplit[3];
  // temp = secondsToHMS(nThreadsTime);
  // memcpy(nThreadsTimeSplit, temp, 3 * sizeof(int));
  // free(temp);

  // printf("Parallel compression took %d hours, %d minutes and %d seconds\n",
  //        nThreadsTimeSplit[0], nThreadsTimeSplit[1], nThreadsTimeSplit[2]);

  // int batchedProcessingTime;
  // for (int i = 1; i <= numCpu; i++) {
  //   batchedProcessingTime = batchedParallelCompression(argv[1], i);
  //   int batchedProcessingTimeSplit[3];
  //   temp = secondsToHMS(batchedProcessingTime);
  //   memcpy(batchedProcessingTimeSplit, temp, 3 * sizeof(int));
  //   free(temp);

  //   printf("Batched parallel compression with %d threads took %d hours, %d "
  //          "minutes and %d seconds\n",
  //          i, batchedProcessingTimeSplit[0], batchedProcessingTimeSplit[1],
  //          batchedProcessingTimeSplit[2]);
  // }

  int nbCoresTasksWithMultFileAssignmentTime;
  for (int i = 1; i <= numCpu; i++) {
    nbCoresTasksWithMultFileAssignmentTime =
        nbCoresTasksWithMultFileAssignment(argv[1], i);
    int nbCoresTasksWithMultFileAssignmentTimeSplit[3];
    temp = secondsToHMS(nbCoresTasksWithMultFileAssignmentTime);
    memcpy(nbCoresTasksWithMultFileAssignmentTimeSplit, temp, 3 * sizeof(int));
    free(temp);
    printf("Nb cores tasks with multiple file assignment with %d threads took "
           "%d hours, %d minutes and %d seconds\n",
           i, nbCoresTasksWithMultFileAssignmentTimeSplit[0],
           nbCoresTasksWithMultFileAssignmentTimeSplit[1],
           nbCoresTasksWithMultFileAssignmentTimeSplit[2]);
  }

  return 0;
}