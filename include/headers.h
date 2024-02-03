#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int getNumCPUs();
int countFiles(const char *folderPath);
char **listFiles(const char *folderPath);
int *secondsToHMS(int totalSeconds);
void appendToMarkdown(const char *fileName, const char *content);
void resetMarkdown(const char *fileName, const char *content);

