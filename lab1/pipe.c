#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    if (argc <= 1) {
        printf("Must have at least one shell command.\n");
        return 1;
    }
	pid_t process_ids[argc-1];
    int pipe_fds[argc-2][2];

    //Create pipes
    for (int i = 0; i < argc-2; i++) {
        if (pipe(pipe_fds[i]) == -1) {
            perror("pipe");
            exit(1);
        }
    }

    for (int i = 0; i < argc-1; i++) {
        //Fork a child process for each command
        process_ids[i] = fork();
        
        //Fork failure
        if (process_ids[i] == -1) {
            perror("fork");
            exit(1);
        } 
        
        if (process_ids[i] == 0) {
            // Child process
            if (i >= 1) {
                // Redirect stdin from the read end of the last pipe
                dup2(pipe_fds[i-1][0], STDIN_FILENO);
                close(pipe_fds[i-1][0]);
            }

            if (i < argc-2) {
                //Redirect stdout to the write end of the current pipe
                dup2(pipe_fds[i][1], STDOUT_FILENO);
                close(pipe_fds[i][1]);
            }

            //Close all pipe file descriptors
            for (int j = 0; j < argc-2; j++) {
                close(pipe_fds[j][0]);
                close(pipe_fds[j][1]);
            }

            //Execute cmd
            execlp(argv[i+1], argv[i+1], NULL);
            perror("execlp");
            exit(1);
        }
    }

    //Close pipe file descriptors in the parent
    for (int i = 0; i < argc-2; i++) {
        close(pipe_fds[i][0]);
        close(pipe_fds[i][1]);
    }

    //Wait child processes to finish
    for (int i = 0; i < argc-1; i++) {
        int status;
        waitpid(process_ids[i], &status, 0);
        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
            printf("Command '%s' failed with exit code %d\n", argv[i+1], WEXITSTATUS(status));
            return 1;
        }
    }

    return 0;
}
