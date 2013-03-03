/* definitions.h
 *
 * EECE 315
 * Lab group B5
 *
 * Contains includes for multiple libraries used in the project.
 * Contains custom type defs for the shell project
 */

#ifndef SHELL_DEFINITIONS_H
#define SHELL_DEFINITIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/types.h>

// C doesn't have builtin boolean types.
typedef int bool;
#define true 1
#define false 0

static const int NEG_ONE = -1;
static const int ENV_ERR = -1;
static const int CMP_OK = 0;
static const int CHILD = 0;
#define ARGV_SIZE 100

// Define text colours
static const char BLACK[] = "\e[00;30";
static const char RED[] = "\e[00;31m";
static const char GREEN[] = "\e[00;32m";
static const char YELLOW[] = "\e[00;33m";
static const char BLUE[] = "\e[00;34m";
static const char ENDCLR[] = "\e[0m";

/*
  command_t struct
@desc: a generic struct type to hold the command name,
the arguments, and # of arguments.
*/
typedef struct {
   char* name;
   int argc;
   bool isBg;
   char* argv[ARGV_SIZE];
} command_t;

#endif
