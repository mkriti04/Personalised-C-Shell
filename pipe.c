#include "headers.h"

void execute_piped_commands(char *pipe_command,struct timeval *command_start_time, struct timeval *command_end_time) {
    char *pipe_commands[MAX_INPUT_LEN];
    int num_pipes = 0;
    char *pipe_token = strtok(pipe_command, "|");
    while (pipe_token != NULL && num_pipes < MAX_INPUT_LEN) {
        pipe_commands[num_pipes] = pipe_token;
        num_pipes++;
        pipe_token = strtok(NULL, "|");
    }
    int pipe_fds[num_pipes - 1][2];
    for (int i = 0; i < num_pipes - 1; i++) {
        if (pipe(pipe_fds[i]) == -1) {
            perror("pipe");
            return;
        }
    }
    for (int i = 0; i < num_pipes; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            if (i > 0) {
                dup2(pipe_fds[i - 1][0], STDIN_FILENO);
                close(pipe_fds[i - 1][0]);
                close(pipe_fds[i - 1][1]);
            }
            if (i < num_pipes - 1) {
                dup2(pipe_fds[i][1], STDOUT_FILENO);
                close(pipe_fds[i][0]);
                close(pipe_fds[i][1]);
            }

            execute_single_command(pipe_commands[i], command_start_time, command_end_time);

            exit(EXIT_SUCCESS);
        } else if (pid > 0) {
            
            if (i > 0) {
                close(pipe_fds[i - 1][0]);
                close(pipe_fds[i - 1][1]);
            }
            int status;
            waitpid(pid, &status, 0);
        } else {
            perror("fork");
        }
    }
}
