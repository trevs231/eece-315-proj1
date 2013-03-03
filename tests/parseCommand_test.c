#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct command_t
{
	char * name;
	int argc;
	char * argv[100];
};

// Function to parse the command line and break it into tokens
// The first token is stored as the command name
// The subsequent tokens are stored as the argument parameters
struct command_t * parse(char commandLine[])
{
	// Variables
	int count;
	char * pTemp;		// Temporary pointer to store the remaining string as it is being parsed
	struct command_t * command;
	command = (struct command_t *)malloc(sizeof(struct command_t)+100*sizeof(char));

	pTemp = strtok(commandLine," ");

	// Parse the command line
	for(count = 0; pTemp != NULL; count++)
	{
		// The first element in the command line is the name of the command
		if (count == 0)
		{
			command->name = (char *) malloc((strlen(pTemp)+1)*sizeof(char));
			strcpy(command->name,pTemp);
			command->argv[count] = (char *) malloc((strlen(pTemp)+1)*sizeof(char));
			strcpy(command->argv[count],pTemp);
			//printf("%s\n",command->name);
			pTemp = strtok(NULL," ");
		}
		// The following elements in the command line are the command parameters
		else
		{
			command->argv[count] = (char *) malloc((strlen(pTemp)+1)*sizeof(char));
			strcpy(command->argv[count],pTemp);
			//printf("%s\n",command->argv[count]);
			pTemp = strtok(NULL," ");
		}
	}
	command->argc = count;

	return command;
}

int main(void)
{
	int i;
	char commandLine[] = "cc -g -o deviation -S main.c inout.c -lmath";
	struct command_t * command;
	command = (struct command_t *)malloc(sizeof(struct command_t));

	command = parse(commandLine);

	// Check what is in argc
	printf("%d\n",command->argc);
	// Check what is in argv
	for(i = 0; i < command->argc; i++)
	{
		printf("%s\n",command->argv[i]);
	}

	
	system("PAUSE");
	return 0;
}
