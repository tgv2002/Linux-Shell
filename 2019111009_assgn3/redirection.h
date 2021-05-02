#ifndef __REDIRECTION_H
#define __REDIRECTION_H

int redirection(char* command, int* ctrc, int* ctrz, char* homie, int* ptr, char* prevvPath,
		char* printPrevv, char* tempCurr, int* ptr2, char currPath[100001], int* curf);
int execCmd(char* command, int* ctrc, int* ctrz, char* homie, int* ptr, char* prevvPath,
		char* printPrevv, char* tempCurr, int* ptr2, char currPath[100001], int* curf);
int doRedirection(int leftL, int leftR, int rightL,
                    int rightR, char* doThis,
                    int* ctrc, int* ctrz, char* homie, int* ptr, char* prevvPath,
		char* printPrevv, char* tempCurr, int* ptr2, char currPath[100001],
		 char* arg2, char* sec, int* curf);
int multips(int idx, char* checkHere);
int rightEnd(int idx, int ctr);

#endif
