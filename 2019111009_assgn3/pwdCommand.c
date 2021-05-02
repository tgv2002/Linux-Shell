#include "pwdCommand.h"
#include "headerFiles.h"

char currDir[4096];

int pwdCommand()
{
   if(getcwd(currDir, sizeof(currDir)) != NULL)
   {
       printf("%s\n", currDir);
   }

   else
   {
       perror("Current directory error");
       return 0;
   }

   return 1;
}
