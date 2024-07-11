#include "headers.h"

void execute_single_command(char *command, struct timeval *command_start_time, struct timeval *command_end_time)
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
        int redirect_index1 = -1;
        for (int j = 0; j < arg_count; j++)
        {
            if (strcmp(args[j], ">") == 0)
            {
                redirect_index1 = j;
                break;
            }
        }
        int redirect_index2 = -1;
        for (int j = 0; j < arg_count; j++)
        {
            if (strcmp(args[j], ">>") == 0)
            {
                redirect_index2 = j;
                break;
            }
        }
        int input_redirect_index3 = -1;
        for (int j = 0; j < arg_count; j++)
        {
            if (strcmp(args[j], "<") == 0)
            {
                input_redirect_index3 = j;
                break;
            }
        }
        if (strcmp(args[0], "warp") == 0)
        {
            char pastevents_args[MAX_INPUT_LEN] = "";
            for (int i = 1; i < arg_count; i++)
            {
                strcat(pastevents_args, args[i]);
                if (i < arg_count - 1)
                {
                    strcat(pastevents_args, " ");
                }
            }

            add_to_pastevents(args[0], pastevents_args);
            for (int i = 1; i < arg_count; i++)
            {
                warp_command(args[i]);
            }
        }
        else if (strcmp(args[0], "proclore") == 0)
        {
            char pastevents_args[MAX_INPUT_LEN] = "";
            for (int i = 1; i < arg_count; i++)
            {
                strcat(pastevents_args, args[i]);
                if (i < arg_count - 1)
                {
                    strcat(pastevents_args, " ");
                }
            }

            add_to_pastevents(args[0], pastevents_args);
            if (arg_count == 1)
            {
                pid_t shell_pid = getpid();
                get_process_info(shell_pid);
            }
            else if (arg_count == 2)
            {
                pid_t target_pid = atoi(args[1]);
                get_process_info(target_pid);
            }
            else
            {
                fprintf(stderr, "ERROR: Invalid usage of 'proclore'\n");
            }
        }
        else if (strcmp(args[0], "peek") == 0)
        {
            char pastevents_args[MAX_INPUT_LEN] = "";
            for (int i = 1; i < arg_count; i++)
            {
                strcat(pastevents_args, args[i]);
                if (i < arg_count - 1)
                {
                    strcat(pastevents_args, " ");
                }
            }

            add_to_pastevents(args[0], pastevents_args);
            show_hidden_files = 0;
            show_extra_info = 0;
            for (int i = 1; i < arg_count; i++)
            {
                if (strcmp(args[i], "-a") == 0)
                {
                    show_hidden_files = 1;
                }
                else if (strcmp(args[i], "-l") == 0)
                {
                    show_extra_info = 1;
                }
            }
            char *peek_path = current_dir;
            if (arg_count >= 2)
            {
                char *peek_arg = args[arg_count - 1];
                if (peek_arg[0] == '~' && peek_arg[1] == '/')
                {
                    char expanded_path[MAX_PATH_LEN+2];
                    snprintf(expanded_path, sizeof(expanded_path), "%s/%s", initial_dir, peek_arg +2);
                    expanded_path[MAX_PATH_LEN]='\0';
                    expanded_path[MAX_PATH_LEN+1]='\0';
                    peek_path = expanded_path;
                }
                else if (strcmp(peek_arg, "~") == 0)
                    peek_path = initial_dir;
                else if (strcmp(peek_arg, "..") == 0)
                {
                    char parent_path[MAX_PATH_LEN+3];
                    snprintf(parent_path, sizeof(parent_path), "%s/..", current_dir);
                     parent_path[MAX_PATH_LEN]='\0';
                    parent_path[MAX_PATH_LEN+1]='\0';
                    parent_path[MAX_PATH_LEN+2]='\0';
                    peek_path = parent_path;
                }
                else
                    peek_path = peek_arg;
            }
            peek_directory(peek_path);
        }
        else if (strcmp(args[0], "pastevents") == 0)
        {
            if (arg_count == 1)
            {
                load_pastevents_from_file();
                for (int i = 0; i < pastevents_count; i++)
                {
                    printf("%s %s\n", pastevents[i].command, pastevents[i].args);
                }
            }
            else if (strcmp(args[1], "purge") == 0)
                clear_pastevents();
            else if (strcmp(args[1], "execute") == 0)
            {
                if (arg_count == 2)
                {
                    fprintf(stderr, "ERROR: Missing index for 'pastevents execute'\n");
                }
                else
                {
                    int index = atoi(args[2]);
                    int i = pastevents_count - index;
                    char a[2048];
                    strcpy(a, pastevents[i].command);
                    char b[2048];
                    strcpy(b, pastevents[i].args);
                    execute_pastevent(pastevents_count - index);
                    add_to_pastevents(a, b);
                }
            }
            else
                fprintf(stderr, "ERROR: Invalid subcommand for 'pastevents'\n");
        }
        else if (redirect_index1 != -1 && redirect_index2 == -1 && input_redirect_index3 == -1)
        {
            const char *filename = args[redirect_index1 + 1];

            if (freopen(filename, "w", stdout) == NULL)
            {
                perror("freopen");
                return;
            }
            args[redirect_index1] = NULL;
            args[redirect_index1 + 1] = NULL;
            strcpy(prev_command, args[0]);
            add_to_pastevents(args[0], arg_count > 1 ? args[1] : " ");
            pid_t pid = fork();
            if (pid == 0)
            {
                execvp(args[0], args);
                fprintf(stderr, "ERROR: '%s' is not a valid command\n", args[0]);
                exit(EXIT_FAILURE);
            }
            else
            {
                int status;
                gettimeofday(command_start_time, NULL);
                waitpid(pid, &status, 0);
                gettimeofday(command_end_time, NULL);
            }
            freopen("/dev/tty", "w", stdout);
        }
        else if (redirect_index1 == -1 && redirect_index2 != -1 && input_redirect_index3 == -1)
        {
            const char *filename = args[redirect_index2 + 1];

            if (freopen(filename, "a", stdout) == NULL)
            {
                perror("freopen");
                return;
            }
            args[redirect_index2] = NULL;
            args[redirect_index2 + 1] = NULL;
            strcpy(prev_command, args[0]);
            add_to_pastevents(args[0], arg_count > 1 ? args[1] : " ");
            pid_t pid = fork();
            if (pid == 0)
            {
                execvp(args[0], args);
                fprintf(stderr, "ERROR: '%s' is not a valid command\n", args[0]);
                exit(EXIT_FAILURE);
            }
            else
            {
                int status;
                gettimeofday(command_start_time, NULL);
                waitpid(pid, &status, 0);
                gettimeofday(command_end_time, NULL);
            }
            freopen("/dev/tty", "w", stdout);
        }
        else if (redirect_index1 == -1 && redirect_index2 == -1 && input_redirect_index3 != -1)
        {
            if (input_redirect_index3 + 1 < arg_count)
            {
                const char *filename = args[input_redirect_index3 + 1];
                args[input_redirect_index3] = strdup(filename);
                args[input_redirect_index3 + 1] = NULL;
            }
            strcpy(prev_command, args[0]);
            add_to_pastevents(args[0], arg_count > 1 ? args[1] : " ");
            pid_t pid = fork();
            if (pid == 0)
            {
                execvp(args[0], args);
                fprintf(stderr, "ERROR: '%s' is not a valid command\n", args[0]);
                exit(EXIT_FAILURE);
            }
            else
            {
                int status;
                gettimeofday(command_start_time, NULL);
                waitpid(pid, &status, 0);
                gettimeofday(command_end_time, NULL);
            }
        }
        else if (input_redirect_index3 != -1 && redirect_index1 != -1 && redirect_index2 == -1)
        {
            const char *filename = args[redirect_index1 + 1];
            if (freopen(filename, "w", stdout) == NULL)
            {
                perror("freopen");
                return;
            }
            args[redirect_index1] = NULL;
            args[redirect_index1 + 1] = NULL;
            if (input_redirect_index3 + 1 < arg_count)
            {
                const char *filename = args[input_redirect_index3 + 1];
                args[input_redirect_index3] = strdup(filename);
                args[input_redirect_index3 + 1] = NULL;
            }
            strcpy(prev_command, args[0]);
            add_to_pastevents(args[0], arg_count > 1 ? args[1] : " ");
            pid_t pid = fork();
            if (pid == 0)
            {
                // args[0]=(char*)
                execvp(args[0], args);
                args[1] = strdup(filename);
                args[2] = NULL;
                execvp(args[0], args);
                fprintf(stderr, "ERROR: '%s' is not a valid command\n", args[0]);
                exit(EXIT_FAILURE);
            }
            else
            {
                int status;
                gettimeofday(command_start_time, NULL);
                waitpid(pid, &status, 0);
                gettimeofday(command_end_time, NULL);
            }
            freopen("/dev/tty", "w", stdout);
        }
        else if (input_redirect_index3 != -1 && redirect_index2 != -1 && redirect_index1 == -1)
        {
            const char *filename = args[redirect_index2 + 1];
            if (freopen(filename, "a", stdout) == NULL)
            {
                perror("freopen");
                return;
            }
            args[redirect_index2] = NULL;
            args[redirect_index2 + 1] = NULL;
            if (input_redirect_index3 + 1 < arg_count)
            {
                const char *filename = args[input_redirect_index3 + 1];
                args[input_redirect_index3] = strdup(filename);
                args[input_redirect_index3 + 1] = NULL;
            }
            strcpy(prev_command, args[0]);
            add_to_pastevents(args[0], arg_count > 1 ? args[1] : " ");
            pid_t pid = fork();
            if (pid == 0)
            {
                // args[0]=(char*)
                execvp(args[0], args);
                args[1] = strdup(filename);
                args[2] = NULL;
                execvp(args[0], args);
                fprintf(stderr, "ERROR: '%s' is not a valid command\n", args[0]);
                exit(EXIT_FAILURE);
            }
            else
            {
                int status;
                gettimeofday(command_start_time, NULL);
                waitpid(pid, &status, 0);
                gettimeofday(command_end_time, NULL);
            }
            freopen("/dev/tty", "w", stdout);
        }
        else if (redirect_index1 != -1 && redirect_index2 != -1)
        {
            return;
        }
        else
        {
            strcpy(prev_command, args[0]);
            add_to_pastevents(args[0], arg_count > 1 ? args[1] : " ");
            pid_t pid = fork();
            if (pid == 0)
            {
                execvp(args[0], args);
                fprintf(stderr, "ERROR: '%s' is not a valid command\n", args[0]);
                exit(EXIT_FAILURE);
            }
            else
            {
                int status;
                gettimeofday(command_start_time, NULL);
                waitpid(pid, &status, 0);
                foreground_process_command_name[0] = (char *)malloc(sizeof(args[0]) + sizeof(args[1]) + 1);
                int x;
                for (x = 0; x < strlen(args[0]); x++)
                {
                    foreground_process_command_name[0][x] = args[0][x];
                }
                foreground_process_command_name[0][x] = ' ';
                x++;
                int y = 0;
                for (y = 0; y < strlen(args[1]); y++)
                {
                    foreground_process_command_name[0][x] = args[1][x];
                    x++;
                }
                foreground_process_command_name[0][x] = '\0';
                foreground_process_command_name[1] = NULL;
                foreground_process_pid = pid;
                gettimeofday(command_end_time, NULL);
            }
        }
    }
}

void execute_command(char *command, bool semicolon, bool ampersand, struct timeval *command_start_time, struct timeval *command_end_time)
{
    if (command == NULL || strlen(command) == 0)
    {
        peek_directory(current_dir);
        return;
    }

    int has_pipe = strstr(command, "|") != NULL;
    if (has_pipe)
    {
        execute_piped_commands(command, command_start_time, command_end_time);
    }
    else
    {
        execute_single_command(command, command_start_time, command_end_time);
    }
}