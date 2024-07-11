#include "prompt.c"
#include "pastevents.c"
#include "input.c"
#include "warp.c"
#include "peek.c"
#include "proclore.c"
#include "seek.c"
#include "fg.c"
#include "bg.c"
#include "pipe.c"
#include "activities.c"
#include "fgbg.c"
#include "ping.c"
#include "neonate.c"
#include "iMan.c"
int main()
{

    process_list.pid = (int *)malloc(sizeof(int));
    struct passwd *pw = getpwuid(getuid());
    char *username = pw->pw_name;
    struct utsname system_info;
    uname(&system_info);
    char *systemname = system_info.nodename;
    getcwd(initial_dir, sizeof(initial_dir));
    strcpy(current_dir, initial_dir);
    load_pastevents_from_file();
    struct timeval command_start_time, command_end_time;
    while (1)
    {
        prompt(username, systemname, current_dir, command_start_time, command_end_time);
        char input[4096];
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            handle_ctrl_d();
        }
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n')
        {
            input[len - 1] = '\0';
        }
        bool semicolon = 0;
        bool ampersand = 0;
        for (int i = 0; i < len; i++)
        {
            if (input[i] == ';')
            {
                semicolon = 1;
                break;
            }
            if (input[i] == '&')
            {
                ampersand = 1;
                break;
            }
        }

        char *commands[MAX_INPUT_LEN];
        int num_commands = 0;
        char *token = strtok(input, "&;");
        while (token != NULL && num_commands < MAX_INPUT_LEN)
        {
            commands[num_commands] = token;
            num_commands++;
            token = strtok(NULL, "&;");
        }
        for (int i = 0; i < num_commands; i++)
        {
            if (strncmp(input, "iMan", 4) == 0)
            {
                char *command_name = input + 5;
                fetch_man_page(command_name);
                continue;
            }
            else if (strncmp(commands[i], "neonate", 7) == 0)
            {
                neonate(input);
            }

            else if (strncmp(commands[i], "ping", 4) == 0)
            {
                execute_ping_command(input);
            }
            else if (strncmp(commands[i], "fg", 2) == 0)
            {
                bring_to_foreground(input);
            }
            else if (strncmp(commands[i], "bg", 2) == 0)
            {
                change_to_running_background(commands[i]);
            }
            else if (strcmp(input, "activities") == 0)
            {
                print_background_processes();
            }
            else if (strncmp(commands[i], "seek", 4) == 0)
            {
                execute_seek_command(commands[i]);
                if (!found_2)
                {
                    printf("No match found!\n");
                }
            }

            else
            {
                if (semicolon)
                    execute_command(commands[i], semicolon, ampersand, &command_start_time, &command_end_time);
                else if (ampersand)
                {
                    if (prev_back_process[0] != NULL)
                    {
                        execute_command_3(prev_back_process[0], semicolon, ampersand);
                    }
                    if (i == 0)
                        execute_command_2(commands[i], semicolon, ampersand);
                    if (i < num_commands - 1)
                        execute_command(commands[i + 1], semicolon, ampersand, &command_start_time, &command_end_time);
                    else if (i == num_commands - 1)
                        break;
                }
                else if (!semicolon & !ampersand)
                {
                    if (prev_back_process[0] != NULL)
                    {
                        execute_command_3(prev_back_process[0], semicolon, ampersand);
                    }
                    execute_command(commands[i], semicolon, ampersand, &command_start_time, &command_end_time);
                }
            }
        }
    }
    free(process_list.pid);
    free(process_list.command_name);
    return 0;
}