#include "echoCommand.h"
#include "headerFiles.h"

int echoCommand(char* value)
{
	char *tokenized = (char*)malloc(1030 * sizeof(char));	
	tokenized = strtok(value, " \t"); 

	while (tokenized != NULL) 
	{ 
		printf("%s ", tokenized); 
		tokenized = strtok(NULL, " \t"); 
	}

	printf("\n"); 
	return 1;
}
