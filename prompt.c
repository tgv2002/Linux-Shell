#include "prompt.h"
#include "headerFiles.h"
#define YELLOW  "\x1B[33m"
#define GREEN "\x1b[32m"
#define RES "\x1B[0m"

void prompt(char currPath[100001], int norm, int printTilda)
{
    char hostInfo[1024];
    char userName[1024];
    hostInfo[1023] = userName[1023] = '\0';
    int retHostName = gethostname(hostInfo, 1023);
    int retUserName = getlogin_r(userName, 1023);

    if(retHostName < 0)
    {
        perror("Host name");
        return;
    }

    if(retUserName < 0)
    {
        perror("User name");
        return;
    }
	
    if(norm == 0)
    {
	    if(printTilda == 0)
    	    	printf(YELLOW "%s@%s:"  RES GREEN "%s$ " RES, getenv("USER"), hostInfo, currPath);
	    else
		printf(YELLOW "%s@%s:"  RES GREEN "~%s$ " RES, getenv("USER"), hostInfo, currPath);
    }

    else
    {    
	    if(printTilda == 0)
		fprintf(stderr, YELLOW "%s@%s:"  RES GREEN "%s$ " RES, getenv("USER"), hostInfo, currPath);
	    else
		fprintf(stderr, YELLOW "%s@%s:"  RES GREEN "~%s$ " RES, getenv("USER"), hostInfo, currPath);
    }
}
