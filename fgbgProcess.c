#include "headerFiles.h"
#include "fgbgProcess.h"
#include "redirection.h"
#include "bgTerminated.h"
#include "prompt.h"
#include "overkill.h"
#include "checkers.h"
#include "jobControl.h"
#include "pipeIt.h"

char readContent[100001];
char cFile[1030];
char *indivCommand[130];
char *commandArg[130];
char reqCommand[100001];
char *tok;
int status;
int PID;
char storePath[100001];
int ctrlc = 0;
int ctrlz = 0;
int ret2 = 0;
int statVar;

void listenToChild(int signum)
{
	PID = waitpid(-1, &status, WNOHANG);

	if(PID > 0)
	{
		if(WIFEXITED(status))
			bgTerminated(PID, storePath);
		else
		{
			fflush(stderr);fflush(stdin);fflush(stdout);
			fprintf(stderr, "\n%s process with %d pid exited with errors\n", 
					search_node(PID), PID);	
			prompt(storePath, 1, 1);
		}
	}
}

int fgbgProcess(char *command, char* homie, int *ptr, char* prevvPath,
		char* printPrevv, char* tempCurr, int* ptr2, char currPath[100001], int* curf)
{
	fflush(stdin);fflush(stdout);fflush(stderr);
	strcpy(storePath, currPath);
	signal(SIGCHLD, listenToChild);
	int leng = strlen(command);
	int ctr = 0, ctr2 = 0;
	int bg = 0, fg = 0, ub, currId, total, proc_ID;
	int ret;
	int *status;
	int cc=0, zz = 0;
	int isRun = 1;
	int sucy = 1;

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
	int prop = 1;

	for(int i=0;i<ub;i++)
	{
		cc = zz = 0;
		prop = 1;

		if(isRedirection(indivCommand[i]) && !isPiping(indivCommand[i]))
		{
			sucy = redirection(indivCommand[i], &ctrlc, &ctrlz, homie, ptr,
					prevvPath, printPrevv, tempCurr, ptr2, currPath, curf);
			continue;
		}

		if(isPiping(indivCommand[i]))
		{
			sucy = doPiping(indivCommand[i], &ctrlc, &ctrlz, homie, ptr,
					prevvPath, printPrevv, tempCurr, ptr2, currPath, curf);
			continue;
		}

		if(fg == 1 && i == (ub - 1))
		{
			if((ret = isOverKill(indivCommand[i])))
			{
				ret2 = overkill(indivCommand[i] + ret, &ctrlc, &ctrlz, curf);
				if(ret2 == 0)
					sucy = 0;
				continue;
			}

			else if(ret = isJobs(indivCommand[i]))
			{
				ret2 = jobsCommand(indivCommand[i] + ret, &ctrlc, &ctrlz, curf);
				if(ret2 == 0)
					sucy = 0;
				continue;
			}

			else if(ret = isFgCommand(indivCommand[i]))
			{
				ret2 = fgCommand(indivCommand[i] + ret, &ctrlc, &ctrlz, curf);
				if(ret2 == 0)
					sucy = 0;
				continue;
			}

			else if(ret = isBgCommand(indivCommand[i]))
			{
				ret2 = bgCommand(indivCommand[i] + ret, &ctrlc, &ctrlz, curf);
				if(ret2 == 0)
					sucy = 0;
				continue;
			}

			else if(ret = isKjobCommand(indivCommand[i]))
			{
				ret2 = kjobCommand(indivCommand[i] + ret, &ctrlc, &ctrlz, curf);
				if(ret2 == 0)
					sucy = 0;
				continue;
			}
		}

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

			if(fg == 1 && i == (ub - 1))
			{
				fflush(stdin);fflush(stdout);fflush(stderr);
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
					sucy = 0;

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
						printf("\n");
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
					fprintf(stderr, "\n[%d]	Stopped			%s\n", getJobNo(), reqCommand);
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
						//printf("state is %c\n", stt);
						break;
					}
				}

				if(stt == 'Z')
					isRun = 2;
				else if(stt == 'T')
					isRun = 0;
				insert_node(currId, reqCommand, 1, isRun);
				printf("\n[+] %d\n",currId);
			}
		}

		else
		{
			perror("fork error");
			sucy = 0;
			continue;
		}
	}

	ctrlc = ctrlz = 0;
	return sucy;
}
