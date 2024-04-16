#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	//first arg is pipe command anyway
	int count = argc-1;
	if(count <= 0) {
		printf("parse error near '|'");
		return 1;
	}
	
	pid_t ids[count];
	int pipes[count-1][2];

	//fill pipes
	for(int i = 0; i<count-1; i++) {
		if(pipe(pipes[i]) == -1) {
			perror("pipe");
			exit(1);
		}
	}

	//main loop
	for(int i = 0; i<count; i++) {
		ids[i] = fork();
		//failed fork
		if(ids[i]==-1) {
			perror("fork");
			exit(1);
		}
		if(ids[0] == 0) {
			if(i>=1) {
				dup2(pipes[i-1][0], STDIN_FILENO);
				close(pipes[i-1][0]);
			}

			//all other cases
			if(i<count-1) {
				dup2(pipes[i][0], STDIN_FILENO);
				close(pipes[i][0]);
			}
			else dup2(STDOUT_FILENO, STDOUT_FILENO);

			//close pipes
			for(int j = 0; j<count-1; j++) {
				close(pipes[j][0]);
				close(pipes[j][1]);
			}

			//execute command
			execlp(argv[i+1], argv[i+1], NULL);
			perror("excelp");
			exit(1);
		}
	}

	//close pipes
	for(int j = 0; j<count-1; j++) {
		close(pipes[j][0]);
		close(pipes[j][1]);
	}

	//wait for all child proc to finish
	for(int i = 0; i<count; i++) {
		int status;
		waitpid(ids[i], &status, 0);
		if(!WIFEXITED(status) || WIFEXITED(status) != 0) {
			printf("Command '%s' fialed with ex;it code %d\n", 
			argv[i+1], WEXITSTATUS(status));
			return 1;
		}
	}

	return 0;
}
