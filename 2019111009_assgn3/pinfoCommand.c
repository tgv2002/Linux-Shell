#include "headerFiles.h"
#include "pinfoCommand.h"

char currFile[100001];
char buff[1000001];
FILE *fp;
char curr[100001];
char state;
int pid, r_link;
int num = 0, dig = 0, leng, ctr = 0;

int pinfoCommand(char* processID, char* home)
{
	char* tokenized = (char *)malloc(1030 * sizeof(char));
	tokenized = strtok(processID, " \t"); 
	bool vis = false;
	int argVal;
	int suc = 1;

	while(tokenized != NULL) 
	{
		num = 0;
		ctr = 0;
		argVal = 0;
		leng = strlen(tokenized);

		if(leng > 6)
		{
			perror("Invalid arguments");
			return 0;
		}

		for(int i=0;i<leng;i++)
		{
			dig = (int)(tokenized[i] - '0');

			if(dig >= 0 && dig <= 9)
				num = (num*10) + dig;
			else
			{
				perror("Invalid arguments");
				argVal = 1;
				suc = 0;
				break;
			}
		}

		if(argVal == 1)
			continue;

		if(getpgid(num) < 0)
		{
			perror("Process doesn't exist for an argument");
			return 0;
		}

		sprintf(currFile, "/proc/%d/stat", num);
		fp = fopen(currFile, "r");

		if(fp == NULL)
		{
			perror("Process Information");
			suc = 0;
			continue;
		}

		printf("pid -- %5d\n", num);

		while(fscanf(fp, "%s", curr) == 1)
		{
			ctr++;

			if(ctr == 3)
				state = curr[0];

			if(ctr == 23)
			{
				printf("Process Status -- %c\n", state);
				printf("memory -- %s\n", curr);
				break;
			}
		}

		sprintf(currFile, "/proc/%d/exe", num);
		r_link = readlink(currFile, buff, sizeof(buff));

		if(r_link < 0)
		{
			perror("Executable location");
			suc = 0;
			continue;
		}

		buff[r_link] = '\0';
		printf("Executable Path -- %s\n", buff);
		fclose(fp);

		if(!vis)
			vis = true;

		tokenized = strtok(NULL, " \t"); 
	}

	if(!vis)
	{
		num = pid = getpid();
		ctr = 0;

		sprintf(currFile, "/proc/%d/stat", num);
		fp = fopen(currFile, "r");

		if(fp == NULL)
		{
			perror("Process Information");
			return 0;
		}

		printf("pid -- %5d\n", pid);

		while(fscanf(fp, "%s", curr) == 1)
		{
			ctr++;

			if(ctr == 3)
				state = curr[0];

			if(ctr == 23)
			{
				printf("Process Status -- %c\n", state);
				printf("memory -- %s\n", curr);
				break;
			}
		}

		sprintf(currFile, "/proc/%d/exe", num);
		r_link = readlink(currFile, buff, sizeof(buff));

		if(r_link < 0)
		{
			perror("Executable location");
			return 0;
		}

		buff[r_link] = '\0';
		printf("Executable Path -- %s\n", buff);
	}

	return suc;
}
