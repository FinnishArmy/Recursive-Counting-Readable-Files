/***********************************************************************
name: Ronny Z Valtonen
    readable -- recursively count readable files.
description:
    See CS 360 Files and Directories lectures for details.
***********************************************************************/

/* Includes and definitions */
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

#ifndef MAX_BUF
#define MAX_BUF 1024
#endif

/*********************************************************************
General Goal:
Very basically checks if the given pathname is a directory. You may
then use that information to your situation in other functions.

Paramters:
char *pathname:       The path you are passing (or not) in.

General Run:
If you pass /home/ubuntu/Desktop/test where 'test' is a directory,
it will return that as it is a directory. If 'test' was a file, it
will return it as a file instead.
*********************************************************************/
int isDirectory (char *pathname);


/*********************************************************************
General Goal:
Take as input some path. Recursively count the files that are readable.
If a file is not readable, do not count it. Return the negative value
of any error codes you may get. However, consider what errors you want
to respond to.

Paramters:
char* inputPath:       The path you are passing (or not) in.

General Run:
The function will use 'isDirectory' to check if the passed in
paramter (inputPath) is a directory or a file. It will recursively
go through every file and directories (if it can) and return the number
of regular files with in the path. It will also return the proper errno
according to the proper situation, such as a file that doesn't exist
as one example. It will then close the directory once it has finished.
*********************************************************************/
int readable(char* inputPath);



// Check if the current file is a file or directory.
int isDirectory (char *pathname) {
  struct stat area, *s = &area;
    return (stat (pathname, s) == 0) && S_ISDIR (s->st_mode);
}


int readable(char* inputPath) {
  static int count = 0;
  // Set a buffer for the directory path.
  char cwd[MAX_BUF];
  // Set the errno to 0 for use to check for valid directories.
  errno = 0;

  // Create a struct for the directory to read recursively through.
  struct dirent *dp;

  // If the input directory isn't readable, return the errno.
  if (access(inputPath, R_OK) == -1 && isDirectory(inputPath)) {
    return -(errno);
  }

  // If a path was entered, we will use this path for the function.
  if (inputPath != NULL) {
    DIR * dir = opendir(inputPath);
    // If the directory exists
    if (dir) {
      // Recursively read through the directory.
      while ((dp = readdir(dir)) != NULL) {
          if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
              // Copy the inputPath
              strcpy(cwd, inputPath);
              // Cat a '/' for the next path name
              strcat(cwd, "/");
              // Cat the next path name
              strcat(cwd, dp->d_name);
              // It's a directory... we don't want to count those.
              if (isDirectory(cwd)) {
                // printf("Is a directrory\n");
              }
              // Then it's a file
              else {
                // It's a file and you can access, add to the counter.
                if (access(cwd, R_OK) == 0) {
                  count++;
                }
                // If you can't access the file
                if (access(cwd, R_OK) == -1) {
                  errno = -(errno);
                }
              }
              // Recurse back through the function with the new path.
              readable(cwd);
          }
      }
    }
    // If the directory does not exist.
    else if (ENOENT == errno) {
      return -(errno);
    }

    else {
      return (errno/20);
    }
    // Return the regular file count.
    return count;
    // Close the directory.
    closedir(dir);
  }



  // If a path was not entered, we will use the current working
  // directory for the function.
  else {
    // Print the current working directory if nothing was passed.
    // Use the buffer to get the cwd.
    getcwd(cwd, MAX_BUF);
    DIR * dir = opendir(cwd);
    // The directory exists.

    if (dir) {
      // Recursively read through the directory.
      while ((dp = readdir(dir)) != NULL) {
          if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
              // Make a new path from our base path.
              strcat(cwd, "/");
              strcat(cwd, dp->d_name);
              // Recurse back through the function.
              if (isDirectory(cwd)) {
                // printf("Is a directrory\n");
              }

              else {
                count++;
              }
              // Recurse back through the function with the new path.
              readable(cwd);
          }
      }
    }
    // Close the directory.
    closedir(dir);
  }
  // Exit the function.
  return count;
}

//**  End of Program  **//




// Testing
// int main(int argc, char **argv) {
//     // Pass the path to 'int readable'
//     printf("%d\n", readable(argv[1]));
//   return 0;
// }
