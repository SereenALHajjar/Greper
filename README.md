# Greper Documentation

## Overview
`greper` is a command-line program designed to recursively search for a specific keyword in all `.txt` files within a specified directory and its subdirectories. It provides a simple yet effective way to locate occurrences of a keyword within text files.

## Build Instructions
To build the `greper` program, follow these steps:

1. Navigate to the `build` directory in your terminal.
2. Run the following command:
    ```sh
    cmake ..
    ```
3. Once CMake has generated the build files, run the build command:
    ```sh
    cmake --build .
    ```

## Usage
After building the `greper` program, you can execute it from the command line. Use the following syntax:
```sh
./greper --dir <directory_path> <keyword>
```
or
```sh 
./greper -d <directory_path> <keyword>
```
## Example
```sh
./greper --dir /home/sereen/WorkSpace/Greper/test ok
```
## Output
When executed, `greper` will output the paths of any text files containing the specified keyword, along with the line numbers where the keyword was found.

## Notes
- The program only searches within .txt files.
- If the specified directory does not exist or is inaccessible, an error message will be displayed.
