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
	//if there are no other cmds to pipe to, exit with error
	if (argc <= 1)
	{
		exit(EINVAL);
	}
	
	for (int i = 0; i < argc; i++)
	{
		//create a pipe with a read end [0] and a write end [1]
		int pipefd[2];

		//if we pipe the fd and get an error, exit
		if (pipe(pipefd) == -1)
		{
			exit(errno);
		}

		//fork to create a child process, where the parent will write to
		

		//if this is the last argument, we want to instead output to stdout, rather than the pipe
		if (i == argc - 1)
		{

		}
		//this is not the last argument, so we want to output to the read end of the child's pipe
		else 
		{

		}
	}

	return 0;
}
