#include "headerFiles.h"
#include "computeLogic.h"
#include "redirection.h"
#include "pipeIt.h"
#include "checkers.h"
#include "executeCmd.h"

char* eqn[130];

int computeLogic(char* command, char* homie, int* ptr, char* prevvPath,
		char* printPrevv, char* tempCurr, int* ptr2, char currPath[100001], int* curf)
{
	int sz = setEquation(command);
	int store, ign1=0, ign2=0, ret;

	if(isPiping(eqn[0]))
		store = doPiping(eqn[0], &ign1, &ign2, homie, ptr, prevvPath,
				printPrevv, tempCurr, ptr2, currPath, curf);
	else if(isRedirection(eqn[0]))
		store = redirection(eqn[0], &ign1, &ign2, homie, ptr, prevvPath,
				printPrevv, tempCurr, ptr2, currPath, curf);
	else
	{
		store = execCmd(eqn[0], &ign1, &ign2, homie, ptr, prevvPath,
				printPrevv, tempCurr, ptr2, currPath, curf);
	}

	for(int i=1;i<sz;i+=2)
	{
		if((store == 1 && eqn[i][0] == '$') || (store == 0 && eqn[i][0] == '@'))
		{
			//printf("%d %c\n",store, eqn[i][0]);
			continue;
		}

		if(i == sz-1)
		{
			perror("Invalid command");
			return 0;
		}
		
		//printf("ikkada\n");

		if(isPiping(eqn[i+1]))
			ret = doPiping(eqn[i+1], &ign1, &ign2, homie, ptr, prevvPath,
					printPrevv, tempCurr, ptr2, currPath, curf);
		else if(isRedirection(eqn[i+1]))
			ret = redirection(eqn[i+1], &ign1, &ign2, homie, ptr, prevvPath,
					printPrevv, tempCurr, ptr2, currPath, curf);
		else
		{
			ret = execCmd(eqn[i+1], &ign1, &ign2, homie, ptr, prevvPath,
					printPrevv, tempCurr, ptr2, currPath, curf);
		}

		//printf("akkada\n");

		if(eqn[i][0] == '$')
			store = (store | ret);
		else if(eqn[i][0] == '@')
			store = (store & ret);
		else
		{
			perror("Invalid command");
			return 0;
		}

		//printf("ret = %d and store = %d\n", ret, store);
	}

	return store;
}

int setEquation(char* command)
{
	int leng = strlen(command);
	int curr = 0, ctr = 0;

	for(int i=0;i<130;i++)
	{
		eqn[i] = (char *)malloc(1030 * sizeof(char));
	}

	for(int i=0;i<leng;i++)
	{
		if(command[i] == '$' || command[i] == '@')
		{
			eqn[curr][ctr] = '\0';
			curr++;
			ctr = 0;
			eqn[curr][0] = command[i];
			eqn[curr][1] = '\0';
			curr++;
			continue;
		}

		eqn[curr][ctr++] = command[i];
	}

	eqn[curr++][ctr] = '\0';
	return curr;
}
