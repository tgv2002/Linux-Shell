#include "lsCommand.h"
#include "headerFiles.h"

char reqPath[100001];
char currFile[100001];
char *fileName;
char curr_dir[100001];
char storeTime[200];
struct stat f;
int sl = -1, path_l, c = 0;
char file_name[1000001];
struct passwd *file_own;
struct group  *group_own;
int suc = 1;

void permLgiven(char* pathname, int a)
{
	if(stat(pathname, &f) < 0)
	{
		perror(pathname);
		suc = 0;
		return;
	}

	if(S_ISREG(f.st_mode))
	{
		sl = -1;
		c = 0;
		path_l = strlen(pathname);

		for(int i=0;i<path_l;i++)
		{
			if(pathname[i] == '/')
				sl = i;
		}

		for(int i=sl+1;i<path_l;i++)
			file_name[c++] = pathname[i];
		file_name[c] = '\0';

		if(a == 0 && file_name[0] == '.')
			return;

		printf((S_ISDIR(f.st_mode)) ? "d" : "-");
		printf((f.st_mode & S_IRUSR) ? "r" : "-");
		printf((f.st_mode & S_IWUSR) ? "w" : "-");
		printf((f.st_mode & S_IXUSR) ? "x" : "-");
		printf((f.st_mode & S_IRGRP) ? "r" : "-");
		printf((f.st_mode & S_IWGRP) ? "w" : "-");
		printf((f.st_mode & S_IXGRP) ? "x" : "-");
		printf((f.st_mode & S_IROTH) ? "r" : "-");
		printf((f.st_mode & S_IWOTH) ? "w" : "-");
		printf((f.st_mode & S_IXOTH) ? "x " : "- ");

		printf("%5d ", (int)f.st_nlink);

		file_own = getpwuid(f.st_uid);
		group_own = getgrgid(f.st_gid);

		if(file_own == NULL || group_own == NULL)
		{
			if(file_own == NULL)
				perror("File owner");
			if(group_own == NULL)
				perror("Group Owner");
			suc = 0;
		}

		else
		{
			printf("%20s %20s %11d ",file_own->pw_name, group_own->gr_name,
					(int)f.st_size);
			strftime(storeTime, sizeof(storeTime), "%b %e %H:%M",
					localtime(&(f.st_mtime)));
			printf("%20s ", storeTime);
			printf("%20s\n", file_name);
		}

		return;
	}

	DIR *d = opendir(pathname);

	if(d == NULL)
	{
		if(getcwd(curr_dir, sizeof(curr_dir)) != NULL)
		{
			d = opendir(curr_dir);

			if(d == NULL)
			{
				perror("Directory access");
				suc = 0;
				return;
			}
		}

		else
		{
			perror("Current Directory access");
			suc = 0;
			return;
		}
	}

	struct dirent * dir;

	while((dir = readdir(d)) != NULL)
	{
		fileName = dir->d_name;
		c = 0;
		path_l = strlen(pathname);
		for(int i=0;i<path_l;i++)
			currFile[c++] = pathname[i];
		currFile[c++] = '/';
		sl = strlen(fileName);
		for(int i=0;i<sl;i++)
			currFile[c++] = fileName[i];
		currFile[c] = '\0';

		if(a == 0 && fileName[0] == '.')
			continue;

		if(stat(currFile, &f) < 0)    
		{
			perror("File access");
			suc = 0;
			continue;
		}

		printf((S_ISDIR(f.st_mode)) ? "d" : "-");
		printf((f.st_mode & S_IRUSR) ? "r" : "-");
		printf((f.st_mode & S_IWUSR) ? "w" : "-");
		printf((f.st_mode & S_IXUSR) ? "x" : "-");
		printf((f.st_mode & S_IRGRP) ? "r" : "-");
		printf((f.st_mode & S_IWGRP) ? "w" : "-");
		printf((f.st_mode & S_IXGRP) ? "x" : "-");
		printf((f.st_mode & S_IROTH) ? "r" : "-");
		printf((f.st_mode & S_IWOTH) ? "w" : "-");
		printf((f.st_mode & S_IXOTH) ? "x " : "- ");

		printf("%5d ", (int)f.st_nlink);

		file_own = getpwuid(f.st_uid);
		group_own = getgrgid(f.st_gid);

		if(file_own == NULL || group_own == NULL)
		{
			if(file_own == NULL)
				perror("File owner");
			if(group_own == NULL)
				perror("Group Owner");
			suc = 0;
			continue;
		}

		else
		{
			printf("%20s %20s %11d ",file_own->pw_name, group_own->gr_name,
					(int)f.st_size);
			strftime(storeTime, sizeof(storeTime), "%b %e %H:%M",
					localtime(&(f.st_mtime)));
			printf("%20s ", storeTime);
			printf("%20s\n", fileName);
		}
	}

	closedir(d);
}

void permNoL(char* pathname, int a)
{
	if(stat(pathname, &f) < 0)
	{
		perror(pathname);
		suc = 0;
		return;
	}

	if(S_ISREG(f.st_mode))
	{
		sl = -1;
		c = 0;
		path_l = strlen(pathname);

		for(int i=0;i<path_l;i++)
		{
			if(pathname[i] == '/')
				sl = i;
		}

		for(int i=sl+1;i<path_l;i++)
			file_name[c++] = pathname[i];
		file_name[c] = '\0';

		if(a == 0 && file_name[0] == '.')
			return;
		printf("%s\n",file_name);
		return;
	}

	DIR* d = opendir(pathname);

	if(d == NULL)
	{
		if(getcwd(curr_dir, sizeof(curr_dir)) != NULL)
		{
			d = opendir(curr_dir);

			if(d == NULL)
			{
				perror("Directory access");
				suc = 0;
				return;
			}
		}

		else
		{
			perror("Current Directory access");
			suc = 0;
			return;
		}
	}

	struct dirent * dir;

	while((dir = readdir(d)) != NULL)
	{
		fileName = dir->d_name;
		c = 0;
		path_l = strlen(pathname);
		for(int i=0;i<path_l;i++)
			currFile[c++] = pathname[i];
		currFile[c++] = '/';
		sl = strlen(fileName);
		for(int i=0;i<sl;i++)
			currFile[c++] = fileName[i];
		currFile[c] = '\0';

		if(a == 0 && fileName[0] == '.')
			continue;
		printf("%s\n", fileName); 
	}

	closedir(d);
}

int lsCommand(char* valuee, char* home)
{
	char* value2 = (char *) malloc(1030 * sizeof(char));
	char* value = (char *) malloc(1030 * sizeof(char));
	char* tokenized = (char *) malloc(1030* sizeof(char));
	char* tokenized2 = (char *) malloc(1030* sizeof(char));
	fileName = (char *) malloc(1030* sizeof(char));
	strcpy(value, valuee);
	strcpy(value2, value);
	tokenized = strtok(value, " \t");

	int l = 0;
	int a = 0;
	int al = 0; 
	bool vis = false;
	int leng;
	int ctr = 0, req_leng;
	int home_l = strlen(home);

	while(tokenized != NULL) 
	{ 
		leng = strlen(tokenized);

		if(leng >= 2 && tokenized[0] == '-')
		{	
			for(int j=1;j<leng;j++)
			{
				if(tokenized[j] == 'a')
					a = 1;
				else if(tokenized[j] == 'l')
					l = 1;
				else
				{
					perror("Invalid flags");
					suc = 0;
					return 0;
				}
			}
		} 

		tokenized = strtok(NULL, " \t"); 
	}

	al = (a==1 && l==1) ? 1 : 0;
	tokenized2 = strtok(value2, " \t");

	while(tokenized2 != NULL)
	{
		leng = strlen(tokenized2);

		if(leng >= 1 && tokenized2[0] != '-')
		{
			//printf("%s\n", tokenized2);

			if(!vis)
				vis = true;

			req_leng = strlen(tokenized2);
			ctr = 0;

			for(int i=0;i<req_leng;i++)
			{
				if(tokenized2[i] != '~')
					reqPath[ctr++] = tokenized2[i];
				else
				{
					for(int j=0;j<home_l;j++)
						reqPath[ctr++] = home[j];
				}
			}

			reqPath[ctr] = '\0';

			if(al == 1)
				permLgiven(reqPath, 1);
			else
			{
				if(l == 1)
					permLgiven(reqPath, a);
				else
				{
					permNoL(reqPath, a);
				}
			}   
		}

		tokenized2 = strtok(NULL, " \t");
	}    

	if(!vis)
	{
		if(getcwd(curr_dir, sizeof(curr_dir)) != NULL)
		{
			if(al == 1)
				permLgiven(curr_dir, 1);
			else
			{
				if(l == 1)
					permLgiven(curr_dir, a);
				else
				{
					permNoL(curr_dir, a);
				}
			}      
		}

		else
		{
			perror("Current directory error");
			suc = 0;
		}
	}

	return suc;
}
