#include "headers.h"
#include <pthread.h>
#include <sys/stat.h>
#include <time.h>

typedef struct {
  char **files;
  int startIdx;
  int endIdx;
} FileBatch;

void ensureDirectoryExists(const char *dirPath) {
  struct stat st;

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
  }
}

void compressFilesSequencially(const char *folderPath) {
  time_t start, end;
  start = time(NULL);
  printf("Starting sequencial compression\n");

  int filesCount = countFiles(folderPath);
  char **filesList = listFiles(folderPath);

  ensureDirectoryExists("output");

  char command[100];

  for (int i = 0; i < filesCount; i++) {
    sprintf(command, "gzip -v -c %s > 'output/meteo%d.gz'", filesList[i], i);
    system(command);
  }

  end = time(NULL);
  printf("Time taken to compress %d files sequencially is %ld seconds\n",
         filesCount, end - start);
}

void compressFile(void *filePath) {
  char command[100];
  sprintf(command, "gzip -v -c %s > 'output/meteo%d.gz'", (char *)filePath,
          (int)filePath);
  system(command);
}

void compressWithNThreads(const char *folderPath) {
  time_t start, end;
  start = time(NULL);
  int filesCount = countFiles(folderPath);
  printf("Starting parallel compression with %d threads\n", filesCount);
  char **filesList = listFiles(folderPath);

  ensureDirectoryExists("output");

  pthread_t threads[filesCount];
  pthread_attr_t attr;

  pthread_attr_init(&attr);

  for (int i = 0; i < filesCount; i++) {
    char *filePath = filesList[i];
    pthread_create(&threads[i], &attr, compressFile, (void *)filePath);
  }

  for (int i = 0; i < filesCount; i++) {
    pthread_join(threads[i], NULL);
  }

  end = time(NULL);

  printf("Time taken to compress %d files with %d threads is %ld seconds\n",
         filesCount, filesCount, end - start);
}

void batchedParallelCompression(const char *folderPath, int batchSize) {
  time_t start, end;
  start = time(NULL);
  int filesCount = countFiles(folderPath);
  printf("Starting parallel compression with %d threads\n", filesCount);
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
    }
    for (int j = i; j < batchEnd; j++) {
      pthread_join(threads[j], NULL);
    }
  }

  end = time(NULL);

  printf("Time taken to compress %d files with %d threads is %ld seconds\n",
         filesCount, batchSize, end - start);
}

void *compressMultipleFiles(void *arg) {
  FileBatch *batch = (FileBatch *)arg;
  for (int i = batch->startIdx; i <= batch->endIdx; i++) {
    compressFile((void *)batch->files[i]);
  }
  free(batch);
  return NULL;
}

void nbCoresTasksWithMultFileAssignment(const char *folderPath,
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
  }

  for (int i = 0; i < numberOfCores; i++) {
    pthread_join(threads[i], NULL);
  }

  end = time(NULL);
  printf("Time taken to compress %d files with %d threads is %ld seconds\n",
         filesCount, numberOfCores, end - start);
}
