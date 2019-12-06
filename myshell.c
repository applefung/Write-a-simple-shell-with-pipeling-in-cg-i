/*
 * myshell.c
 *
 *  
 *      Author: FUNG, Ip Tsing
 *	Student ID: 20510788
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>


#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
// Assume that each command line has at most 256 characters (including NULL)
#define MAX_CMDLINE_LEN 256

// Assume that we have at most 16 pipe segments
#define MAX_PIPE_SEGMENTS 16

// Assume that each segment has at most 256 characters (including NULL)
#define MAX_SEGMENT_LENGTH 256

/*
  Function  Prototypes
 */
void show_prompt();
int get_cmd_line(char *cmdline);
void process_cmd(char *cmdline);
void **tokenize(char **argv, char *line, int *numTokens, char *token);
typedef void (*fun)(char *);
void exec(char* cmdline);
void piped(char *cmdline);

/* The main function implementation */
int main()
{
    char cmdline[MAX_CMDLINE_LEN];
    printf("The shell program (pid=%d) starts\n", getpid());
    while (1)
    {
        show_prompt();
        if (get_cmd_line(cmdline) == -1)
            continue; /* empty line handling */

        process_cmd(cmdline);
    }
    return 0;
}

void piped(char *cmdline)
{
   char *str1 = strtok(cmdline,"|");
   char *str2 = strtok(NULL,"|");
   char line[40];

   // use pipe implements the '|'
   FILE *in = popen(str1,"r");
   FILE *out = popen(str2,"w");

   if(in == NULL||out == NULL)
    {
       printf("popen error~!\n");
       return ;
    }

   //read the string and write to out
   while(fgets(line,40,in) != NULL)
       if(fputs(line,out) == EOF)
            printf("fputs error~!\n");

   pclose(in);
   pclose(out);
}

void exec(char* cmdline)
{
    char ** tmp = (char **)malloc(5*sizeof(char*));
    int num = 0;
    pid_t pid;

    //split the cmdline
    tmp[num] = strtok(cmdline," ");
    while(tmp[++num] = strtok(NULL," "))
     ;

    pid = fork();
    if(pid == 0)
    {
        //execute it
       int error = execvp(tmp[0],tmp);

       if(error < 0)
        {
            printf("Command not found~!\n");
         }
       free(tmp);
       exit(0);
    }
    else
        wait(NULL);

}

/*
    Implementation of process_cmd

    TODO: Clearly explain how you implement process_cmd in point form. For example:

    Step 1: ....
    Step 2: ....
        Step 2.1: .....
        Step 2.2: .....
            Step 2.2.1: .....
    Step 3: ....

 */

/*
    Implementation of process_cmd

    TODO: Clearly explain how you implement process_cmd in point form. For example:

    Step 1: ....
    Step 2: ....
        Step 2.1: .....
        Step 2.2: .....
            Step 2.2.1: .....
    Step 3: ....

 */

/*Implementation of process_cmd
 * Step1: variable defintion
 * 			type: indicate whether the program is executing in normal mode or in pipeline mode
 * 			k: the k is an indicator which used to indicate the current position in cmdline, 
 * 				++k will make k to move
 * 			c: when it is executing in reading cmdline, c is used to check whether it is the end
 * 				of char array. C is also used to check whether it is a pipeline mode not.
 * 			
 * Step2: check exit command
 * 			Step2.1: use while loop to take out the spaces before the useful command in cmdline
 * 			Step2.2: use if to check whether the cmdline is an exit command, if the command is
 * 					exit, the program will be terminated and display the pid and termination
 * 					message
 * 					
 * Step3: function array definition
 * 			I defined an user-defined type function pointer which the parameter is char*.
 * 			Step3.1: I defined a function pointer array which has exec and piped. 0 is exec() 
 * 					and 1 is piped(). 
 * 			Step3.2: reset the k to 0
 * 			step3.3: assignment the first element of cmdline to c
 * 			
 * Step4: use a while loop to check whether the program should be operated under normal mode
 * 			or pipeline mode
 * 			Step4.1: If the cmdline contains "|", it means the program should be operated
 * 				pipeline mode. The type will be assigned to 1 which means a pipeline mode.
 * 			Step4.2: use k++ to move the position of reading cmdline
 * 			Step4.3: assign the character in cmdline to c
 * 			
 * Step5: make a function call, if type is 0, it will call exec() to operate under normal mode. If type is 1,
 * 			it will call piped() to operate under pipeline mode.
 * 
 * 
 * Implementation of exec()
 * Step 1: variable defintion
 * 		tmp: tmp is a 2d char pointer array which points to different command, for example: if the cmdline
 * 			is "ls -l", then tmp[0] contains "ls" and tmp[1] is "-l"
 * 		num: the num is an indicator which used to indicate the current position in cmdline, 
 * 			++num will make num to move
 * 		pid: store the value which returns by fork()
 * 		
 * Step2: split the cmdline and take out all the spaces in cmdline, if the cmdline is "ls -l", then tmp[0] 
 * 		contains "ls" and tmp[1] is "-l"
 * 		
 * Step3: use fork() to create a new process and the return value will be stored in pid
 * 
 * Step4: use if to check whether it returns 0 or not, if it returns 0, it will enter the if block
 * 		Step4.1: define a new int variable called error which is used to save the returned value of execvp(),
 * 			at the same time, it will execute execvp according to the cmdline
 * 		Step4.2: use if to check whether execvp returns negative number or not, if it returns negative number,
 * 		 	it means the cmdline not found and a message will be displayed on the screen to let the user know
 * 
 * Step5: to release the dynamically allocated memory of tmp
 * 
 * Step6: if the fork() is not return 0, wait(null) will block parent process until its childern have finished
 * 
 * 
 * Implementation of piped()
 * Step1: take out all the "|" which contains in cmdline
 * 
 * Step2: Open a process by creating a pipe. If the mode is "r", when the child process is started, 
 * 		its stdout is the writable end of the pipe, and the file descriptor of the returned stream 
 * 		is the readable end of the pipe. If the mode is "w", when the child process is started, its 
 * 		stdin is the readable end of the pipe, and the file descriptor of the returned stream is the 
 * 		writable end of the pipe.
 * 
 * Step3: if one of the popen() returns null, a message will be displayed to tell the user it encounters
 * 		an error
 * 
 * Step4: get the string from in, and write the string to out
 * 
 * Step5: close a stream that was opened by popen(), wait for the command to terminate, and return the 
 * 		termination status of the process that was running
 * 
 */
void process_cmd(char *cmdline)
{
    printf("Debug: %s\n", cmdline); // delete this line to start your work

    int type = 0;
    int k = 0;
    char c;
    
    //exit
    while(cmdline[k] == ' ')
    {
    	++k;
    }
    if(cmdline[k]=='e' && cmdline[k+1]=='x' && cmdline[k+2]=='i' && cmdline[k+3]=='t')
    {
    	printf("The shell program (pid=%d) terminates\n", getpid());
    	exit(0);
    }
    
     //function mapping
     fun fun_array[] ={exec,piped};
     k=0;
     c = *(cmdline+k);

     while(c != '\0')
     {
    	 if(c == '|')
         {
            type = 1; //pipeline
         }
       ++k;
       c = *(cmdline+k);
     }
     //choose the correct type to execute the function
     (*fun_array[type])(cmdline);



}

void show_prompt()
{
    printf("$> ");
}

int get_cmd_line(char *cmdline)
{
    int i;
    int n;
    if (!fgets(cmdline, MAX_CMDLINE_LEN, stdin))
        return -1;
    // Ignore the newline character
    n = strlen(cmdline);
    cmdline[--n] = '\0';
    i = 0;
    while (i < n && cmdline[i] == ' ') {
        ++i;
    }
    if (i == n) {
        // Empty command
        return -1;
    }
    return 0;
}

void **tokenize(char **argv, char *line, int *numTokens, char *delimiter)
{
    int argc = 0;
    char *token = strtok(line, delimiter);
    while (token != NULL)
    {
        argv[argc++] = token;
        token = strtok(NULL, delimiter);
    }
    argv[argc++] = NULL;
    *numTokens = argc - 1;
}


