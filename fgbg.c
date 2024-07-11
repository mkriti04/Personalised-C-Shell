#include "headers.h"

void bring_to_foreground(char* command) {
    int found = 0;
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
        int pid=atoi(args[1]);
        for (int i = 0; i < process_count; i++) {
        if (process_list.pid[i] == pid) {
            found = 1;
            if (process_states[i] == 0)
            {
                process_states[i] = 1;
                prev_back_process[0] = NULL;
                free(prev_back_process[0]); 
            } 
            else
            {
                process_states[i] = 0; 
                char* dup=(char*)malloc(sizeof(process_list.command_name[i]));
                strcpy(dup,process_list.command_name[i]);
                execute_single_command_1(dup);
            }
            process_list.command_name[i]=NULL;
            process_list.pid[i]= INT_MIN;
            break;
        }
    }
    if (!found) {
        printf("No such process found\n");
    }
    }
}

void change_to_running_background(char* command) {
    int found = 0;
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
        int pid=atoi(args[1]);
    for (int i = 0; i < process_count; i++) {
        if (process_list.pid[i] == pid) {
            found = 1;
            if (process_states[i] == 0)
            {
                process_states[i] = 1;
                prev_back_process[0] = NULL;
                free(prev_back_process[0]); 
            } 
            else
            {
                process_states[i] = 0; 
                char* dup=(char*)malloc(sizeof(process_list.command_name[i]));
                strcpy(dup,process_list.command_name[i]);
                execute_command_4(dup,process_list.pid[i],0,0);
            }
            break;
        }
    }
    if (!found) {
        printf("No such process found\n");
    }
    }
}

void execute_command_4(char *command,int pid_1, bool semicolon, bool ampersand)
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
            printf("%s exited normally (%d)\n", args[0], pid_1);
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

void execute_single_command_1(char *command) 
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
        strcpy(prev_command,args[0]);
        add_to_pastevents(args[0],arg_count>1 ? args[1]: " ");
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
        }
    }
}