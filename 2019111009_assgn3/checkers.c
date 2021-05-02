#include "headerFiles.h"
#include "checkers.h"

char storeCmd[1030];
char tempS[1030];

int firstNonSpace(char* command)
{
	int path_leng = strlen(command);

	for(int i=0;i<path_leng;i++)
	{
		if(command[i] != ' ' && command[i] != '\t')
		{
			return i;
		}
	}

	return -1;
}

int isRedirection(char* command)
{
	strcpy(storeCmd, command);
	int leng = strlen(storeCmd);

	for(int i=0;i<leng;i++)
	{
		if(i != leng - 1 && storeCmd[i] == '>' && storeCmd[i+1] == '>')
		{
			if(i!=0 && (storeCmd[i-1] != ' ' && storeCmd[i-1] != '\t'))
			{
				//printf("firsty %c\n", storeCmd[i-1]);
				continue;
			}

			if(i<leng-2 && (storeCmd[i+2] != ' ' && storeCmd[i+2] != '\t'))
			{
				//printf("secondy %c\n", storeCmd[i+2]);
				continue;
			}

			return 1;
		}

		if(storeCmd[i] == '>' || storeCmd[i] == '<')
		{
			if(i!=0 && (storeCmd[i-1] != ' ' && storeCmd[i-1] != '\t'))
				continue;
			if(i!=leng-1 && (storeCmd[i+1] != ' ' && storeCmd[i+1] != '\t'))
				continue;
			

			return 1;
		}
	}

	return 0;
}

int isLogical(char* command)
{
	strcpy(storeCmd, command);
	int leng = strlen(storeCmd);

	for(int i=0;i<leng;i++)
	{
		if(storeCmd[i] == '@' || storeCmd[i] == '$')
			return 1;
	}

	return 0;
}

int isPiping(char* command)
{
	strcpy(storeCmd, command);
	int leng = strlen(storeCmd);

	for(int i=0;i<leng;i++)
	{
		if(storeCmd[i] == '|')
			return 1;
	}

	return 0;
}

int isSetEnv(char* command)
{
	strcpy(tempS, command);

	int starty = firstNonSpace(tempS);

	if(starty == -1)
		return 0;
	if(strlen(tempS) < starty + 6)
		return 0;
	tempS[starty + 6] = '\0';
	if(strcmp(tempS + starty, "setenv") == 0)
		return (starty + 6);
	return 0;
}

int isUnSetEnv(char* command)
{
	strcpy(tempS, command);

	int starty = firstNonSpace(tempS);

	if(starty == -1)
		return 0;
	if(strlen(tempS) < starty + 8)
		return 0;
	tempS[starty + 8] = '\0';
	if(strcmp(tempS + starty, "unsetenv") == 0)
		return (starty + 8);
	return 0;
}

int isOverKill(char* command) 
{
	strcpy(tempS, command);
	int starty = firstNonSpace(tempS);

	if(starty == -1)
		return 0;
	int leng = (int)strlen(tempS);

	if(leng < (starty + 8))
	{
		return 0;
	}

	tempS[starty + 8] = '\0';
	if(strcmp(tempS + starty, "overkill") == 0)
		return (starty + 8);
	return 0;
}

int isJobs(char* command)
{
	strcpy(tempS, command);
	int starty = firstNonSpace(tempS);

	if(starty == -1)
		return 0;
	int leng = (int)strlen(tempS);

	if(leng < (starty + 4))
	{
		return 0;
	}

	tempS[starty + 4] = '\0';
	if(strcmp(tempS + starty, "jobs") == 0)
		return (starty + 4);
	return 0;
}

int isBgCommand(char* command)
{
	strcpy(tempS, command);
	int starty = firstNonSpace(tempS);

	if(starty == -1)
		return 0;
	int leng = (int)strlen(tempS);

	if(leng < (starty + 2))
	{
		return 0;
	}

	tempS[starty + 2] = '\0';
	if(strcmp(tempS + starty, "bg") == 0)
		return (starty + 2);
	return 0;
}

int isFgCommand(char* command)
{
	strcpy(tempS, command);
	int starty = firstNonSpace(tempS);

	if(starty == -1)
		return 0;
	int leng = (int)strlen(tempS);

	if(leng < (starty + 2))
	{
		return 0;
	}

	tempS[starty + 2] = '\0';
	if(strcmp(tempS + starty, "fg") == 0)
		return (starty + 2);
	return 0;
}

int isKjobCommand(char* command)
{
	strcpy(tempS, command);
        int starty = firstNonSpace(tempS);

        if(starty == -1)
                return 0;
        int leng = (int)strlen(tempS);

        if(leng < (starty + 4))
        {
                return 0;
        }

        tempS[starty + 4] = '\0';
        if(strcmp(tempS + starty, "kjob") == 0)
                return (starty + 4);
        return 0;
}

int isLs(char* command)
{
	strcpy(tempS, command);
	int starty = firstNonSpace(tempS);

	if(starty == -1)
		return 0;
	int leng = (int)strlen(tempS);

	if(leng < (starty + 2))
	{
		return 0;
	}

	tempS[starty + 2] = '\0';
	if(strcmp(tempS + starty, "ls") == 0)
		return (starty + 2);
	return 0;
}

int isCd(char* command)
{
	strcpy(tempS, command);
	int starty = firstNonSpace(tempS);

	if(starty == -1)
		return 0;
	int leng = (int)strlen(tempS);

	if(leng < (starty + 2))
	{
		return 0;
	}

	tempS[starty + 2] = '\0';
	if(strcmp(tempS + starty, "cd") == 0)
		return (starty + 2);
	return 0;
}

int isEcho(char* command)
{
	strcpy(tempS, command);
	int starty = firstNonSpace(tempS);

	if(starty == -1)
		return 0;
	int leng = (int)strlen(tempS);

	if(leng < (starty + 4))
	{
		return 0;
	}

	tempS[starty + 4] = '\0';
	if(strcmp(tempS + starty, "echo") == 0)
		return (starty + 4);
	return 0;
}

int isHistory(char* command)
{
	strcpy(tempS, command);
	int starty = firstNonSpace(tempS);

	if(starty == -1)
		return 0;
	int leng = (int)strlen(tempS);

	if(leng < (starty + 7))
	{
		return 0;
	}

	tempS[starty + 7] = '\0';
	if(strcmp(tempS + starty, "history") == 0)
		return (starty + 7);
	return 0;
}

int isNightswatch(char* command)
{
	strcpy(tempS, command);
	int starty = firstNonSpace(tempS);

	if(starty == -1)
		return 0;
	int leng = (int)strlen(tempS);

	if(leng < (starty + 11))
	{
		return 0;
	}

	tempS[starty + 11] = '\0';
	if(strcmp(tempS + starty, "nightswatch") == 0)
		return (starty + 11);
	return 0;
}

int isPinfo(char* command)
{
	strcpy(tempS, command);
	int starty = firstNonSpace(tempS);

	if(starty == -1)
		return 0;
	int leng = (int)strlen(tempS);

	if(leng < (starty + 5))
	{
		return 0;
	}

	tempS[starty + 5] = '\0';
	if(strcmp(tempS + starty, "pinfo") == 0)
		return (starty + 5);
	return 0;
}

int isPwd(char* command)
{
	strcpy(tempS, command);
	int starty = firstNonSpace(tempS);

	if(starty == -1)
		return 0;
	int leng = (int)strlen(tempS);

	if(leng < (starty + 3))
	{
		return 0;
	}

	tempS[starty + 3] = '\0';
	if(strcmp(tempS + starty, "pwd") == 0)
		return (starty + 3);
	return 0;
}

