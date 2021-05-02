#include "headerFiles.h"
#include "prompt.h"
#include "cdCommand.h"
#include "pwdCommand.h"
#include "echoCommand.h"
#include "lsCommand.h"
#include "pinfoCommand.h"
#include "fgbgProcess.h"
#include "bgTerminated.h"
#include "nightswatch.h"
#include "historyFunc.h"
#include "checkers.h"
#include "envVars.h"
#include "overkill.h"
#include "jobControl.h"
#include "executeCmd.h"
#include "redirection.h"
#include "pipeIt.h"
#include "computeLogic.h"

char currPath[100001];
int cdRet, ret;
char homie[4096];
int cd_sp1 = -1,i,j,k,echo_sp1 = -1,ls_sp1 = -1;
int path_leng, sp;
int printTilda = 1;
int tempy = 1;
jmp_buf buff;
int succ = 1;
int curFg = -1;
int quitty = 0;

void controlCPressed(int signum)
{
	if(curFg != -1)
	{
		kill((pid_t)curFg, 2);
	}

	succ = 0;
}

void controlZPressed(int signum)
{
	if(curFg != -1)
	{
		kill((pid_t)curFg, 20);
	}

	succ = 0;
}

int main()
{
	char* currCmd2;
	char* currCmd;
	char* cmd;
	char* tempCurr;
	char* reqPath;
	char* prevvPath;
	char* printPrevv;
	char* restt;

	signal(SIGINT, controlCPressed);
	signal(SIGTSTP, controlZPressed);
	int ign1 = 0, ign2 = 0;

	size_t leng = 0;
	currPath[0] = '\0';
	getcwd(homie, sizeof(homie));
	currCmd2 = (char *) malloc(1030 * sizeof(char));
	currCmd = (char *) malloc(1030 * sizeof(char));
	cmd = (char *) malloc(1030 * sizeof(char));
	restt = (char *) malloc(1030 * sizeof(char));
	reqPath = (char *) malloc(1030 * sizeof(char));
	prevvPath = (char *) malloc(1030 * sizeof(char));
	printPrevv = (char *) malloc(1030 * sizeof(char));
	tempCurr = (char *) malloc(1030 * sizeof(char));
	printPrevv[0] = '\0';
	tempCurr[0] = '\0';
	int ctr2 = 0;
	strcpy(prevvPath, homie);
	initt();

	if(access("historyFile.txt", F_OK) == -1)
		fclose(fopen("historyFile.txt", "w+"));

	while(1)
	{
		prompt(currPath, 0, printTilda);
		leng = 0;
		quitty = getline(&cmd, &leng, stdin);

		if(quitty == -1)
		{
			overkill("", &ign1, &ign2, &curFg);
			printf("\n");
			return 0;
		}

		restt = cmd;
		fflush(stdin);fflush(stdout);fflush(stderr);
		addToHistory(cmd, homie);

		while(currCmd = strtok_r(restt, ";\n", &restt))
		{
			fflush(stdin);fflush(stdout);fflush(stderr);

			if(strcmp(currCmd, "quit") == 0)
			{
				overkill("", &ign1, &ign2, &curFg);
				return 0;
			}

			sp = 0;
			path_leng = strlen(currCmd);

			for(i=0;i<path_leng;i++)
			{
				if(currCmd[i] != ' ' && currCmd[i] != '\t')
				{
					sp = i;
					break;
				}
			}

			if(isLogical(currCmd))
			{
				succ = computeLogic(currCmd, homie, &printTilda, prevvPath,
						printPrevv, tempCurr, &tempy, currPath, &curFg);
			}

			else if(!isRedirection(currCmd) && !isPiping(currCmd))
			{

				if(path_leng >= (sp+2) && currCmd[sp] == 'c' && currCmd[sp+1] == 'd')
				{ 
					cd_sp1 = -1;

					for(i=sp+2;i<path_leng;i++)
					{
						if(currCmd[i] != ' ' && currCmd[i] != '\t')
						{
							cd_sp1 = i;
							break;
						}
					}

					if(cd_sp1 != -1)
					{
						strcpy(currCmd2, currCmd + cd_sp1);
						reqPath = cdCommand(currCmd2, homie, &printTilda,
								prevvPath, printPrevv, tempCurr, &tempy);

						if(reqPath != NULL)
						{
							succ = 1;
							strcpy(currPath, reqPath);
							leng = strlen(reqPath);
							ctr2 = 0;
							for(i=0;i<leng;i++)
								tempCurr[ctr2++] = reqPath[i];
							tempCurr[ctr2] = '\0';
						}

						else
							succ = 0;
					}
				}

				else if(path_leng >= (sp+7) && currCmd[sp] == 'h' &&
						currCmd[sp+1] == 'i' && currCmd[sp+2] == 's' &&
						currCmd[sp+3] == 't' && currCmd[sp+4] == 'o' &&
						currCmd[sp+5] == 'r' && currCmd[sp+6] == 'y')
					succ = printHistory(currCmd + sp + 7, homie);

				else if(path_leng >= (sp+3) && currCmd[sp] == 'p' &&
						currCmd[sp+1] == 'w' && currCmd[sp+2] == 'd')
					succ = pwdCommand();

				else if(path_leng >= (sp+4) && currCmd[sp] == 'e' && currCmd[sp+1] == 'c' &&
						currCmd[sp+2] == 'h' && currCmd[sp+3] == 'o')
				{
					echo_sp1 = -1;

					for(i=sp+4;i<path_leng;i++)
					{
						if(currCmd[i] != ' ' && currCmd[i] != '\t')
						{
							echo_sp1 = i;
							break;
						}
					}

					if(echo_sp1 == -1)
						echoCommand("");
					else
					{
						strcpy(currCmd2, currCmd + echo_sp1);
						succ = echoCommand(currCmd2);
					}    
				}

				else if(path_leng >= (sp+2) && currCmd[sp] == 'l' && currCmd[sp+1] == 's')
				{
					ls_sp1 = -1;

					for(i=sp+2;i<path_leng;i++)
					{
						if(currCmd[i] != ' ' && currCmd[i] != '\t')
						{
							ls_sp1 = i;
							break;
						}
					} 

					if(ls_sp1 != -1)
					{
						strcpy(currCmd2, currCmd + ls_sp1);
						succ = lsCommand(currCmd2, homie);
					}

					else
					{
						succ = lsCommand("", homie);
					}
				}

				else if(path_leng >= (sp+5) && currCmd[sp] == 'p' &&
						currCmd[sp+1] == 'i' && currCmd[sp+2] == 'n' &&
						currCmd[sp+3] == 'f' && currCmd[sp+4] == 'o')
				{
					strcpy(currCmd2, currCmd + sp + 5);
					succ = pinfoCommand(currCmd2, homie);
				}

				else if(path_leng >= (sp+11) && currCmd[sp] == 'n' && currCmd[sp+1] == 'i' &&
						currCmd[sp+2] == 'g' && currCmd[sp+3] == 'h' &&
						currCmd[sp+4] == 't' && currCmd[sp+5] == 's' &&
						currCmd[sp+6] == 'w' && currCmd[sp+7] == 'a' &&
						currCmd[sp+8] == 't' && currCmd[sp+9] == 'c' && currCmd[sp+10] == 'h')
				{
					strcpy(currCmd2, currCmd + sp + 11);
					succ = nightsWatch(currCmd2);
				}

				else if(ret = isSetEnv(currCmd))
					succ = setEnvVars(currCmd + ret);
				else if(ret = isUnSetEnv(currCmd))	
					succ = unsetEnvVars(currCmd + ret);
				else
				{
					strcpy(currCmd2, currCmd);
					succ = fgbgProcess(currCmd2, homie, &printTilda, prevvPath,
							printPrevv, tempCurr, &tempy, currPath, &curFg);
				}
			}

			else
			{
				strcpy(currCmd2, currCmd);
				succ = fgbgProcess(currCmd2, homie, &printTilda, prevvPath,
						printPrevv, tempCurr, &tempy, currPath, &curFg);
			}
		}

		printf("%s", (succ == 1) ? ":')" : ":'(");
	}

	return 0;
}
