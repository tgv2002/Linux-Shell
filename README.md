# LINUX SHELL

## Compiling and running
1. A makefile has been included in the zip file for compiling the code. Just run the command 'make', and it would compile all the involved files appropriately.
2. Run the command './a.out', to begin the execution of shell. (NOTE - Don't use single quotes while running the command, Thank you.).

# ASSIGNMENT-2:

## Header files
1. Important header files are defined in the file 'headerFiles.h'.

## Main
1. The file 'main.c' contains the main infinite loop, that controls the functioning of the shell. It takes the input entered and makes the appropriate function call to execute the command. 

## Prompt
1. The file 'prompt.c' contains function which is used to obtain the user name and host name. It prints them to the screen (stdout or stderr) with appropriate colors.
2. The file 'prompt.h' contains the function definitions of functions used in 'prompt.c', and is included in 'main.c'.

## cd Command
1. The file 'cdCommand.c' contains function that implements the basic functionality (along with '.','..','~') of command 'cd' in linux.
2. This function treats the directory in which the executable resides to be the home directory. It alters the prompt appropriately when change of directory occurs.
3. The file 'cdCommand.h' contains the function definitions of functions used in 'cdCommand.c', and is included in 'main.c'.

## pwd Command
1. The file 'pwdCommand.c' contains function that extracts the name of the current directory in which we are present, and it prints the name of it to the console.
2. The file 'pwdCommand.h' contains the function definitions of functions used in 'pwdCommand.c', and is included in 'main.c'.

## echo Command 
1. The file 'echoCommand.c' contains function that prints the string provided as input, to stdout. It won't treat single/double quotes and backslashes specially.
2. The file 'echoCommand.h' contains the function definitions of functions used in 'echoCommand.c', and is included in 'main.c'.

## ls Command
1. The file 'lsCommand.c' contains functions that implements basic functionality of (along with required flags '.','..','~','-a','-l','-al/la') of command 'ls' in linux.
2. The flags like '-laa','-allalalaa' have been handled so as to mimic the actual shell. Multiple directories and flags can be passed in any order.
3. The file 'lsCommand.h' contains the function definitions of functions used in 'lsCommand.c', and is included in 'main.c'.

## foreground and background process handling
1. The file 'fgbgProcess.c' contains functions and signal handlers, that separate foreground and background processes and handle them appropriately, using asynchronous functions wherever needed.
2. The file 'bgTerminated.c' contains a linked-list implementation of structures that associate the PID of a process and its command. An instance of this structure is inserted into the linked list whenever the appropriate command is encountered/ process is made. It is used to know the name of process that terminated in background. All of this is done in the functions defined in this file.
3. '&&' is assumed to be a single '&' in this shell.
4. The file 'fgbgProcess.h' contains the function definitions of functions used in 'fgbgProcess.c', and is included in 'main.c'.
5. The file 'bgTerminated.h' contains the function definitions of functions used in 'bgTerminated.c', and is included in 'main.c'.

## pinfo Command
1. The file 'pinfoCommand.c' contains functions that describe the information of a process (PID, status, virtual memory size, executable path), whose PID has been passed as an argument.
2. It can take multiple arguments. If no argument is passed, then by default it considers the process ID of the execution of shell.
3. If an invalid PID is encountered in the arguments passed, it immediately terminates execution and prints the error.
4. The file 'pinfoCommand.h' contains the function definitions of functions used in 'pinfoCommand.c', and is included in 'main.c'.


## BONUS -

## nightswatch command
1. The file 'nightswatch.c' contains functions that print the information about interrupts of keyboard controller for each thread / PID of most recently created process in system, based on the command provided.
2. It executes the above functionality periodically for number of seconds that have been mentioned along with '-n' flag. If no such argument is mentioned, irrespective of presence of '-n' flag, a default time period of 2 seconds is considered, provided command entered is either 'interrupt'/'newborn'.
3. Execution stops when 'q' is pressed (need not hit ENTER after that).
4. Flags and command name and arguments can be passed in any order.
5. The file 'nightswatch.h' contains the function definitions of functions used in 'nightswatch.c', and is included in 'main.c'.

## history command
1. When the shell is run, at the beginning itself (before infinite loop), a file 'historyFile.txt' is created (if it doesn't exist), in the same directory in which the executable resides. This is meant to contain the information regarding history of commands entered. If 'historyFile.txt' is not empty, it is assumed to contain valid data from previous sessions.
2. The file 'historyFunc.c' contains functions that write the entered commands to this file, ensuring that it is always constrained to 20 commands; and also print the number of commands passed in argument. It prints all the commands in the file if no parameter is mentioned.
3. A command isn't considered if it begins with a space/tab/newline. It is also disregarded when it exactly matches the previous command entered.
4. The file 'historyFunc.h' contains the function definitions of functions used in 'historyFunc.c', and is included in 'main.c'.

NOTE - All of the commands mentioned in the assignment PDF, adhere to those restrictions (hopefully).

# ASSIGNMENT-3:

## I/O redirection
1. The file 'redirection.c' contains functions that handles both input and output redirection appropriately.
2. It consists of two functions to execute redirection, and another function to execute commands whose input/output is redirected.
3. An assumption made here is that commands having the same symbol consecutively (even if separated by a file/command in between), such as 'cat 1.txt > out1.txt > out2.txt', is treated as an error in this case.
4. The file 'redirection.h' contains the function definitions of functions used in 'redirection.c', and is included in 'main.c'.

## Piping
1. The file 'pipeIt.c' contains a function that handles piping appropriately.
2. When redirection is also involved in the commands that are being processed while piping, the corresponding functions in the file 'redirection.c' are called for execution, which executes the command appropriately, and piping continues normally after the call returns.
3. The file 'pipeIt.h' contains the function definition of function used in 'pipeIt.c', and is included in 'main.c'.

## Environment variables
1. The file 'envVars.c' contains functions which are used to set or get values of environment variables from shell's parent process, in accordance to the constraints as mentioned in the pdf.
2. The file 'envVars.h' contains the function definition of function used in 'envVars.c', and is included in 'main.c'.

## Job controlling
1. The file 'jobControl.c' contains individual functions which are used to execute the user-defined commands that control execution of processes - jobs, kjob, fg, bg. These are executed in accordance with the constraints imposed in the pdf.
2. The file 'jobControl.h' contains the function definition of function used in 'jobControl.c', and is included in 'main.c'.

## Overkill and quit
1. The file 'overkill.c' contains a function that terminates all the currently running background processes when entered.
2. The file 'overkill.h' contains the function definition of function used in 'overkill.h', and is included in 'main.c'.
3. The quit command is implemented in 'main.c', and the shell also quits on pressing CTRL+D, which has been implemented in 'main.c' too.

## Signal Handling
1. Signal handlers to function accordingly are defined in main.c, but most of the update functions are called on checking the exit status information of a process.

## Additional helper functions in files
1. The file 'checkers.c' contains functions which validate the entered command, to simplify calling the required function for that command execution. The file 'checkers.h' contains definitions of functions defined here, and is included in 'main.c' and wherever required.
2. The file 'executeCmd.c' contains function that facilitates the execution of external commands that can be called from any possible part in the code. The file 'executeCmd.h' contains definitions of functions defined here, and is included in 'main.c' and wherever required.
3. Inbuilt functions defined in files for assignment-2 are used, whenever an inbuilt command is entered.

## BONUS - 

## Last working directory
1. The appropriate additions have been made to the code in 'cdCommand.c', and required variables are passed to store up-to-date information.

## Exit codes
1. These have been handled appropriately with the help of return values of the functions called, and they are in accordance with the constraints imposed in the pdf.

## Command chaining with AND and OR
1. The file 'computeLogic.c' contains functions which handle command chaining as required, in accordance with details of pdf.
2. The file 'computeLogic.h' contains the function definition of functions used in 'computeLogic.c', and is included in 'main.c'.


