#ifndef __BGTERMINATED_H
#define __BGTERMINATED_H

void initt();
void bgTerminated(int pid, char currPath[100001]);
void insert_node(int pid, char* cmd, int isBg, int isRunning);
char* search_node(int pid);
char* delete_node(int pos);
int* display_bg();
void print_bg();
int* display_states();
char** display_names();
void setBgStatus(int pid, int isBg);
void setRunningStatus(int pid, int isRunning);
int getJobNo();
char* returnName(int pid);

#endif
