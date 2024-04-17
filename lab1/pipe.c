#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	//first arg is pipe command anyway
	if(argc <= 1) {
		printf("parse error near '|'\n");
		return 1;
	}
	
	pid_t ids[argc-1];
	int pipes[argc-2][2];
	int status;

	int in_fd = 0;

	//main loop
	for(int i = 1; i<argc; i++) {
		if(i< argc-1) {
			if(pipe(pipes[i]) == -1) {
				perror("pipe");
				exit(1);
			}	
		}

		ids[i-1] = fork();
		//failed fork
		if(ids[i]==-1) {
			perror("fork");
			exit(1);
		}

		if(ids[i-1] == 0) { //child
			if(in_fd != 0) {
				dup2(in_fd, STDIN_FILENO);
				close(in_fd);
			}

			if (i < argc - 1) {
                close(pipes[i-1][0]);
                dup2(pipes[i-1][1], STDOUT_FILENO);
                close(pipes[i-1][1]);
            }

            execlp(argv[i], argv[i], NULL);
            perror("execlp");
            exit(1);
		}
		else { //parent
			if (in_fd != 0) {
                close(in_fd);
            }
            if (i < argc - 1) {
                close(pipes[i-1][1]);
                in_fd = pipes[i-1][0]; 
            }
		}
	}
	
	//close last used input
	if(in_fd!=0) close(in_fd);


	//wait for all child proc to finish
	for(int i = 0; i<argc-1; i++) {
		waitpid(ids[i], &status, 0);
		if(!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
			printf("Command '%s' failed with exit code %d\n", 
			argv[i+1], WEXITSTATUS(status));
			return 1;
		}
	}

	return 0;
}
