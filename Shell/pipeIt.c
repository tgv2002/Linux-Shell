#include "headerFiles.h"
#include "pipeIt.h"
#include "redirection.h"
#include "checkers.h"

char* pipeCmd[130];

int doPiping(char* command, int* ctrc, int* ctrz, char* homie, int* ptr, char* prevvPath,
		char* printPrevv, char* tempCurr, int* ptr2, char currPath[100001], int* curf)
{
	int pipeRet[2];
	int retVal = 0;
	for(int i=0;i<130;i++)
		pipeCmd[i] = (char *)malloc(1030 * sizeof(char));
	char *tokenized = (char*)malloc(1030 * sizeof(char));
	tokenized = strtok(command, "|"); 
	int ctr = 0, leng, inny, outy, val2 = 0, val = 1;

	while(tokenized != NULL) 
	{
		leng = strlen(tokenized); 
		pipeCmd[ctr++] = tokenized;
		pipeCmd[ctr-1][leng] = '\0';
		tokenized = strtok(NULL, "|"); 
	}

	pipeCmd[ctr] = NULL;
	int read1 = 0, write1 = 1;

	for(int i=0;i<ctr;i++)
	{
		inny = dup(0);
		outy = dup(1);

		if(pipe(pipeRet) < 0)
		{
			perror("Pipe creation error");
			exit(1);
		}

		if(i != ctr - 1)
			write1 = pipeRet[1];
		else
		{
			//fprintf(stderr, "read1 is %d and write1 is %d\n", read1, write1);
			write1 = 1;
		}

		int val1 = dup2(read1, 0);
		int val2 = dup2(write1, 1);
		//fprintf(stderr, "dup2 for read1 returns %d and for write1 returns %d\n", val1, val2);
		
		if(!isRedirection(pipeCmd[i]))
			retVal = execCmd(pipeCmd[i], ctrc, ctrz, homie, ptr, prevvPath,
					printPrevv, tempCurr, ptr2, currPath, curf);
		else
		{
			//fprintf(stderr, "read1 is %d and write1 is %d\n", read1, write1);
			retVal = redirection(pipeCmd[i], ctrc, ctrz, homie, ptr, prevvPath,
					printPrevv, tempCurr, ptr2, currPath, curf);
		}

		//fprintf(stderr, "pipe[0] = %d and pipe[1] = %d\n", pipeRet[0], pipeRet[1]);

		close(pipeRet[1]);

		read1 = pipeRet[0];
		val = (retVal);
		dup2(inny, 0);
		dup2(outy, 1);
	}

	return val;
}
