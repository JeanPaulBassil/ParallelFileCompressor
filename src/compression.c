#include "headers.h"
#include <pthread.h>
#include <sys/stat.h>
#include <time.h>

typedef struct {
  char **files;
  int startIdx;
  int endIdx;
} FileBatch;

// Cleans up the memory allocated for the files list
// @param filesList: the list of files
// @param filesCount: the number of files
// *
// *
// *
void cleanUp(char **filesList, int filesCount) {
  for (int i = 0; i < filesCount; i++) {
    free(filesList[i]);
  }
  free(filesList);

  char command[100];
  sprintf(command, "rm -rf output");
  system(command);

  printf("Cleaned up\n");
}

// Ensures that the directory exists, if not, it creates it
// @param dirPath: the path to the directory
// *
// *
// *
void ensureDirectoryExists(const char *dirPath) {
  struct stat st;
  printf("Ensuring directory exists: %s...\n", dirPath);
  if (stat(dirPath, &st) != 0) {
    if (mkdir(dirPath, 0700) == -1) {
      perror("Failed to create directory");
    } else {
      printf("Directory created: %s\n", dirPath);
    }
  } else {
    if (!S_ISDIR(st.st_mode)) {
      fprintf(stderr, "Path exists but is not a directory: %s\n", dirPath);
    }
    printf("Directory already exists: %s\n", dirPath);
  }
}

// Method 1: Sequentially compress files
// @param folderPath: the path to the folder containing the files to compress
// @return: the time taken to compress the files
// *
// *
// *
int compressFilesSequencially(const char *folderPath) {
  time_t start, end;
  start = time(NULL);
  printf("Starting sequencial compression\n");

  int filesCount = countFiles(folderPath);
  char **filesList = listFiles(folderPath);

  ensureDirectoryExists("output");

  char command[100];

  for (int i = 0; i < filesCount; i++) {
    sprintf(command, "gzip -c %s > 'output/meteo%d.gz'", filesList[i], i);
    system(command);
    printf("Compressed file %d/%d\n", i, filesCount);
  }

  end = time(NULL);

  cleanUp(filesList, filesCount);

  return (int)(end - start);
}

// Compresses a file using gzip
// @param filePath: the path to the file to compress
// *
// *
// *
void compressFile(void *filePath) {
  char command[100];
  sprintf(command, "gzip -c %s > 'output/meteo%d.gz'", (char *)filePath,
          (int)filePath);
  system(command);

  return NULL;
}

// Method 2: Parallel compression with N threads
// @param folderPath: the path to the folder containing the files to compress
// @return: the time taken to compress the files
// *
// *
// *
int compressWithNThreads(const char *folderPath) {
  time_t start, end;
  start = time(NULL);
  int filesCount = countFiles(folderPath);
  printf("Starting parallel compression with %d threads\n", filesCount);
  char **filesList = listFiles(folderPath);

  ensureDirectoryExists("output");

  pthread_t threads[filesCount];
  pthread_attr_t attr;

  pthread_attr_init(&attr);
  printf("Compressing files with %d threads\n", filesCount);
  for (int i = 0; i < filesCount; i++) {
    char *filePath = filesList[i];
    pthread_create(&threads[i], &attr, compressFile, (void *)filePath);
    printf("Compressed file %d/%d using thread %d\n", i, filesCount, i);
  }

  for (int i = 0; i < filesCount; i++) {
    pthread_join(threads[i], NULL);
  }

  end = time(NULL);

  cleanUp(filesList, filesCount);

  return (int)(end - start);
}

// Method 3: Parallel compression with batched files
// @param folderPath: the path to the folder containing the files to compress
// @param batchSize: the size of the batch
// @return: the time taken to compress the files
// *
// *
// *
int batchedParallelCompression(const char *folderPath, int batchSize) {
  time_t start, end;
  start = time(NULL);
  int filesCount = countFiles(folderPath);
  printf("Starting batched parallel compression with %d threads\n", batchSize);
  char **filesList = listFiles(folderPath);

  ensureDirectoryExists("output");

  pthread_t threads[filesCount];

  for (int i = 0; i < filesCount; i += batchSize) {
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    int batchEnd = i + batchSize;
    if (batchEnd > filesCount) {
      batchEnd = filesCount;
    }
    for (int j = i; j < batchEnd; j++) {
      char *filePath = filesList[j];
      pthread_create(&threads[j], &attr, compressFile, (void *)filePath);
      printf("Compressed file %d/%d using thread %d\n", j, filesCount, j);
    }
    for (int j = i; j < batchEnd; j++) {
      pthread_join(threads[j], NULL);
    }
  }

  end = time(NULL);

  cleanUp(filesList, filesCount);

  return (int)(end - start);
}

// Compresses multiple files
// @param arg: the batch of files
// *
// *
// *
void *compressMultipleFiles(void *arg) {
  FileBatch *batch = (FileBatch *)arg;
  for (int i = batch->startIdx; i <= batch->endIdx; i++) {
    compressFile((void *)batch->files[i]);
    printf("Compressed file %d/%d using thread %d\n", i, batch->endIdx,
           batch->startIdx);
  }
  free(batch);
  return NULL;
}

// Method 4: Parallel compression with N threads and multiple file assignment
// @param folderPath: the path to the folder containing the files to compress
// @param numberOfCores: the number of cores to use
// @return: the time taken to compress the files
// *
// *
// *
int nbCoresTasksWithMultFileAssignment(const char *folderPath,
                                       int numberOfCores) {
  time_t start, end;
  start = time(NULL);
  int filesCount = countFiles(folderPath);
  printf("Starting parallel compression with %d threads\n", numberOfCores);
  char **filesList = listFiles(folderPath);

  ensureDirectoryExists("output");

  pthread_t threads[numberOfCores];
  int filesPerThread = filesCount / numberOfCores;
  int extraFiles = filesCount % numberOfCores;

  for (int i = 0; i < numberOfCores; i++) {
    int startIdx = i * filesPerThread;
    int endIdx = startIdx + filesPerThread - 1;
    if (i < extraFiles) {
      startIdx += i;
      endIdx += i + 1;
    } else {
      startIdx += extraFiles;
      endIdx += extraFiles;
    }

    FileBatch *batch = malloc(sizeof(FileBatch));
    batch->files = filesList;
    batch->startIdx = startIdx;
    batch->endIdx = endIdx;

    pthread_create(&threads[i], NULL, compressMultipleFiles, (void *)batch);
    printf("Compressed files %d-%d using thread %d\n", startIdx, endIdx, i);
  }
  printf("Waiting for threads to finish\n");

  for (int i = 0; i < numberOfCores; i++) {
    pthread_join(threads[i], NULL);
  }
  end = time(NULL);

  cleanUp(filesList, filesCount);

  return (int)(end - start);
}
