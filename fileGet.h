/* 	fileGet.h
*	Header for fileGet.c
*
*	EECE 315
*	Lab group B5
*
*	Functions for retrieving/checking files 
*/

#ifndef FILEGET_H
#define FILEGET_H

#include "definitions.h"

/* Checks if a directory exists
 * @output: false if no directory, true if exists
 */
bool doesDirExist(char* path);

/* Checks if file exists
 * @output: false if file does not exist, 
 * 	    or path is a directory. true if exists.
 */ 
bool doesFileExist(char* path);

/* If path begins with '~', it replaces it with the current absolute
 * HOME path.
 * @output: A newly allocated string with the full path.
 * 	   If path does not start with a ~, returns path, unaltered.
 */
char* tildaToHome(char* path);

/* If path is in the HOME dir, replaces HOME with '~'
 * @output: A newly allocated string, with the abbreviated path
 *	    If path is not in HOME, returns path, unaltered.
 */
char* homeToTilda(char* path);

/* Get the absolute path of a file. Searches first in PATH.
 * If not found in PATH, will check if fileName is already 
 * an absolute OR relative path.
 * Accepts absolute paths with '~' representing HOME
 * @param: string of the name or path of a file
 * @return: string of absolute path of the file if found. NULL otherwise.
 */
char* getFilePath(char* fileName);

/* @output: An array of strings, which are the PATH environment values
 * 	    The last element in the array is set to NULL
 */
char** getPaths();

#endif
