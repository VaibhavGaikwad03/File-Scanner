# File Scanner

## Overview

File Scanner is a multi-threaded file scanning utility that searches for specific strings within files in a given directory and its subdirectories. The program uses a thread pool to speed up the scanning process by distributing tasks across multiple threads. The file extensions to be scanned are configurable via a `config.txt` file.

## Features

- Multi-threaded file scanning using a thread pool.
- Configurable file extensions to specify which files to scan.
- Outputs results to an `output.txt` file.
- Robust error handling for file operations.

## Dependencies

- C++17 or later
- Thread support (`<thread>`, `<mutex>`, `<condition_variable>`, `<future>`)
- Filesystem library (`<filesystem>`)

## Files Description

### `FileScanner.cpp`

Contains the implementation of the `FileScanner` class, which handles the file scanning process including:
- Reading the configuration file for valid file extensions.
- Traversing directories to find files.
- Searching for the specified string within files.
- Using a thread pool to manage concurrent tasks.

### `ThreadPool.cpp`

Contains the implementation of the `ThreadPool` class, which manages a pool of worker threads to execute tasks concurrently. This helps in efficiently utilizing CPU resources for the file scanning process.

### `Thread_Pool.h`

Header file for the `ThreadPool` class, defining its interface and member functions.

### `config.txt`

A configuration file containing the list of file extensions to be scanned. Each extension should be on a new line. For example:
```
.txt
.cpp
.h
```

### `output.txt`

The output file where the paths of files containing the searched string are listed.

### `README.md`

Documentation file providing an overview of the project, setup instructions, and usage guidelines.

## Setup Instructions

1. **Download all the files**  
   Ensure you have all the project files in the same directory.

2. **Edit the config file**  
   Update the `config.txt` file to include the file extensions you want to scan. Add each extension on a new line.

3. **Compile the program**  
   Open a terminal and run the following command to compile the program:
   ```sh
   g++ ThreadPool.cpp FileScanner.cpp -o FileScanner
   ```

4. **Run the program**  
   Execute the program with the following command:
   ```sh
   ./FileScanner <directory_path> <string_to_search>
   ```
   - `<directory_path>`: The path to the directory to be scanned.
   - `<string_to_search>`: The string to search for within the files.

## Usage Example

```sh
./FileScanner /home/user/documents "Thala for a reason"
```

This command will scan all files in `/home/user/documents` and its subdirectories for the string "Thala for a reason". The paths of files containing the string will be written to `output.txt`.

## License

This project is licensed under the MIT License. See the `LICENSE` file for more details.

---

Thank you for using File Scanner! If you have any questions or feedback, please reach out via the project's issue tracker.
