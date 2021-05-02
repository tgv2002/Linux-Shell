#ifndef __JOBCONTROL_H
#define __JOBCONTROL_H

int jobsCommand(char *command, int* ctrc, int* ctrz, int* curf);
int fgCommand(char *command, int* ctrc, int* ctrz, int* curf);
int bgCommand(char* command, int* ctrc, int* ctrz, int* curf);
int kjobCommand(char* command, int* ctrc, int* ctrz, int* curf);

#endif
