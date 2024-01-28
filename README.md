# Parallel File Compressor

## Overview
Parallel File Compressor is a utility designed to leverage multi-core processors for efficient file compression. By parallelizing compression tasks, this tool aims to significantly reduce the time required to compress large numbers of files, making it ideal for handling bulk data compression in a high-performance computing environment.

## Features
- Utilizes multi-core processors to parallelize compression tasks.
- Offers multiple parallelization strategies to optimize performance based on the system's hardware and workload characteristics.
- Includes utility functions for listing files in a directory and determining the number of CPU cores.

## Prerequisites
- GCC (GNU Compiler Collection) for compiling C programs.
- A Unix-like environment (Linux, macOS) for execution.
- Basic understanding of command-line operations and file management.

## Getting Started

### Cloning the Repository
Start by cloning the repository to your local machine:

```sh
git clone https://github.com/JeanPaulBassil/ParallelFileCompressor.git
cd ParallelFileCompressor
```

### Compiling the Utility
Compile the utility using the provided Makefile:

```sh
make
```

This command compiles the source files and generates an executable.

### Running the Utility
To run the utility, use the following command, replacing `folderName` with the path to the folder containing the files you wish to compress:

```sh
./ParallelFileCompressor folderName
```

## Parallelization Strategies
The utility implements three main parallelization strategies:
1. **Individual File Parallelism**: Each file is compressed in its own parallel task.
2. **Fixed Task Pool**: A fixed number of tasks equal to the number of CPU cores, each compressing a subset of the total files.
3. **Dynamic Task Assignment**: A fixed number of tasks that dynamically receive files to compress until all files are processed.

## Performance Analysis
For insights into the performance gains achieved through each parallelization strategy, refer to the `PERFORMANCE.md` document.
