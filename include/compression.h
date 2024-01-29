void compressFilesSequencially(const char *folderPath);
void compressWithNThreads(const char *folderPath);
void batchedParallelCompression(const char *folderPath, int batchSize);
void nbCoresTasksWithMultFileAssignment(const char *folderPath, int numberOfCores);
