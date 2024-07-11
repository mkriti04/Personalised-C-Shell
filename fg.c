#include "headers.h"

void execute_command_3(char *command, bool semicolon, bool ampersand)
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
            waitpid(pid, &status, 0);
            printf("%s exited normally (%d)\n", prev_back_process[0], prev_back_pid);
            int k=-1;
            for (int i = 0; i < MAX_PROCESS_COUNT; i++)
            {
                if(process_list.pid[i]==prev_back_pid)
                {
                    k=i;
                    break;
                }
            }
            if(k>=0 && strcmp(commands[0],"bg")!=0)
                process_states[k]=1;
            prev_back_process[0] = NULL;
            free(prev_back_process[0]);
        }
    }
}

void execute_command_2(char *command, bool semicolon, bool ampersand)
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
        pid_t pid = fork();
        if (pid == 0)
        {
            exit(EXIT_FAILURE);
        }
        else
        {
            int status;
            waitpid(pid, &status, 0);
            if (!semicolon && ampersand)
            {
                if (prev_back_process[0] == NULL)
                {
                    prev_back_process[0] = (char *)malloc(sizeof(args[0]) + 1);
                    process_list.command_name[process_count] = (char *)malloc(sizeof(args[0]) + 1);
                }
                else if (prev_back_process[0] != NULL)
                {
                    prev_back_process[0] = (char *)malloc(sizeof(args[0]) + sizeof(args[1]) + 1);
                    process_list.command_name[process_count] = (char *)malloc(sizeof(args[0]) + 1);
                }
                int i;
                for (i = 0; i < strlen(args[0]); i++)
                {
                    prev_back_process[0][i] = args[0][i];
                    process_list.command_name[process_count][i]=args[0][i];
                }
                prev_back_process[0][i] = ' ';
                process_list.command_name[process_count][i]=' ';
                i++;
                int j = 0;
                for (j = 0; j < strlen(args[1]); j++)
                {
                    prev_back_process[0][i] = args[1][j];
                    process_list.command_name[process_count][i]=args[1][j];
                    i++;
                }
                prev_back_process[0][i] = '\0';
                process_list.command_name[process_count][i]='\0';
                prev_back_process[1] = NULL;
                prev_back_pid = pid;
                process_list.pid[process_count] = prev_back_pid;
                process_count++;
                printf("%d\n", prev_back_pid);
            }
        }
    }
}
