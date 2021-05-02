# include "headerFiles.h"
# include "bgTerminated.h"
# include "overkill.h"

int staty, PID;
char storePath[1030];

int overkill(char* command, int* ctrc, int* ctrz, int* curf)
{
	int lengy = strlen(command);
	int proc_ID, suc = 1;

	for(int i=0;i<lengy;i++)
	{
		if(command[i] == ' ' || command[i] == '\t' || command[i] == '\n')
			continue;
		perror("Invalid command");
		return 0;
	}

	int *p = display_bg();
	int ctr = 0, curr_pid, cid;
	char* storePID = (char *)malloc(7 * sizeof(char));
	char* killThis[4];
	for(int i=0;i<4;i++)
		killThis[i] = (char*)malloc(7 * sizeof(char));

	while(*(p + ctr) != -1)
	{
		curr_pid = *(p + ctr);
		kill((pid_t)curr_pid, 9);
		ctr++;
	}

	return suc;
}
