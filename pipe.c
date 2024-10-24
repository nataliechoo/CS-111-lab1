#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

int main(int argc, char *argv[])
{
	//printf("%d", argc);

	//if there are no other cmds to pipe to, exit with error
	if (argc <= 1)
	{
		fprintf(stderr, "Error: No commands provided.\nUsage: ./pipe cmd1 cmd2 ... cmdN\n");

		exit(EINVAL);
	}
	else if (argc == 2)
	{
		//execute command with the current arguments
		if (execlp(argv[1], argv[1], NULL) == -1)
		{
			perror("execlp");  // Print a descriptive error
			exit(errno);			
		}
		return 0;
	}
	else 
	{
		for (int i = 1; i < argc - 1; i++)
		{
			//create a pipe with a read end [0] and a write end [1]
			int pipefd[2];
			
			if (i < argc - 1)
			{
				//if we pipe the fd and get an error, exit
				if (pipe(pipefd) == -1)
				{
		            perror("pipe");  // Print pipe error
					exit(errno);
				}
			}
			
			//fork to create a child process, where the parent will write to
			int ret = fork();
			if (ret == -1)
			{
				perror("fork error");
				exit(errno);
			}

			//if we are in the CHILD PROCESS, we want to only output to the write end of the pipe
			//and since this is not the last arg, we will output it into the write end of the pipe
			if (ret == 0)
			{
				//close the read end of the pipe, as child only needs to write 
				if (close(pipefd[0]) == -1)
				{
					perror("pipe");  // Print pipe error
					exit(errno);
				}

				//redirect child's std out to the write end of the pipe
				if (dup2(pipefd[1], 1) == -1)
				{
                    perror("dup2");  // Print dup2 error					
					exit(errno);
				}
				
				//after we have redirected STDOUT to the write end, we should retire the old fd existing there
				if (close(pipefd[1]) == -1) 
				{
					perror("pipe");  // Print pipe error
					exit(errno);
				}

				//execute command with the current arguments
				if (execlp(argv[i], argv[i], NULL) == -1)
				{
					perror("execlp");
					exit(errno);
				}
				exit(errno);
			}
			//if we are in the PARENT PROCESS, we want to read the result of the child process
			//thus, we close the write end, since parents do not use it, redirect the stdin to the read end,
			//and close the old read fd
			else
			{
				if (i < argc -1)
				{
					if (close(pipefd[1]) == -1)
					{
						perror("pipe");  // Print pipe error
						exit(errno);
					}

					//redirect parent's std in to the read end of the pipe
					if (dup2(pipefd[0], 0) == -1)
					{
						perror("dup2");
						exit(errno);
					}
					
					//after we have redirected STDIN to the write end, we should retire the old fd existing there
					if (close(pipefd[0]) == -1) 
					{
						perror("pipe");  // Print pipe error
						exit(errno);
					}
				}
				
				int sts;
				wait(&sts);
				if (WEXITSTATUS(sts) != 0) {
					perror("wait error");
					exit(errno);
				}

			}
		}
			//execute last command with the right arguments
			if (execlp(argv[argc-1], argv[argc-1], NULL) == -1)
			{
				perror("execlp error");
				exit(errno);
			}
	}
	

	return 0;
}
