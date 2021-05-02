# include "headerFiles.h"
# include "bgTerminated.h"
# include "jobControl.h"

char value[1030];
char cFile[1030];
char readContent[100001];

int jobsCommand(char *command, int* ctrc, int* ctrz, int* curf)
{
	int lengy = strlen(command);

	for(int i=0;i<lengy;i++)
	{
		if(command[i] == ' ' || command[i] == '\t' || command[i] == '\n')
			continue;
		perror("Invalid command");
		return 0;
	}

	int *p = display_bg();
	char **nam = display_names();
	int *posy = display_states();
	char seeState[9];
	int ctr = 0, curr_pid, curr_state;
	char* curr_name = (char *)malloc(1030 * sizeof(char));

	while(*(p + ctr) != -1)
	{
		curr_pid = *(p + ctr);
		curr_name = *(nam + ctr);
		curr_state = *(posy + ctr);

		sprintf(cFile, "/proc/%d/stat", curr_pid);
		FILE* fp = fopen(cFile, "r");
		char stt;
		int isRun = 1;
		int ct = 0;

		while(fscanf(fp, "%s", readContent) == 1)
		{
			ct++;

			if(ct == 3)
			{
				stt = readContent[0];
				break;
			}
		}

		if(stt == 'Z')
			isRun = 2;
		else if(stt == 'T')
			isRun = 0;

		if(isRun == 1)
			strcpy(seeState, "Running");
		else if(isRun == 2)
			strcpy(seeState, "Zombie");
		else
		{
			strcpy(seeState, "Stopped");	
		}

		printf("[%d] %s %s [%d]\n", ctr+1, seeState,
				curr_name, curr_pid);
		ctr++;
	}

	return 1;
}

int bgCommand(char *command, int* ctrc, int* ctrz, int* curf)
{
	int lengy = strlen(command);
	int proc_ID, dig, jobNum = 0, curr_pid, cid, visn = 0;
	int suc = 1;

	for(int i=0;i<lengy;i++)
	{
		if(command[i] == ' ' || command[i] == '\t' || command[i] == '\n')
		{
			if(visn == 0)
				continue;
			perror("Invalid arguments");
			return 0;
		}

		if(command[i] >= '0' && command[i] <= '9')
		{
			dig = (int)(command[i] - '0');
			jobNum = ((jobNum*10) + dig);
			visn = 1;
			continue;
		}

		perror("Invalid command");
		return 0;
	}

	if(jobNum == 0)
	{
		perror("Invalid job ID");
		return 0;
	}

	jobNum--;
	int ctr = 0, got = 0;
	int *p = display_bg();

	while(*(p + ctr) != -1)
	{
		if(ctr == jobNum)
		{
			curr_pid = *(p + ctr);
			got = 1;
			break;
		}

		ctr++;
	}

	if(got == 0)
	{
		perror("Invalid process");
		return 0;
	}

	suc = kill((pid_t)curr_pid, 18);
	sprintf(cFile, "/proc/%d/stat", curr_pid);
	FILE* fp = fopen(cFile, "r");
	char stt;
	int isRun = 1;
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
	setRunningStatus(curr_pid, isRun);
	return (1 + suc);
}

int kjobCommand(char* command, int* ctrc, int* ctrz, int* curf)
{
	int ctr = 0, leng, jobNum = 0, sigNum = 0, dig;
	strcpy(value, command);	
	char *tokenized = (char*)malloc(1030 * sizeof(char));
	tokenized = strtok(value, " \t\n");
	int suc = 1;

	while (tokenized != NULL)
	{
		ctr++;
		leng = strlen(tokenized);

		if(ctr == 1)
		{
			for(int i=0;i<leng;i++)
			{
				dig = (int)(tokenized[i] - '0');

				if(dig >= 0 && dig <= 9)
				{
					jobNum = ((jobNum * 10) + dig);
					continue;
				}

				perror("Invalid job number");
				return 0;
			}
		}

		else if(ctr == 2)
		{
			for(int i=0;i<leng;i++)
			{
				dig = (int)(tokenized[i] - '0');

				if(dig >= 0 && dig <= 9)
				{
					sigNum = ((sigNum * 10) + dig);
					continue;
				}

				perror("Invalid job number");
				return 0;
			}
		}

		else
		{
			perror("Invalid arguments");
			return 0;
		}

		tokenized = strtok(NULL, " \t\n");
	}

	if(ctr != 2 || jobNum == 0)
	{
		perror("Invalid arguments");
		return 0;
	}

	jobNum--;
	int curr_pid, cid, got = 0;
	ctr = 0;
	int *p = display_bg();

	while(*(p + ctr) != -1)
	{
		if(ctr == jobNum)
		{
			curr_pid = *(p + ctr);
			got = 1;
			break;
		}

		ctr++;
	}

	if(got == 0)
	{
		perror("Invalid process");
		return 0;
	}

	suc = kill((pid_t)curr_pid, sigNum);

	sprintf(cFile, "/proc/%d/stat", curr_pid);
	FILE* fp = fopen(cFile, "r");
	char stt;
	int isRun = 1;
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
	setRunningStatus(curr_pid, isRun);

	return (1 + suc);
}

int fgCommand(char* command, int* ctrc, int* ctrz, int* curf)
{
	int lengy = strlen(command);
	int proc_ID, suc = 1, sucy = 1, visn = 0, dig = 0, jobNum = 0;
	int statVar;

	for(int i=0;i<lengy;i++)
	{
		if(command[i] == ' ' || command[i] == '\t' || command[i] == '\n')
		{
			if(visn == 0)
				continue;
			perror("Invalid arguments");
			return 0;
		}

		if(command[i] >= '0' && command[i] <= '9')
		{
			dig = (int)(command[i] - '0');
			jobNum = ((jobNum*10) + dig);
			visn = 1;
			continue;
		}

		perror("Invalid command");
		return 0;
	}

	if(jobNum == 0)
	{
		perror("Invalid job ID");
		return 0;
	}

	jobNum--;
	int ctr = 0, got = 0, curr_pid, zz = 0;
	int *p = display_bg();

	while(*(p + ctr) != -1)
	{
		if(ctr == jobNum)
		{
			curr_pid = *(p + ctr);
			got = 1;
			break;
		}

		ctr++;
	}

	if(got == 0)
	{
		perror("Invalid job number");
		return 0;
	}

	*curf = curr_pid;

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

	if(tcsetpgrp(STDIN_FILENO, curr_pid) < 0)
	{

	}

	kill((pid_t)curr_pid, 18);
	int prop = waitpid(curr_pid, &statVar, WUNTRACED);

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
		zz = 1;
		sucy = 0;
	}

	else if(WIFSIGNALED(statVar) || WIFEXITED(statVar))
	{
		if(WIFSIGNALED(statVar))
			sucy = 0;
		search_node(curr_pid);
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

	if(zz == 1)
	{
		sprintf(cFile, "/proc/%d/stat", curr_pid);
		FILE* fp = fopen(cFile, "r");
		char stt;
		int isRun = 1;
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
		setRunningStatus(curr_pid, isRun);
		fprintf(stderr, "\n[%d]	Stopped			%s\n", jobNum + 1, returnName(curr_pid));
	}

	*curf = -1;
	return sucy;
}
