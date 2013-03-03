#include "definitions.h"
#include "fileGet.h"
#include "prompt.h"

/*
 * @desc: Upon first run, this welcome screen runs so that
 * the user understands they are running B5's shell.
 * @input: none
 * @output: none
*/
void welcome()
{
   printf("\n%s********************************\n", RED);
   printf("* %sThe EECE 315 B5 SHELL        %s*\n", BLUE, RED);
   printf("********************************\n");
   printf("* %sDeveloped by:                %s*\n", BLUE, RED);
   printf("* %sAndrej Satara, Jessica Seto  %s*\n", GREEN, RED);
   printf("* %sTrevor Siemens, Rohit Singla %s*\n", GREEN, RED);
   printf("********************************\n%s", ENDCLR);

   return;
}

/* 
 * @desc: retrieves the environment variables and
 * concatenates a prompt in the following format:
 * loginName@machineName:CWD>>
 * @params: cmdCount, keeps track of command history
 * @return: pPrompt, a char* with the prompt.
*/
char* makePrompt(int cmdCount)
{
   char fileBuffer[FILENAME_MAX] = { 0 };
   char smallBuffer[32] = { 0 };
   char largeBuffer[1024] = { 0 };

   char* pPrompt;   // the Prompt
   char* pHost;    // the machine name
   char* pCWD;     // the Current WOrking Directory
   char* pLogin;   // current Login name

   char intStr[3];
   sprintf(intStr,"%d",cmdCount);

   pLogin = getenv("LOGNAME");

   pCWD = getcwd(fileBuffer,sizeof(fileBuffer));
   pCWD = homeToTilda(pCWD);
   
   pHost = gethostname(smallBuffer,sizeof(smallBuffer)) == 0 ? smallBuffer : "B5";

   // Checking the Host, CWD, and Login 
   if(pHost == NULL){
       printf("ERROR: no HOSTNAME\n");
   }

   if(pCWD == NULL){
       printf("getcwd() ERROR!\n");
   }

   if(pLogin == NULL){
       printf("ERROR: no LOGIN\n");
   }

   if( (pLogin != NULL) && (pHost != NULL) ) {
       strcat(largeBuffer, BLUE);
       strcat(largeBuffer,pLogin);
       strcat(largeBuffer, RED);
       strcat(largeBuffer,"@");
       strcat(largeBuffer, GREEN);
       strcat(largeBuffer,pHost);
       strcat(largeBuffer, YELLOW);

       if(pCWD != NULL) {
           strcat(largeBuffer,":");
           strcat(largeBuffer,pCWD);
       }
	   strcat(largeBuffer,"[");
	   strcat(largeBuffer,intStr);
	   strcat(largeBuffer,"]");
           strcat(largeBuffer,"\n>>");
	   strcat(largeBuffer, ENDCLR);
           pPrompt = largeBuffer;
       } else {
           printf("ERROR: cannot form prompt. set to default\n");
           pPrompt = "???$";
       }

   if( pPrompt == NULL) {
       printf("ERROR: prompt is null \n");
   }

   return pPrompt;
}

/* 
 * @desc: retrieves the $PATH variable and
 * makes sure that its valid. Otherwise sets it
 * to a default.
 * @param: none
 * @return: pPath, a char* with the full path.
 */
char* getPathEnv(){
   char* pPath;
   // get PATH and LOGNAME env variables
   pPath = getenv("PATH");
   char* defaultPath = "/bin";

   if(pPath == NULL){
       pPath = defaultPath; // make default
   }

   return pPath;
}

/* Converts all spaces outside of quotes to '"' 
 * EG: |This is "a string" with quotes|
 * -> |This"is""a string""with"quotes|
 * @param: string with spaces for delimiters. quotes optional.
 *			NOTE: this string is altered!
 * @return: string with only spaces inside quotes.
 */
char* quoteDelimit(char* command){
   if( command == NULL ){
       return NULL;
   }

   bool inQuotes = false;
   int length = strlen(command);
   int i;

   for(i = 0; i < length; i++){
       if(!inQuotes && command[i] == ' '){
 	   command[i] = '\"';
       } else if( !inQuotes && command[i] == '\"' ){
	   inQuotes = true;
       } else if( inQuotes && command[i] == '\"' ){
	   inQuotes = false;
       }
   }
   
   return command;
}

/* Creates a command struct from the command line by
 * breaking up the command line string into tokens.
 * @params: pCommand - a char pointer to the command line
 * @return: command - a struct holding the command line strings
 *	    command->name = the name of the command
 *	    command->argv = an array of command parameters
 * 	    command->argc = the number of arguments in argv[]
 *	    command->isBg = a bool to specify whether a background command is called
 */
command_t* parseCommand(char * pCommand){
   if (strcmp(pCommand, "\n") == CMP_OK){
       return NULL;
   }

   int count = 0;
   int strLength = 0;
   int nameAlloc = 0;
   int argAlloc = 0;
   char * pTemp;	// Temporary pointer to store the remaining string as it is being parsed
   command_t * command;
   command = (command_t *)malloc(sizeof(command_t)+ARGV_SIZE*sizeof(char));

   // First delete the newline character from the command line string
   strLength = strlen(pCommand);
   
   if(pCommand[strLength - 1] == '\n'){
       pCommand[strLength - 1] = '\0';
   }
	
   pCommand =  quoteDelimit(pCommand);
   pTemp = strtok(pCommand,"\"");

   // Parse the command line
   for(count = 0; pTemp != NULL; count++){
           // The first element in the command line is the name of the command
           if (count == 0){
	       nameAlloc = (strlen(pTemp)+1)*sizeof(char);
	       command->name = (char *) malloc(nameAlloc);
	       strcpy(command->name,pTemp);
	       argAlloc = (strlen(pTemp)+1)*sizeof(char);
	       command->argv[count] = (char *) malloc(argAlloc);
	       strcpy(command->argv[count],pTemp);
	       pTemp = strtok(NULL,"\"");
	   } else {
	   // The following elements in the command line are the command parameters
	       argAlloc = (strlen(pTemp)+1)*sizeof(char);
	       command->argv[count] = (char *) malloc(argAlloc);
	       strcpy(command->argv[count],pTemp);
	       pTemp = strtok(NULL,"\"");
	   }
   }

   command->argc = count;

   if( strcmp(command->argv[command->argc-1],"&") == CMP_OK){	
       command->isBg = true;
       command->argv[command->argc-1] = NULL;
       command->argc--;
   }

   return command;
}

/* Takes the command struct and takes approproate action.
 * Will either execute a shell command, program, or output an error message.
 * @params: command - a struct representing the command
 * @return: false is the command was note found, true otherwise.
 */
bool processCommand(command_t* command, char* pEnv[]){

   char* defaultDir = getenv("HOME");

   if( strcmp(command->name, "cd") == CMP_OK) {
       if( command->argc == 1 ) {
	   changeDirectory(defaultDir);
       } else {
	   changeDirectory(command->argv[1]);
       }
   } else if( strcmp(command->name,"help") == CMP_OK) {
       help();
   } else if( strcmp(command->name, "senv") == CMP_OK) {
       char* envName = NULL;
       char* envVal = NULL;

       if( command->argc > 1) {
           envName = command->argv[1];
       } 

       if( command->argc > 2) {
           envVal = command->argv[2];
       }

       setEnvVar(envName,envVal);

   } else if( strcmp(command->name, "usenv") == CMP_OK) {
       char* envName = NULL;
       if( command->argc > 1) {
	       envName = command->argv[1];
       }
       unsetEnvVar(envName);

   } else if( strcmp(command->name, "genv") == CMP_OK) {
       char* envName = NULL;
       if( command-> argc > 1) {
	       envName = command->argv[1];
       }
       getEnvVar(envName);
   } else {
       char* filePath = getFilePath(command->name);
       if(filePath != NULL) {
	   pid_t pid_command = fork();

           if(pid_command < CHILD) { // ERROR
	       fprintf(stderr, "Fork failure!\n");
  	   } else if ( pid_command == CHILD) {
	       if(command->isBg) {
		   freopen("/dev/null","w",stdout);
	           freopen("/dev/null","w",stderr);
	       }

	       execve(filePath, command->argv, pEnv);
	   } else {
	       if(!command->isBg) {
		   int stat;
		   waitpid(pid_command, &stat, 0);
	       } else {
			   printf("Process ID: %d\n", pid_command);
		   }
           }
       } else if(strcmp(command->name, "quit") == CMP_OK ||
		 strcmp(command->name, "exit") == CMP_OK ||
		 strcmp(command->name, "q") == CMP_OK ) {
	   return false;
       } else {
   	   printf("%s: Command not found\n", command->name);
	   return false;
       }
   }  
   return true;
}

/* Prints instructions for shell specific commands
 * @params: none
 * @return: none
 */
void help()
{
   printf("EECE 315 B5 Shell, version 1.0\n");
   printf("These shell commands are defined internally:\n");
   printf("cd [dir]\n");
   printf("help\n");
   printf("senv [env var name] [env var value]\n");
   printf("usenv [env var name]\n");
   printf("genv [env var name]\n");
   printf("quit OR exit OR q\n");
   printf("Insert '&' after a program command to run in background\n");

   return;
}

/* Sets the passed enviroment variable to the passed value
 * @param: envName - the name of the environment variable
 * @param: envValue - the value to be set for the variable
 * @return: none
 */
void setEnvVar(char* envName, char* envValue){
   if( envName == NULL || envValue == NULL ){
       printf("Error: senv requires arguments [environment name] [value]\n");
   } else {
       if( setenv(envName, envValue, 1) == ENV_ERR ){
    	   printf("Error: Could not set %s\n", envName);
       }
   }

   return;
}

/* Deletes the enviroment variable
 * @param: envName - the name of the environment variable
 * @return: none
 */
void unsetEnvVar(char* envName){
   if( envName == NULL ){
       printf("Error: usenv requires arguments [environment name]\n");
   } else {
       if( unsetenv(envName) == ENV_ERR ){
 	   printf("Error: Could not unset %s\n", envName);
       }
   }

   return;
}

/* Prints the value of the environment variable
 * @param: envName - the name of the environment variable
 * @return: none
 */
void getEnvVar(char* envName){
   if( envName == NULL ){
       printf("Error: genv requires arguments [environment name]\n");
   } else{
       printf("%s = %s\n", envName, getenv(envName));
   }

   return;
}

/* void changeDirectory(char* pPath)
 * @desc: Changes the cwd to the speciied path.
 * @input: pPath, specified char* to path.
 * @output: none
 */
void changeDirectory(char* pPath)
{
   int ret;
   char* absPath = tildaToHome(pPath);
   ret = chdir(absPath);
   if(ret == NEG_ONE) {
       printf("I'm sorry Dave, I cannot change to that directory.\n");
   }

   return;
}
