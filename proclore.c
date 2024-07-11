#include "headers.h"

void get_process_info(pid_t pid)
{
    char proc_path[MAX_PATH_LEN];
    snprintf(proc_path, sizeof(proc_path), "/proc/%d/exe", pid);

    char exec_path[MAX_PATH_LEN];
    ssize_t len = readlink(proc_path, exec_path, sizeof(exec_path) - 1);
    if (len != -1)
    {
        exec_path[len] = '\0';
    }
    else
    {
        perror("Error reading executable path");
        return;
    }

    char status_path[MAX_PATH_LEN];
    snprintf(status_path, sizeof(status_path), "/proc/%d/status", pid);

    FILE *status_file = fopen(status_path, "r");
    if (status_file)
    {
        char line[MAX_INPUT_LEN];
        char process_status[MAX_INPUT_LEN];
        char process_group[MAX_INPUT_LEN];
        char virtual_memory[MAX_INPUT_LEN];

        while (fgets(line, sizeof(line), status_file))
        {
            if (sscanf(line, "State:\t%s", process_status) == 1)
            {
                // Process status found
            }
            else if (sscanf(line, "VmSize:\t%s", virtual_memory) == 1)
            {
                // Virtual memory size found
            }
            else if (sscanf(line, "PGrp:\t%s", process_group) == 1)
            {
                // Process group found
            }
        }

        printf("pid : %d\n", pid);
        printf("process status : %s\n", process_status);
        printf("Process Group : %s\n", process_group);
        printf("Virtual memory : %s\n", virtual_memory);
        printf("executable Path : %s\n", exec_path);

        fclose(status_file);
    }
    else
    {
        perror("Error opening status file");
    }
}