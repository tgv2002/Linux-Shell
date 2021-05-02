#include "headerFiles.h"
#include "envVars.h"

int setEnvVars(char *command)
{
	char *tokenized = (char*)malloc(1030 * sizeof(char));
	char *tempS = (char*)malloc(1030 * sizeof(char));	
	char *varName = (char*)malloc(1030 * sizeof(char));	
	char *varVal = (char*)malloc(1030 * sizeof(char));	
	strcpy(tempS, command);
	tokenized = strtok(tempS, " \t"); 
	int ctr = 0;

	while(tokenized != NULL) 
	{
		ctr++;

		if(ctr == 3)
		{
			perror("Too many arguments passed");
			return 0;
		}

		if(ctr == 1)
			strcpy(varName, tokenized);
		else
		{
			strcpy(varVal, tokenized);
		}

		tokenized = strtok(NULL, " \t"); 
	}

	if(ctr == 0)
	{
		perror("Invalid arguments");
		return 0;
	}

	if(ctr == 2)
		setenv(varName, varVal, 1);
	else
	{
		setenv(varName, "", 1);
	}

	return 1;
}

int unsetEnvVars(char *command)
{
	char *tokenized = (char*)malloc(1030 * sizeof(char));
	char *tempS = (char*)malloc(1030 * sizeof(char));	
	char *varName = (char*)malloc(1030 * sizeof(char));		
	strcpy(tempS, command);
	tokenized = strtok(tempS, " \t"); 
	int ctr = 0;

	while(tokenized != NULL) 
	{
		ctr++;

		if(ctr == 2)
		{
			perror("Too many arguments passed");
			return 0;
		}

		if(ctr == 1)
			strcpy(varName, tokenized);
		tokenized = strtok(NULL, " \t"); 
	}

	if(ctr == 0)
	{
		perror("Invalid arguments");
		return 0;
	}

	if(ctr == 1)
		unsetenv(varName);

	return 1;
}
