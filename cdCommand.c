#include "cdCommand.h"
#include "headerFiles.h"

char reqPath[1030];
char currDir[1030];
char* curry;
char* reqqy;

char* cdCommand(char *pathname, char home[4096], int* ptr, char* prevvPath,
		char* printPrevv, char* tempCurr, int* ptr2)
{
	int req_leng, home_l, curr_l;
	req_leng = strlen(pathname);
	home_l = strlen(home);
	int ctr = 0;
	int temp = 0;
	curry = (char *) malloc(1030 * sizeof(char));
	reqqy = (char *) malloc(1030 * sizeof(char));

	for(int i=req_leng-1;i>=0;i--)
	{
		if(pathname[i] != ' ' && pathname[i] != '\t')
			break;
		pathname[i] = '\0';
	}

	req_leng = strlen(pathname);

	if(req_leng >= 1 && pathname[0] == '-')
	{
		for(int i=1;i<req_leng;i++)
		{
			if(pathname[i] == '\t' || pathname[i] == ' ')
				continue;
			perror("Invalid command");
			return NULL;
		}
		
		ctr = 0;

		if(*ptr2 == 1)
		{
			printf("~");
			for(int j=0;j<home_l;j++)
				reqPath[ctr++] = home[j];
		}

		printf("%s\n", printPrevv);
		req_leng = strlen(printPrevv);

		for(int i=0;i<req_leng;i++)
                {
                        if(printPrevv[i] != '~')
                                reqPath[ctr++] = printPrevv[i];
                        else
                        {
                                for(int j=0;j<home_l;j++)
                                        reqPath[ctr++] = home[j];
                        }
                }

                reqPath[ctr] = '\0';
	}	

	else
	{
		ctr = 0;

		for(int i=0;i<req_leng;i++)
		{
			if(pathname[i] != '~')
				reqPath[ctr++] = pathname[i];
			else
			{
				for(int j=0;j<home_l;j++)
					reqPath[ctr++] = home[j];
			}
		}

		reqPath[ctr] = '\0';
	}
	
	strcpy(printPrevv, tempCurr);
	strcpy(reqqy, reqPath);
	//strcpy(prevvPath, reqqy);

	int pathChange = chdir(reqPath);

	if(pathChange < 0)
	{
		perror(reqPath);
		return NULL;
	}

	getcwd(currDir, sizeof(currDir));
	curr_l = strlen(currDir);
	*ptr2 = *ptr;

	if(curr_l < home_l)
	{
		*ptr = 0;
		strcpy(curry, currDir);
		return curry;
	}

	int farFromHome = 0;

	for(int i=0;i<home_l;i++)
		if(home[i] != currDir[i])
			farFromHome = 1;

	if(farFromHome == 1)
	{
		*ptr = 0;
		strcpy(curry, currDir);
		return (curry);
	}

	*ptr = 1;
	strcpy(curry, currDir + home_l);
	return (curry);

	return NULL;
}
