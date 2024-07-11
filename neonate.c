#include "headers.h"
int kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch == 'x') {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

void neonate(char* command)
{
    if (command == NULL || strlen(command) == 0)
    {
        peek_directory(current_dir);
        return;
    }
    int has_semicolon = strstr(command, ";") != NULL;
    char *commands[MAX_INPUT_LEN];
    int command_count = 0;
    char *token = has_semicolon ? strtok(command, ";") : strtok(command, "&");
    while (token != NULL && command_count < MAX_INPUT_LEN)
    {
        commands[command_count] = token;
        command_count++;
        token = has_semicolon ? strtok(NULL, ";") : strtok(NULL, "&");
    }
    for (int i = 0; i < command_count; i++)
    {
        char *current_command = commands[i];
        char *args[MAX_INPUT_LEN];
        int arg_count = 0;
        char *subtoken = strtok(current_command, " \t");
        while (subtoken != NULL && arg_count < MAX_INPUT_LEN - 1)
        {
            args[arg_count] = subtoken;
            arg_count++;
            subtoken = strtok(NULL, " \t");
        }
        args[arg_count] = NULL;
        int time_arg=atoi(args[2]); 
    struct timeval tv;
    tv.tv_sec = time_arg;
    tv.tv_usec = 0;

    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    int flag=0;
    while (!flag)
    {
        pid_t pid = getpid();
        printf("%d\n", pid);
        fflush(stdout);

        int ret = select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);

        if (ret == -1)
        {
            perror("select");
            exit(EXIT_FAILURE);
        }
        else if (ret == 0)
        {
            tv.tv_sec = time_arg;
            tv.tv_usec = 0;
        }
        else if (FD_ISSET(STDIN_FILENO, &fds))
        {
            stop_neonate = 1;
        }
         if (kbhit()) {
            char input = getchar();
            if (input == 'x') {
                return; 
            }
         }
    }
    }
}
