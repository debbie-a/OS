#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdbool.h>


//these defines are for the designing of my shell
#define RESET   "\033[0m"
#define BOLDGREEN   "\033[1m\033[32m"
#define BOLDBLUE    "\033[1m\033[34m"    
#define BOLDWHITE   "\033[1m\033[37m"
#define clear() printf("\033[H\033[J")


void shell_design()//func to print command line 
{
	printf(BOLDGREEN "Debbie@Debbie-VirtualBox"  RESET);
	printf(BOLDWHITE ":"  RESET);
	printf(BOLDBLUE "~ "  RESET);
}

struct sigaction sa;

int prepare(void)
{	
	sa.sa_handler = SIG_IGN; 
	sigaction(SIGINT, &sa, NULL);//parent (shell) and background child should not terminate upon SIGINT 

	shell_design();//call func to design my shell!
	return 0;
}

int process_arglist(int count, char **arglist)
{	
	int pipefd[2];
	int pipe_position;
	int command_case = 1; // "flag" to tell command case (1=regular, 2=&, 3=pipe)
	pid_t child_pid;

	for(int i=0; i < count; i++)//check for pipe
	{
		if(strcmp(arglist[i], "|") == 0)
		{
			command_case = 3;
			if (pipe(pipefd) < 0) 
			{ 
        			fprintf(stderr,"Pipe could not be initialized\n"); 
       				exit(1);
			}
			pipe_position = i;
			arglist[i] = '\0';
			break;
		}		
	}
	if(command_case != 3)//check for &
	{
		if(strcmp(arglist[count-1],"&") == 0)
		{
			command_case = 2;
			arglist[count-1] = '\0';
		}
	}

	switch(command_case)
	{
		case 1://no pipe no &
			child_pid = fork();
			if (child_pid < 0)
			{ 
				fprintf(stderr,"Error: fork failed\n");
				exit(1);
			}
			else if(child_pid == 0)
			{
				sa.sa_handler = SIG_DFL;//foreground child processes should terminate upon SIGINT. 
				sigaction(SIGINT, &sa, NULL);
				if(execvp(arglist[0], arglist) < 0)
				{
					fprintf(stderr,"%s: command not found\n", arglist[0]);
					exit(1);
				}
			}
			else if(child_pid > 0)
			{
				int exit_code;
				waitpid(child_pid,&exit_code, WUNTRACED);//parent waiting for the child process to finish
			}
			break;

		case 2://& case
			child_pid = fork();
			if (child_pid < 0)
			{ 
				fprintf(stderr,"Error: fork failed\n");
				exit(1);
			}
			else if(child_pid == 0)
			{
				if(execvp(arglist[0], arglist) < 0)
				{
					fprintf(stderr,"%s: command not found\n", arglist[0]);
					exit(1);
				}
			}
			else if(child_pid > 0)
			{
				signal(SIGCHLD,SIG_IGN);//siganl to prevent zombies
			}
			break;

		case 3://pipe case
			child_pid = fork();
			if (child_pid < 0)
			{ 
				fprintf(stderr,"Error: fork failed\n");
				exit(1);
			}
			else if(child_pid == 0)
		    	{	
				sa.sa_handler = SIG_DFL; //foreground child processes should terminate upon SIGINT.
				sigaction(SIGINT, &sa, NULL);
				close(pipefd[0]); 
				dup2(pipefd[1], STDOUT_FILENO); 
				close(pipefd[1]);
				if(execvp(arglist[0], arglist) < 0)
				{
					fprintf(stderr,"%s: command not found\n", arglist[0]);
					exit(1);
				}
			}
			else if(child_pid > 0)
			{
				pid_t child2_pid = fork();
				if(child2_pid < 0)
				{
					fprintf(stderr,"Error: fork failed\n");
					exit(1);
				}
				else if(child2_pid == 0)
				{
					sa.sa_handler = SIG_DFL; //foreground child processes should terminate upon SIGINT.
					sigaction(SIGINT, &sa, NULL);
					close(pipefd[1]); 
					dup2(pipefd[0], STDIN_FILENO); 
					close(pipefd[0]); 
					if(execvp(arglist[pipe_position+1], arglist+pipe_position+1) < 0)
					{
						fprintf(stderr,"%s: command not found\n", arglist[pipe_position+1]);
						exit(1);
					}
				}
				else if(child2_pid > 0)
				{	
					close(pipefd[0]);
					close(pipefd[1]);
					wait(NULL);//parent waiting for children processes to finish
					wait(NULL);
				}
			}
			break;
	}
	shell_design();//print again command-line
	return 1;
}
int finalize(void)
{
	return 0;
}
