#include "headers.h"
#include <sys/stat.h>
#include <time.h>

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
