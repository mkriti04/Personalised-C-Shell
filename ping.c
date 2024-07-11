#include "headers.h"

void send_signal_to_process(pid_t pid, int signal_number) {
    signal_number=signal_number%32;
    int found=0;
    int k;
    for (int i = 0; i < MAX_BACKGROUND_PROCESSES; i++)
    {
        if(pid==process_list.pid[i])
        {
            k=i;
            found=1;
            break;
        }
    }
    if(found==1)
    {
        int signal_to_send = -1;
        switch (signal_number) {
            case 9: 
                signal_to_send = 1;
                process_list.command_name[k]=NULL;
                break;
            case 20:
                signal_to_send = 1;
                if(pid==prev_back_pid)
                    prev_back_process[0]=NULL;
                process_states[k]=1;
                break;
            default:
                printf("Unsupported signal number: %d\n", signal_number);
                return;
        }

        if (signal_to_send != -1) {
            printf("Sent signal %d to process with PID %d\n", signal_number, pid);
        } else {
            perror("kill");
        }
    } else {
        printf("No such process found with PID: %d\n", pid);
    }
}


void execute_ping_command(char *command) {
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
    if (arg_count != 3) {
        fprintf(stderr, "Usage: ping <pid> <signal_number>\n");
        return;
    }
    pid_t pid = atoi(args[1]);
    int signal_number = atoi(args[2]);
    send_signal_to_process(pid, signal_number);
    }
}

void handle_ctrl_c() {
    if (foreground_process_pid != -1) {
        kill(foreground_process_pid, SIGINT);
    }
}

void handle_ctrl_z() {
    if (foreground_process_pid != -1) {
        kill(foreground_process_pid, SIGTSTP);
        process_count++;
        process_states[process_count]=1;
        process_list.command_name[process_count]=foreground_process_command_name[0];
        process_list.pid[process_count]=foreground_process_pid;
        foreground_process_pid = -1; 
    }
}
void handle_ctrl_d() {
    printf("\nCtrl-D  detected. Exiting shell...\n");
    exit(0);
}