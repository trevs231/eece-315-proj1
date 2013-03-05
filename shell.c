#include "prompt.h"
#include "fileGet.h"

// Main function.
// Gets the command line arguments from the user.
// ANd runs the desired commands.
int main(int argc, char* argv[], char* envp[]) {

   char fileBuffer[FILENAME_MAX];

   char* pPrompt;
   char* pCommand;
   bool loop = true;
   pid_t pid;
   static int cmdCount = 0;

   // Struct variable for command 
   command_t * command;
   command = NULL;

   pid = fork();

   if(pid < CHILD ){ // ERROR!
       fprintf(stderr, "Forking failed");
       return 1;
   } else if (pid == CHILD){ // Child process

       welcome();
       while(loop == true){
               
           pPrompt = makePrompt(cmdCount);
           printf("%s ",pPrompt);
           pCommand = fgets(fileBuffer,sizeof(fileBuffer),stdin);

           if(command != NULL){
               free(command);
           }
           command = parseCommand(pCommand);
           memset(fileBuffer, 0, sizeof(fileBuffer));

           if (command == NULL){
	       continue;
	   } 

           if(processCommand(command, envp) == true) {
 	       cmdCount++;
           } else if( strcmp(command->name, "quit") == CMP_OK ||
	       strcmp(command->name, "exit") == CMP_OK ||
	       strcmp(command->name, "q") == CMP_OK ) {
	       loop = false;
           }
       }
   } else { // Parent 
       wait(NULL);
       printf("SHELL COMPLETE!\n");
   }
   
   return 0;
}
