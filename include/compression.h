int compressFilesSequencially(const char *folderPath);
int compressWithNThreads(const char *folderPath);
int batchedParallelCompression(const char *folderPath, int batchSize);
int nbCoresTasksWithMultFileAssignment(const char *folderPath, int numberOfCores);
