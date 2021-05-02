#include "headerFiles.h"
#include "historyFunc.h"

char Command[100001];
char recCommand[25][1030];
char history[25][1030];
FILE* fp;
int fd_req;
int curSize = 0;
int st = 0;
char newl = '\n';
char currentPath[100001];

void addToHistory(char* cmd, char* home)
{
	if(strlen(cmd) >= 1 && (cmd[0] == ' ' || cmd[0] == '\t' || cmd[0] == '\n'))
		return;
	int leng;
	int ctr = 0;
	getcwd(currentPath, sizeof(currentPath));
	int pathChange = chdir(home);

	if(pathChange < 0)
	{
		perror("History access error");
		return;
	}

	fp = fopen("historyFile.txt", "r");

	if(fp == NULL)
	{
		perror("History access error");
		return;
	}

	while(fgets(Command, sizeof(Command), fp) != NULL)
	{
		leng = strlen(Command);
		for(int i=0;i<leng;i++)
			history[ctr][i] = Command[i];
		history[ctr][leng] = '\0';
		ctr++;
	}

	fclose(fp);
	curSize = ctr;

	if(curSize > 0)
	{
		if(strcmp(cmd, history[0]) == 0)
			return;
		st = (curSize == 20) ? (18) : (curSize - 1);

		for(int i=st;i>=0;i--)
		{
			leng = strlen(history[i]);
			for(int j=0;j<leng;j++)
				history[i+1][j] = history[i][j];
			history[i+1][leng] = '\0';
		}
	}
	
	leng = strlen(cmd);
	for(int i=0;i<leng;i++)
		history[0][i] = cmd[i];
	history[0][leng] = '\0';

	if(curSize < 20)
		curSize++;

	fd_req = open("historyFile.txt", O_WRONLY | O_APPEND | O_TRUNC, (mode_t)0600);

	if(fd_req < 0)
	{
		perror("History access error");
		return;
	}

	for(int i=0;i<curSize;i++)
	{
		leng = strlen(history[i]);	
		write(fd_req, history[i], leng);
	}

	close(fd_req);
	pathChange = chdir(currentPath);

	if(pathChange < 0)
	{
		perror("History access error");
		return;
	}
}

int printHistory(char *num, char* home)
{
	int n = 0, dig;
	int leng = strlen(num);
	int n_sp = 0;
	int vis = 0;

	for(int i=0;i<leng;i++)
	{
		if(num[i] != ' ')
		{
			n_sp = i;
			break;
		}
	}

	for(int i=n_sp;i<leng;i++)
	{
		dig = (int)(num[i] - '0');

		if(dig >= 0 && dig <= 9)
		{
			vis = 1;
			n = (n*10 + dig);
		}

		else
		{
			if(num[i] == ' ' || num[i] == '\t' || num[i] == '\n')
				continue;
			perror("Invalid number");
			return 0;
		}
	}

	if(n > 20 || vis == 0)
		n = 20;
	int ctr = 0;

	getcwd(currentPath, sizeof(currentPath));
	int pathChange = chdir(home);

	if(pathChange < 0)
	{
		perror("History access error");
		return 0;
	}

	fp = fopen("historyFile.txt", "r");

	if(fp == NULL)
	{
		perror("History access error");
		return 0;
	}

	while(fgets(Command, sizeof(Command), fp) != NULL)
	{
		leng = strlen(Command);
                for(int i=0;i<leng;i++)
                        recCommand[ctr][i] = Command[i];
               	recCommand[ctr][leng] = '\0';
		ctr++;
	}

	fclose(fp);

	if(n > ctr)
		n = ctr;

	for(int i=n-1;i>=0;i--)
	  printf("%s",recCommand[i]);

	pathChange = chdir(currentPath);

	if(pathChange < 0)
	{
		perror("History access error");
		return 0;
	}

	return 1; 
}
