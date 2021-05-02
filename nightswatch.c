#include "headerFiles.h"
#include "nightswatch.h"
#include <termios.h>

char cur[100005];
clock_t startNow;

int qKeyPress(void)
{
	struct termios oldt, newt;
	int bytes;

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	ioctl(STDIN_FILENO, FIONREAD, &bytes);
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

	if(bytes > 0)
	{
		if(getchar()=='q')
		{
			return 1;
		}
	}

	return 0;
}

int nightsWatch(char* cmd)
{
	int suc = 1;
	int leng, dig, num = 0, readNum = 0, ctr = 0, col = -1;
	int nFlag = 0;
	int interr = -1;
	int vis = 0;
	FILE *fp;
	char ch;
	char* tokenized = (char *)malloc(1030 * sizeof(char));
	tokenized = strtok(cmd, " \t"); 

	while (tokenized != NULL) 
	{ 
		leng = strlen(tokenized);

		if(leng == 2 && tokenized[0] == '-' && tokenized[1] == 'n')
			nFlag = 1;
		else if(strcmp(tokenized, "interrupt") == 0)
			interr = 1;
		else if(strcmp(tokenized, "newborn") == 0)
			interr = 0;
		else
		{
			for(int i=0;i<leng;i++)
			{
				dig = (int)(tokenized[i] - '0');

				if(dig >= 0 && dig <= 9 && readNum == 0 && nFlag == 1)
					num = (num*10 + dig);
				else
				{
					perror("Invalid command");
					return 0;
				}                
			}
			
			readNum = 1;
		}

		vis = 1;
		tokenized = strtok(NULL, " \t"); 
	}

	if(readNum == 1 && num >= 2)
		num--;

	if(interr == -1 || vis == 0)
	{
		perror("Invalid arguments");
		return 0;
	}

	if(readNum == 0)
		num = 2;

	while(1)
	{
		fflush(NULL);

		if(qKeyPress())
		{
			break;
		}

		if(interr == 1)
		{
			fp = fopen("/proc/interrupts", "r");

			if(fp == NULL)
			{
				perror("Keyboard Interrupts");
				suc = 0;
				continue;
			}

			ctr = 0;

			while(fscanf(fp, "%[^\n]\n", cur) == 1)
			{
				ctr++;

				if(ctr == 1)
				{
					printf("%s\n", cur);
				}

				else
				{
					leng = strlen(cur);
					col = 1;

					for(int i=0;i<leng;i++)
					{
						if(cur[i] == ':')
						{
							col = i;
							break;
						}
					}

					if(cur[col-1] != '1')
						continue;

					printf("   ");

					for(int i=0;i<leng;i++)
					{
						if(i <= col)
						{   
							if(cur[i] == ' ' || cur[i] == '\t')
								printf("%c", cur[i]);
							else
							{
								printf(" ");
							}

							continue;
						}

						if((cur[i] >= '0' && cur[i] <= '9') ||
								cur[i] == '\t' || cur[i] == ' ')
							printf("%c", cur[i]);
						else
						{
							break;
						}
					}

					printf("\n");
					break;
				}
			}

			fclose(fp);
		}

		else
		{
			fp = fopen("/proc/loadavg", "r");

			if(fp == NULL)
			{
				perror("PID of recent process");
				suc = 0;
				continue;
			}

			ctr = 0;

			while(fscanf(fp, "%s", cur) == 1)
			{
				ctr++;

				if(ctr == 5)
				{
					printf("%s\n", cur);
					break;
				}
			}

			fclose(fp);
		}

		startNow = clock();

		while(clock() < startNow + (num * CLOCKS_PER_SEC))
		{
			if(qKeyPress())
				return suc;
		}
	}

	return suc;
}
