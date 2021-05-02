#include "headerFiles.h"
#include "bgTerminated.h"
#include "prompt.h"
#include "checkers.h"
#include "executeCmd.h"

char *indivCommand[130];
char *commandArg[130];
char reqCommand[100001];
char readContent[100001];
char cFile[1030];
char *tok;
int status;
int statVar;
int PID;
char storePath[100001];
int cc = 0, zz = 0;
int isRun = 1;
int prop;

int executeCmd(char* command, char currPath[100001], int* ctrc, int* ctrz, int* curf)
{
	int sucy = 1;
	prop = 1;
	//fflush(stdin);fflush(stdout);fflush(stderr);
	strcpy(storePath, currPath);
	int leng = strlen(command);
	int ctr = 0, ctr2 = 0;
	int bg = 0, fg = 0, ub, currId, total, proc_ID;
	int ret;
	int *status;

	for(int i=0;i<leng;i++)
		if(command[i] == '&')
			bg++;

	char *tokenized = malloc(1030);
	tok = (char *)malloc(1030 * sizeof(char));
	tokenized = strtok(command, "&\n"); 

	for(int i=0;i<130;i++)
	{
		indivCommand[i] = (char*)malloc(1030 * sizeof(char));
		commandArg[i] = (char*)malloc(1030 * sizeof(char));
	}

	while(tokenized != NULL) 
	{ 
		indivCommand[ctr++] = tokenized;
		leng = strlen(tokenized);
		indivCommand[ctr-1][leng] = '\0';
		tokenized = strtok(NULL, "&\n"); 
	}

	total = ub = ctr;
	fg = ctr - bg;

	for(int i=0;i<ub;i++)
	{
		cc = zz = 0;

		currId = fork();
		ctr = 0;
		ctr2 = 0;
		tok = strtok(indivCommand[i], " \t\n");

		while (tok != NULL)
		{
			commandArg[ctr] = tok;
			leng = strlen(tok);

			for(int j=0;j<leng;j++)
			{
				reqCommand[ctr2++] = tok[j];
			}

			commandArg[ctr][leng] = '\0';
			reqCommand[ctr2++] = ' ';
			ctr++;
			tok = strtok(NULL, " \t\n");
		}

		commandArg[ctr] = NULL;
		reqCommand[ctr2-1] = '\0';

		if(currId == 0)
		{
			if(setpgid(currId, currId) < 0)
			{
				perror("setpgid");
				if(fg == 1 && i == (ub-1))
					sucy = 0;
				exit(-1);
			}

			if(execvp(commandArg[0], commandArg) < 0)
			{
				perror("Process execution");
				if(fg == 1 && i == (ub - 1))
					sucy = 0;
				exit(-1);
			}
		}

		else if(currId > 0)
		{
			if((fg == 1) && (i == (ub - 1)))
			{
				*curf = currId;

				if(signal(SIGTTIN, SIG_IGN) == SIG_ERR)
				{
					perror("output signal");
					sucy = 0;
					exit(-1);
				}

				if(signal(SIGTTOU, SIG_IGN) == SIG_ERR)
				{
					perror("input signal");
					sucy = 0;
					exit(-1);
				}

				if(tcsetpgrp(STDIN_FILENO, currId) < 0)
				{
					
				}

				prop = waitpid(currId, &statVar, WUNTRACED);

				if(prop < 0)
				{
					sucy = 0;
				}

				if(!WIFEXITED(statVar))
					sucy = 0;

				if(WIFEXITED(statVar) && (WEXITSTATUS(statVar) != 0))
				{
					sucy = 0;	
				}

				if(WIFSTOPPED(statVar))
				{
					if(*curf != -1)
					{
						setBgStatus(*curf, 1);
						setRunningStatus(*curf, 0);
						*curf = -1;
					}

					zz = 1;
					sucy = 0;
				}

				else if(WIFSIGNALED(statVar))
				{
					if(WTERMSIG(statVar) == 2)
					{
						if(*curf != -1)
						{
							*curf = -1;
						}

						cc = 1;
						sucy = 0;
					}
				}

				proc_ID = getpgrp();

				if(tcsetpgrp(STDIN_FILENO, proc_ID) < 0)
				{

				}

				if(signal(SIGTTIN, SIG_DFL) == SIG_ERR)
				{
					perror("output signal");
					sucy = 0;
					exit(-1);
				}

				if(signal(SIGTTOU, SIG_DFL) == SIG_ERR)
				{
					perror("input signal");
					sucy = 0;
					exit(-1);
				}

				*curf = -1;

				if(zz == 1)
				{
					insert_node(currId, reqCommand, 1, 0);
					printf("\n[%d]	Stopped			%s\n", getJobNo(), reqCommand);
				}
			}

			else
			{
				sprintf(cFile, "/proc/%d/stat", currId);
				FILE* fp = fopen(cFile, "r");
				char stt;
				isRun = 1;
				ctr = 0;

				while(fscanf(fp, "%s", readContent) == 1)
				{
					ctr++;

					if(ctr == 3)
					{
						stt = readContent[0];
						break;
					}
				}

				if(stt == 'Z')
					isRun = 2;
				else if(stt == 'T')
					isRun = 0;
				insert_node(currId, reqCommand, 1, isRun);	
			}	
		}

		else
		{
			perror("fork error");
			sucy = 0;
			continue;
		}
	}
	
	return sucy;
}
