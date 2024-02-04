#define _GNU_SOURCE
#include "headers.h"
#include <dirent.h>
#include <sys/stat.h>

// Returns the number of CPUs on the machine
// @param: void
// @return: the number of CPUs
// *
// *
// *
int getNumCPUs() { return sysconf(_SC_NPROCESSORS_ONLN); }

// Counts the number of files in a directory
// @param folderPath: the path to the directory
// @return: the number of files in the directory
// *
// *
// *
int countFiles(const char *folderPath) {
  DIR *dir;
  struct dirent *entry;
  int count = 0;

  dir = opendir(folderPath);
  if (dir == NULL) {
    return -1;
  }
  while ((entry = readdir(dir)) != NULL) {
    if (entry->d_type == DT_REG)
      count++;
  }
  closedir(dir);
  return count;
}

// Lists the files in a directory
// @param folderPath: the path to the directory
// @return: the list of files in the directory
// *
// *
// *
char **listFiles(const char *folderPath) {
  DIR *dir;
  struct dirent *entry;
  char **filesList;
  int filesCount = countFiles(folderPath);

  if (filesCount == -1) {
    filesList = (char **)malloc(sizeof(char *));
    *filesList = NULL;
    printf("Error opening directory <%s>\n", folderPath);
    return filesList;
  }

  dir = opendir(folderPath);

  filesList = (char **)malloc((filesCount + 1) * sizeof(char *));
  int fileIndex = 0;
  char *full_file_name;

  while ((entry = readdir(dir)) != NULL) {
    if (entry->d_type == DT_REG) {
      full_file_name = malloc((strlen(folderPath) + strlen(entry->d_name) + 5) *
                              sizeof(char));
      sprintf(full_file_name, "\"%s/%s\"", folderPath, entry->d_name);
      filesList[fileIndex++] = full_file_name;
    }
  }
  filesList[fileIndex] = NULL;
  closedir(dir);
  return filesList;
}

// Translates seconds to hours, minutes and seconds 
// @param totalSeconds: the total number of seconds
// @return: array containing hours, minutes and seconds
// *
// *
// *
int *secondsToHMS(int totalSeconds) {
  int *hms = (int *)malloc(3 * sizeof(int));
  hms[0] = totalSeconds / 3600;
  hms[1] = (totalSeconds % 3600) / 60;
  hms[2] = totalSeconds % 60;
  return hms;
}

// Incrementally add to markdown file
// @param fileName: the name of the file
// @param content: the content to add to the file
// @return: void
// *
// *
// *
void appendToMarkdown(const char *fileName, const char *content) {
  FILE *file = fopen(fileName, "a");
  if (file == NULL) {
    printf("Error opening file <%s>\n", fileName);
    return;
  }
  fprintf(file, "%s\n", content);
  fclose(file);
}

// reset markdown file
// @param fileName: the name of the file
// @param content: the content to add to the file
// @return: void
// *
// *
// *
void resetMarkdown(const char *fileName, const char *content) {
  FILE *file = fopen(fileName, "w");
  if (file == NULL) {
    printf("Error opening file <%s>\n", fileName);
    return;
  }
  fprintf(file, "%s\n", content);
  fclose(file);
}

// Function to escape spaces in the filename
char* escapeSpaces(char *filename) {
    int spaceCount = 0;

    // Count the number of spaces to determine the new length
    for (int i = 0; filename[i]; i++) {
        if (filename[i] == ' ') spaceCount++;
    }

    // Allocate memory for the new string
    char *escapedFilename = (char *)malloc(strlen(filename) + spaceCount + 1); // +1 for the null terminator
    if (escapedFilename == NULL) {
        printf("Memory allocation failed\n");
        return NULL; // Return NULL in case of memory allocation failure
    }

    // Copy characters and add escape before spaces
    for (int i = 0, j = 0; filename[i]; i++, j++) {
        if (filename[i] == ' ') {
            escapedFilename[j++] = '\\'; // Add escape character
        }
        escapedFilename[j] = filename[i];
    }
    escapedFilename[strlen(filename) + spaceCount] = '\0'; // Null-terminate the new string

    return escapedFilename;
}

// Modified getFileSize function
long getFileSize(char *filename) {
    char *escapedFilename = escapeSpaces(filename);
    if (!escapedFilename) {
        return -1; // Return -1 if escaping failed
    }

    printf("Escaped File name: %s\n", escapedFilename);

    FILE *fp = fopen(escapedFilename, "rb"); // Open the file in binary mode
    free(escapedFilename); // Free the allocated memory for escapedFilename

    if (fp == NULL) {
        printf("File could not be opened\n");
        return -1;
    }

    if (fseek(fp, 0, SEEK_END) < 0) {
        fclose(fp);
        return -1;
    }

    long size = ftell(fp);
    printf("File size: %ld\n", size);
    fclose(fp);
    return size;
}
