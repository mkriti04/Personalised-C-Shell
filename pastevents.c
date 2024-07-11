#include "headers.h"
void execute_command_1(char *command)
{
    if (command == NULL || strlen(command) == 0)
    {
        peek_directory(current_dir);
        return;
    }

    char *args[MAX_INPUT_LEN];
    int arg_count = 0;
    char *token = strtok(command, " \t");

    while (token != NULL && arg_count < MAX_INPUT_LEN - 1)
    {
        args[arg_count] = token;
        arg_count++;
        token = strtok(NULL, " \t");
    }
    args[arg_count] = NULL;

    if (strcmp(args[0], "warp") == 0)
    {

        for (int i = 1; i < arg_count; i++)
        {
            warp_command(args[i]);
        }
    }
    else if (strcmp(args[0], "peek") == 0)
    {
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
                snprintf(expanded_path, sizeof(expanded_path), "%s/%s", initial_dir, peek_arg + 2);
                expanded_path[MAX_PATH_LEN]='\0';
                expanded_path[MAX_PATH_LEN+1]='\0';
                peek_path = expanded_path;
            }
            else if (strcmp(peek_arg, "~") == 0)
            {
                peek_path = initial_dir;
            }
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
            {
                peek_path = peek_arg; 
            }
        }
        peek_directory(peek_path);
    }
    else if (strcmp(args[0], "proclore") == 0)
    {
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
    else
    {
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
            printf("Process with pid %d exited normally\n", pid);
            printf("<%d> %d\n", pid, getpid());
        }
    }
}

void clear_pastevents()
{
    pastevents_count = 0;
    pastevents_index = 0;
    FILE *file = fopen("pastevents.txt", "w");
    if (file != NULL)
    {
        fclose(file);
    }
}

void add_to_pastevents(const char *command1,const char *args1)
{
    if (pastevents_count >= 15)
    {
        for (int i = 0; i < 14; i++)
        {
            strcpy(pastevents[i].command,pastevents[i+1].command);
            strcpy(pastevents[i].args,pastevents[i+1].args);
        }
        strcpy(pastevents[14].command, command1);
        strcpy(pastevents[14].args, args1);
        save_pastevents_to_file();
    }
    else{
    if (pastevents_count > 0 &&
        strcmp(command1, pastevents[(pastevents_index - 1 + MAX_HISTORY) % MAX_HISTORY].command) == 0 &&
        strcmp(args1, pastevents[(pastevents_index - 1 + MAX_HISTORY) % MAX_HISTORY].args) == 0)
    {
        return; 
    }

    strcpy(pastevents[pastevents_index].command, command1);
    strcpy(pastevents[pastevents_index].args, args1);
    pastevents_index = (pastevents_index + 1) % MAX_HISTORY;
    if (pastevents_count < MAX_HISTORY)
    {
        pastevents_count++;
    }
    save_pastevents_to_file();
    }
}

void save_pastevents_to_file()
{
    FILE *file = fopen("pastevents.txt", "w");
    if (file == NULL)
    {
        perror("Error opening pastevents.txt for writing");
        return;
    }

    for (int i = 0; i < pastevents_count; i++)
    {
        fprintf(file, "%s %s\n", pastevents[i].command, pastevents[i].args);
    }

    fclose(file);
}

void load_pastevents_from_file()
{
    FILE *file = fopen("pastevents.txt", "r");
    if (file == NULL)
    {
        return; 
    }

    char line[MAX_INPUT_LEN];
    while (fgets(line, sizeof(line), file))
    {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n')
        {
            line[len - 1] = '\0';
        }
        char command[MAX_INPUT_LEN];
        char args[MAX_INPUT_LEN];
        if (sscanf(line, "%s %s", command, args) == 2)
        {
            add_to_pastevents(command, args);
        }
    }
    fclose(file);
}

void execute_pastevent(int index)
{
    if (index < 0 || index > pastevents_count)
    {
        fprintf(stderr, "ERROR: Invalid index\n");
        return;
    }

    char *command = pastevents[index].command;
    char *args = pastevents[index].args;
    
    if (strcmp(command, "pastevents") == 0)
    {
        return;
    }
    strcat(command, " ");
    strcat(command, args);
    execute_command_1(command);
}