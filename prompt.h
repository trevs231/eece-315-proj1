/* prompt.h
 *
 * EECE 315
 * Lab Group B5 
 *
 * Desc:
 * Contains functions directly used by the shell.
 * Many of these are commands enetered by the user.
 */

#ifndef PROMPT_H
#define PROMPT_H

#include "definitions.h"

/* Upon first run, this welcome screen runs so that
 * the user understand they are running B5's shell.
 * @param: none
 * @return: none
 */
void welcome();


/* Retrieves the environment variables and
 * concatenates a prompt in the following format:
 * loginName@machineName:CWD>>
 * @input: cmdCount, an integer.
 * @output: pPrompt, a char* with the prompt.
 */
char* makePrompt(int cmdCount);

/* Retrieves the $PATH variable and
 * makes sure that its valid. Otherwise sets it
 * to a default.
 * @param: none
 * @return: pPath, a char* with the full path.
 */
char* getPathEnv();

/* Creates a command struct from the command line by
 * breaking up the command line string into tokens.
 * @params: pCommand - a char pointer to the command line
 * @return: command - a struct holding the command line strings
 *	    command->name = the name of the command
 *	    command->argv = an array of command parameters
 * 	    command->argc = the number of arguments in argv[]
 *	    command->isBg = a bool to specify whether a background command is called
 */
command_t* parseCommand(char * pCommand);

/* Converts all spaces outside of quotes to '"' 
 * EG: |This is "a string" with quotes|
 * -> |This"is""a string""with"quotes|
 * @param: string with spaces for delimiters. quotes optional.
 * @return: string with only spaces inside quotes.
 */
char* quoteDelimit(char* command);

/* Takes the command struct and takes approproate action.
 * Will either execute a shell command, program, or output an error message.
 * @params: command - a struct representing the command
 * @return: false if quit or exit were called, true otherwise.
 */
bool processCommand(command_t* command, char* pEnv[]);

/* Prints instructions for shell specific commands
 * @params: none
 * @return: none
 */
void help();

/* Sets the passed enviroment variable to the passed value
 * @param: envName - the name of the environment variable
 * @param: envValue - the value to be set for the variable
 * @return: none
 */
void setEnvVar(char* envName, char* envValue);


/* Deletes the enviroment variable
 * @param: envName - the name of the environment variable
 * @return: none
 */
void unsetEnvVar(char* envName);

/* Prints the value of the environment variable
 * @param: envName - the name of the environment variable
 * @return: none
 */
void getEnvVar(char* envName);

/* Changes the directory to the specified path.
 * @param: pPath, specified path string
 * @return: none
 */
void changeDirectory(char* pPath);

/* Since stdlib.h does not do the wait
 * function prototypes, they are defined
 * here so to avoid all warnings.
 */
void waitpid();
void wait();

#endif
