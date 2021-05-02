#include "headerFiles.h"
#include "bgTerminated.h"
#include "prompt.h"

struct node
{
	int pid;
	char *cmdInfo;
	struct node *next;
	int isBg;
	int isRunning;

}*start=NULL,*q,*curNode;

int leng;
char currCommand[100001];
char outy[100001];
int pids[1030];
int states[1030];
char *names[130];

void initt()
{
	start = (struct node*)malloc(sizeof(struct node));
	start->pid = 0;
	start->next = NULL;
	start->cmdInfo = NULL;
	start->isBg = 0;
	start->isRunning = 0;

	for(int i=0;i<130;i++)
		names[i] = (char*) malloc(1030 * sizeof(char));
}

int* display_bg()
{
	int ctr = 0;

	if(start==NULL)
	{
		perror("No background processes");
		return NULL;
	}

	else
	{
		q=start;

		while(q!=NULL)
		{
			if(q->isBg == 1)
				pids[ctr++] = q->pid;
			q=q->next;
		}
	}

	pids[ctr] = -1;
	return pids;
}

int getJobNo()
{
	int ctr = 0;

	if(start==NULL)
	{
		perror("No background processes");
		return 0;
	}

	else
	{
		q=start;

		while(q!=NULL)
		{
			if(q->isBg == 1)
				ctr++;
			q=q->next;
		}
	}

	return ctr;
}

void print_bg()
{
	int ctr = 0;

	if(start==NULL)
	{
		perror("No background processes");
		return;
	}

	else
	{
		q=start;

		while(q!=NULL)
		{
			if(1 == 1)
				fprintf(stderr, "pid=%d isRunning=%d isBg=%d\n ", q->pid, q->isRunning, q->isBg);
			q=q->next;
		}
	}
}

int* display_states()
{
	int ctr = 0;

	if(start==NULL)
	{
		perror("No background processes");
		return NULL;
	}

	else
	{
		q=start;

		while(q!=NULL)
		{
			if(q->isBg == 1)
				states[ctr++] = q->isRunning;
			q=q->next;
		}
	}

	states[ctr] = -1;
	return states;
}

char** display_names()
{
	int ctr = 0;

	if(start==NULL)
	{
		perror("No background processes");
		return NULL;
	}

	else
	{
		q=start;

		while(q!=NULL)
		{
			if(q->isBg == 1)
				names[ctr++] = q->cmdInfo;
			q=q->next;
		}
	}

	names[ctr] = NULL;
	return names;
}

void insert_node(int pid, char* cmd, int isBg, int isRunning)
{
	curNode = (struct node*)malloc(sizeof(struct node));
	curNode->pid = pid;
	curNode->next = NULL;
	curNode->cmdInfo = (char*)malloc(1030);
	curNode->isBg = isBg;
	curNode->isRunning = isRunning;

	leng = strlen(cmd);
	for(int i=0;i<leng;i++)
		curNode->cmdInfo[i] = cmd[i];

	if(start == NULL)
	{
		start = curNode;
	}

	else
	{
		q = start;
		while(q->next != NULL)
			q= q->next;
		q->next = curNode;
	}
}

char* delete_node(int pos)
{
	if(start == NULL)
	{
		//perror("No processes running");
		return NULL;
	}

	q=start;

	for(int i=1;i<pos-1;i++)
	{
		if(q->next==NULL)
		{
			//perror("Process not found to delete");
			return NULL;
		}

		q=q->next;
	}

	curNode = q->next;
	q->next = curNode->next;
	leng = strlen(curNode->cmdInfo);
	for(int i=0;i<leng;i++)
		currCommand[i] = curNode->cmdInfo[i];
	currCommand[leng] = '\0';
	free(curNode);
	return currCommand;
}

char* search_node(int pid)
{
	if(start == NULL)
	{
		//perror("No process running");
		return NULL;
	}

	else
	{
		q=start;
		int ctr = 1;

		while(q!=NULL)
		{
			if(q->pid == pid)
			{
				return (delete_node(ctr));
			}

			q=q->next;
			ctr++;
		}

		//perror("Process not found");
		return NULL;
	}
}

char* returnName(int pid)
{
	if(start == NULL)
	{
		perror("No process running");
		return NULL;
	}

	else
	{
		q=start;
		int ctr = 1;

		while(q!=NULL)
		{
			if(q->pid == pid)
			{
				return (q->cmdInfo);
			}

			q=q->next;
			ctr++;
		}

		perror("Process not found");
		return NULL;
	}
}

void setBgStatus(int pid, int isBg)
{
	if(start == NULL)
	{
		perror("No process running");
		return;
	}

	else
	{
		q=start;
		int ctr = 1;

		while(q!=NULL)
		{
			if(q->pid == pid)
			{
				//fprintf(stderr, "bg changed\n");
				q->isBg = isBg;
				return;
			}

			q=q->next;
			ctr++;
		}

		perror("Process not found");
	}
}

void setRunningStatus(int pid, int isRunning)
{
	if(start == NULL)
	{
		perror("No process running");
		return;
	}

	else
	{
		q=start;
		int ctr = 1;

		while(q!=NULL)
		{
			if(q->pid == pid)
			{
				//fprintf(stderr, "run changed\n");
				q->isRunning = isRunning;
				return;
			}

			q=q->next;
			ctr++;
		}

		perror("Process not found");
	}
}

void bgTerminated(int pid, char currPath[100001])
{
	fflush(stderr);fflush(stdin);fflush(stdout);
	fprintf(stderr, "\n%s process with %d pid exited normally\n", search_node(pid), pid);
	prompt(currPath, 1, 1);
}

