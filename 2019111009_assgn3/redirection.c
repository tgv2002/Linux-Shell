# include "headerFiles.h"
# include "redirection.h"
# include "checkers.h"
# include "envVars.h"
# include "overkill.h"
# include "jobControl.h"
# include "lsCommand.h"
# include "cdCommand.h"
# include "echoCommand.h"
# include "historyFunc.h"
# include "nightswatch.h"
# include "pinfoCommand.h"
# include "pwdCommand.h"
# include "executeCmd.h"

char* redirCmd[130];

int redirection(char* command, int* ctrc, int* ctrz, char* homie, int* ptr, char* prevvPath,
		char* printPrevv, char* tempCurr, int* ptr2, char currPath[100001], int* curf)
{
	char* storeHere = (char*)malloc(1030 * sizeof(char));
	strcpy(storeHere, command);
	for(int i=0;i<130;i++)
		redirCmd[i] = (char*)malloc(1030 * sizeof(char));
	char *tokenized = (char*)malloc(1030 * sizeof(char));
	char *sec = (char*)malloc(3 * sizeof(char));
	char *arg2 = (char*)malloc(1030 * sizeof(char));	
	tokenized = strtok(command, " \t"); 
	int ctr = 0, leng;
	int excep = 0;

	while (tokenized != NULL) 
	{ 
		leng = strlen(tokenized);
		redirCmd[ctr++] = tokenized;
		redirCmd[ctr-1][leng] = '\0';
		tokenized = strtok(NULL, " \t"); 
	}

	redirCmd[ctr] = NULL;
	int leftL = 0, rightL = 0, rightR = 0, leftR = 0, retVal = 0;

	for(int i=0;i<ctr;i++)
	{
		excep = 0;

		if(strcmp(redirCmd[i], "<") == 0 || strcmp(redirCmd[i], ">") == 0 ||
				strcmp(redirCmd[i], ">>") == 0)
		{
			if(multips(i+1, redirCmd[i]) == 0)
			{
				perror("Invalid command");
				return 0;
			}

			if(i != ctr - 1 && (strcmp(redirCmd[i+1], "<") == 0 || strcmp(redirCmd[i+1], ">") == 0 ||
						strcmp(redirCmd[i+1], ">>") == 0))
			{
				perror("Invalid command");
				return 0;
			}

			if(i != 0 && (strcmp(redirCmd[i-1], "<") == 0 || strcmp(redirCmd[i-1], ">") == 0 ||
						strcmp(redirCmd[i-1], ">>") == 0))
			{
				perror("Invalid command");
				return 0;
			}

			if(i == 0 || i == ctr - 1)
			{
				perror("Invalid command");
				return 0;
			}

			arg2 = NULL;
			sec = NULL;

			if(i != ctr - 2 && (strcmp(redirCmd[i+2],">") == 0 || strcmp(redirCmd[i+2],">>") == 0))
			{
				if(multips(i+3, redirCmd[i+2]) == 0 || i == ctr - 3)
				{
					perror("Invalid command");
					return 0;
				}

				excep = 1;
				arg2 = redirCmd[i+3];
				sec = redirCmd[i+2];
			}

			if(i != ctr - 2 && (strcmp(redirCmd[i+2],"<") == 0))
			{
				if(multips(i+3, redirCmd[i+2]) == 0 || i == ctr - 3)
				{
					perror("Invalid command");
					return 0;
				}

				excep = 1;
				arg2 = redirCmd[i+3];
				sec = NULL;
			}

			leftR = i - 1;
			rightL = i + 1;
			rightR = rightEnd(i+1, ctr);
			retVal = doRedirection(leftL, leftR, rightL, rightR, redirCmd[i],
					ctrc, ctrz, homie, ptr, prevvPath,
					printPrevv, tempCurr, ptr2, currPath, arg2, sec, curf);
			if(retVal == 0)
			{
				return 0;
			}

			leftL = i + 1;
		}

		if(excep == 1)
			i += 3;
	}

	return 1;
}

int multips(int idx, char* checkHere)
{
	for(int i=idx;i<130 && redirCmd[i] != NULL;i++)
	{
		if(strcmp(redirCmd[i], "<") == 0 || strcmp(redirCmd[i], ">") == 0 ||
				strcmp(redirCmd[i], ">>") == 0)
		{
			if(strcmp(redirCmd[i], checkHere) == 0)
			{
				perror("Invalid argument format");
				return 0;
			}

			if((strcmp(redirCmd[i], ">") == 0 && strcmp(checkHere, ">>") == 0) ||
				(strcmp(redirCmd[i], ">>") == 0 && strcmp(checkHere, ">") == 0))
			{
				perror("Invalid argument format");
				return 0;
			}

			return 1;
		}
	}

	return 1;
}

int rightEnd(int idx, int ctr)
{
	for(int i=idx;i<130 && redirCmd[i] != NULL;i++)
	{
		if(strcmp(redirCmd[i], "<") == 0 || strcmp(redirCmd[i], ">") == 0 ||
				strcmp(redirCmd[i], ">>") == 0)
			return (i-1);
	}

	return (ctr - 1);
}

int execCmd(char* command, int* ctrc, int* ctrz, char* homie, int* ptr, char* prevvPath,
		char* printPrevv, char* tempCurr, int* ptr2, char currPath[100001], int* curf)
{
	int ret = 0;

	if(ret = isSetEnv(command))
	{
		return (setEnvVars(command + ret));
	}

	if(ret = isUnSetEnv(command))
	{
		return (unsetEnvVars(command + ret));
	}

	if(ret = isOverKill(command))
	{
		return (overkill(command + ret, ctrc, ctrz, curf));
	}

	if(ret = isJobs(command))
	{
		return (jobsCommand(command + ret, ctrc, ctrz, curf));
	}

	if(ret = isFgCommand(command))
	{
		return (fgCommand(command + ret, ctrc, ctrz, curf));
	}

	if(ret = isBgCommand(command))
	{
		return (bgCommand(command + ret, ctrc, ctrz, curf));
	}

	if(ret = isKjobCommand(command))
	{
		return (kjobCommand(command + ret, ctrc, ctrz, curf));
	}

	if(ret = isLs(command))
	{
		int ls_sp1 = -1;
		int path_leng = strlen(command);

		for(int i=ret;i<path_leng;i++)
		{
			if(command[i] != ' ' && command[i] != '\t')
			{
				ls_sp1 = i;
				break;
			}
		} 

		if(ls_sp1 != -1)
		{
			return lsCommand(command + ls_sp1, homie);
		}

		else
		{
			return lsCommand("", homie);
		}
	}

	if(ret = isCd(command))
	{
		int cd_sp1 = -1, leng, ctr2;
		int path_leng = strlen(command);
		char* reqPath = (char*)malloc(1030 * sizeof(char));

		for(int i=ret;i<path_leng;i++)
		{
			if(command[i] != ' ' && command[i] != '\t')
			{
				cd_sp1 = i;
				break;
			}
		}

		if(cd_sp1 != -1)
		{
			reqPath = cdCommand(command + cd_sp1, homie, ptr,
					prevvPath, printPrevv, tempCurr, ptr2);

			if(reqPath != NULL)
			{
				strcpy(currPath, reqPath);
				leng = strlen(reqPath);
				ctr2 = 0;
				for(int i=0;i<leng;i++)
					tempCurr[ctr2++] = reqPath[i];
				tempCurr[ctr2] = '\0';
				return 1;
			}

			else
				return 0;
		}

		return 1;
	}

	if(ret = isEcho(command))
	{
		int echo_sp1 = -1;
		int path_leng = strlen(command);

		for(int i=ret;i<path_leng;i++)
		{
			if(command[i] != ' ' && command[i] != '\t')
			{
				echo_sp1 = i;
				break;
			}
		}

		if(echo_sp1 == -1)
			return echoCommand("");
		else
		{
			return echoCommand(command + echo_sp1);
		}    
	}

	if(ret = isHistory(command))
	{
		return printHistory(command + ret, homie);
	}

	if(ret = isNightswatch(command))
	{
		return nightsWatch(command + ret);
	}

	if(ret = isPinfo(command))
	{
		return pinfoCommand(command + ret, homie);
	}

	if(ret = isPwd(command))
	{
		return pwdCommand();
	}
	
	//fprintf(stderr, "fine");
	return executeCmd(command, currPath, ctrc, ctrz, curf);
}

int doRedirection(int leftL, int leftR, int rightL,
		int rightR, char* doThis, int* ctrc,
		int* ctrz, char* homie, int* ptr, char* prevvPath,
		char* printPrevv, char* tempCurr, int* ptr2, char currPath[100001], char* arg2,
		char* sec, int* curf)
{
	char* lefty = (char*)malloc(1030 * sizeof(char));
	char* righty = (char*)malloc(1030 * sizeof(char));
	int l = 0, r = 0, leng, retVal = 0;

	//printf("%d %d %d %d\n", leftL,leftR,rightL,rightR);

	for(int i=leftL;i<=leftR;i++)
	{
		leng = strlen(redirCmd[i]);
		for(int j=0;j<leng;j++)
			lefty[l++] = redirCmd[i][j];
		if(i != leftR)
			lefty[l++] = ' ';
	}

	lefty[l] = '\0';

	for(int i=rightL;i<=rightR;i++)
	{
		leng = strlen(redirCmd[i]);
		for(int j=0;j<leng;j++)
			righty[r++] = redirCmd[i][j];
		if(i != rightR)
			righty[r++] = ' ';
	}

	righty[r] = '\0';

	int inny = dup(0);
	int outy = dup(1);
	int ii = 0, oo = 1;

	if(inny < 0 || outy < 0)
	{
		perror("Execution error");
		return 0;
	}

	if(strcmp(doThis, "<") == 0)
	{
		int fd = open(righty, O_RDONLY);
		ii = fd; 

		if(fd < 0)
		{
			perror("Input file access error");
			return 0;
		}           

		dup2(fd, 0); 
		close(fd);
	}

	if(arg2 != NULL && sec != NULL)
	{
		if(strcmp(sec, ">") == 0)
		{
			int fd = open(arg2, O_WRONLY | O_APPEND |
					O_CREAT | O_TRUNC, (mode_t)0644);
			oo = fd;

			if (fd < 0)
			{
				perror("Output file access error");
				return 0;
			}           

			dup2(fd, 1); 
			close(fd);
		}

		if(strcmp(sec, ">>") == 0)
		{
			int fd = open(arg2, O_WRONLY | O_APPEND |
					O_CREAT, (mode_t)0644);
			
			oo = fd;

			if (fd < 0)
			{
				perror("Output file access error");
				return 0;
			}           

			dup2(fd, 1); 
			close(fd);
		}
	}

	if(arg2 != NULL && sec == NULL)
	{
		int fd = open(arg2, O_RDONLY);
		ii = fd; 

		if(fd < 0)
		{
			perror("Input file access error");
			return 0;
		}           

		dup2(fd, 0); 
		close(fd);
	}

	if(strcmp(doThis, ">") == 0)
	{
		int fd = open(righty, O_WRONLY | O_APPEND |
				O_CREAT | O_TRUNC, (mode_t)0644);
		oo = fd;
		if (fd < 0)
		{
			perror("Output file access error");
			return 0;
		}           

		dup2(fd, 1); 
		close(fd);
	}

	if(strcmp(doThis, ">>") == 0)
	{
		int fd = open(righty, O_WRONLY | O_APPEND |
				O_CREAT, (mode_t)0644);
		
		oo = fd;
		if (fd < 0)
		{
			perror("Output file access error");
			return 0;
		}           

		dup2(fd, 1); 
		close(fd);
	}
	
	//fprintf(stderr, "Input from %d and output from %d\n", ii, oo);
	retVal = execCmd(lefty, ctrc, ctrz, homie, ptr, prevvPath,
			printPrevv, tempCurr, ptr2, currPath, curf);
	//fprintf(stderr, "%s %s %d", lefty, righty, retVal);
	dup2(inny, 0);
	dup2(outy, 1);
	close(inny);
	close(outy);
	return retVal;
}
