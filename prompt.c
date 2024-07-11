#include "headers.h"

void prompt(const char *username, const char *systemname, const char *current_path, struct timeval command_start_time, struct timeval command_end_time) {
    // Do not hardcode the prmopt
    // printf("<Everything is a file> ");   
    char display_path[MAX_PATH_LEN];

    if (strcmp(current_path, initial_dir) == 0)
    {
        snprintf(display_path, sizeof(display_path), "~ ");
    }
    else if (strncmp(current_path, initial_dir, strlen(initial_dir)) == 0)
    {
        snprintf(display_path, sizeof(display_path), "~%s ", current_path + strlen(initial_dir));
    }
    else
    {
        snprintf(display_path, sizeof(display_path), "%s ", current_path);
    }

    printf("%s@%s:%s ", username, systemname, display_path);
    print_elapsed_time(command_start_time, command_end_time);
}


