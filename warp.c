#include "headers.h"

void warp_command(char *new_path) {
    if (new_path == NULL || strlen(new_path) == 0 || strcmp(new_path, "~") == 0) {
        chdir(initial_dir); 
        strcpy(current_dir, initial_dir); 
        printf("%s\n", current_dir);
    } else if (strcmp(new_path, "-") == 0) {
        if (history_index >= 0) {
            if (chdir(history_dirs[history_index]) == 0) {
                getcwd(current_dir, sizeof(current_dir)); 
                printf("%s\n", current_dir);
                history_index--; 
            } else {
                fprintf(stderr, "ERROR: Could not change directory to '%s'\n", history_dirs[history_index]);
            }
        } else {
            fprintf(stderr, "ERROR: No previous directory in history\n");
        }
    } else if (strcmp(new_path, ".") == 0) {
        printf("%s\n", current_dir); 
    } else if (strcmp(new_path, "..") == 0) {
        if (chdir("..") == 0) {
            getcwd(current_dir, sizeof(current_dir)); 
            printf("%s\n", current_dir);
        } else {
            fprintf(stderr, "ERROR: Could not change to the parent directory\n");
        }
    } else if (new_path[0] == '~') { 
        char expanded_path[MAX_PATH_LEN+1];
        snprintf(expanded_path, sizeof(expanded_path), "%s/%s", initial_dir, new_path + 1);
        expanded_path[MAX_PATH_LEN]='\0';
        if (chdir(expanded_path) == 0) {
            getcwd(current_dir, sizeof(current_dir)); 
            printf("%s\n", current_dir);
        } else {
            fprintf(stderr, "ERROR: Could not change directory to '%s'\n", expanded_path);
        }
    } else {
        if (chdir(new_path) == 0) {
            getcwd(current_dir, sizeof(current_dir)); 
            printf("%s\n", current_dir);
        } else {
            fprintf(stderr, "ERROR: Could not change directory to '%s'\n", new_path);
        }
    }
}

void update_history(const char *path, const char *cmd)
{
    if (history_index < MAX_HISTORY - 1)
    {
        history_index++;
    }
    else
    {
        for (int i = 0; i < MAX_HISTORY - 1; i++)
        {
            strcpy(history_dirs[i], history_dirs[i + 1]);
            strcpy(history_cmds[i], history_cmds[i + 1]);
        }
    }
    strcpy(history_dirs[history_index], path);
    strcpy(history_cmds[history_index], cmd);
}
