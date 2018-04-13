#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/**
 * @brief Initializes all things necessary for directory/file operations
 */
void files_init(void);

/**
 * @brief Prepare for reading entries from a directory
 * 
 * @param The path of the directory to read
 */
void startread(char* dirpath);

/**
 * @brief Reads and returns the next entry from the directory currently being read
 * 
 * @return The next entry from the current directory
 */
struct dirent* streamentry(void);

/**
 * @brief Returns the directory currently being read
 * 
 * @return The path to the directory currently being read
 */
char* getcurdir(void);

void directory_go_into(char* dest);

void directory_go_up(void);

/**
 * @brief Returns the next entry from the current directory's name, and prepends the current directory
 * 
 * @return The full path to the next file in the current directory
 */
char* getentry(void);

/**
 * @brief Finishes reading the current directory so the next can read
 */
void endread(void);

/**
 * @brief Attempts to get the format of unknownfile; near redundant code from player.c
 * 
 * @param The path to a file which needs to have its format be determined
 * 
 * @return The file format (reference Formats in player.c for more detail)
 */
int recognizefiletype(FILE* unknownfile);

/**
 * @brief Loops through the current directory, counting the total number of audio/playlist files
 * 
 * @param type Type of files to count, audio or playlist. (reference SelectionType in main.c)
 * 
 * @return The total number of audio/playlist files within the current directory
 */
int getdirsize(int type);

/**
 * @brief Loops through the current directory, counting the total number of audio files
 * 
 * @return The total number of audio files within the current directory
 */
int getalldirsize(void);

/**
 * @brief A wrapper around recognizefiletype, prepends the current directory
 * 
 * @param The name of a file which needs to have its format be determined
 * 
 * @return The file format (reference Formats in player.c)
 */
int detectfiletype(char* filename);

/**
 * @brief Opens and closes a directory to reset it.  Normally this would be done via rewinddir(), but ctrulib stubs it, so ¯\_(ツ)_/¯
 */
void resetdir(void);

/**
 * @brief Checks to see if path contains a file or directory
 * 
 * @param path The path to file/directory to check
 * 
 * @return 1 if it is a directory, 0 otherwise
 */
int isdirectory(char* path);

/**
 * @brief Constructs an array of audio file entry names at entryarray, of size length
 * 
 * @param entryarray Pointer to the array where the names should be placed
 * @param length Number of names to put in the array
 * @param entrytypes Type of entries to put into the array (reference SelectionType in main.c)
 */
void buildentries(char** entryarray, int length, int entrytypes);