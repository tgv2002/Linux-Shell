#ifndef __CHECKERS_H
#define __CHECKERS_H

int firstNonSpace(char* command);
int isRedirection(char *command);
int isPiping(char* command);
int isSetEnv(char* command);
int isUnSetEnv(char* command);
int isOverKill(char* command);
int isJobs(char* command);
int isFgCommand(char* command);
int isBgCommand(char* command);
int isKjobCommand(char* command);
int isLs(char* command);
int isCd(char* command);
int isEcho(char* command);
int isHistory(char* command);
int isNightswatch(char* command);
int isPinfo(char* command);
int isPwd(char* command);
int isLogical(char* command);

#endif
