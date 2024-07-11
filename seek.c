#include "headers.h"
void seek_directory(const char *target, const char *target_dir, int only_files, int only_dirs, int execute_flag, const char *relative_path) {
    DIR *dir = opendir(target_dir);

    if (dir == NULL) {
        found_2=0;
        perror("opendir");
        return;
    }
    else
    {
        found_2=1;
    struct dirent *entry;
    int found_1 = 0;

    while ((entry = readdir(dir)) !=NULL) {
        if (strncmp(entry->d_name, ".",1) == 0 || strncmp(entry->d_name, "..",1) == 0) {
            continue;
        }

        char full_path[MAX_PATH_LEN];
        snprintf(full_path, sizeof(full_path), "%s/%s", target_dir, entry->d_name);

        struct stat st;
        if (lstat(full_path, &st) == -1) {
            perror("lstat");
            continue; 
        }

        char next_relative_path[MAX_PATH_LEN];
        snprintf(next_relative_path, sizeof(next_relative_path), "%s/%s", relative_path, entry->d_name);

        if (S_ISDIR(st.st_mode)) {
            if (only_files) {
                continue; 
            }

            if (strcmp(entry->d_name, target) == 0) {
                printf("\033[34m.%s\033[0m\n", next_relative_path);
                found_1++;

                if (execute_flag) {
                    if (access(full_path, X_OK) == 0) {
                        if (chdir(full_path) == -1) {
                            perror("chdir");
                        }
                    } else {
                        printf("Missing permissions for task!\n");
                    }
                }
            }

            seek_directory(target, full_path, only_files, only_dirs, execute_flag, next_relative_path);
        } else if (S_ISREG(st.st_mode)) {
            if (only_dirs) {
                continue;
            }

            if (strcmp(entry->d_name, target) == 0) {
                printf("\033[32m.%s\033[0m\n", next_relative_path);
                found_1++;

                if (execute_flag) {
                    if (access(full_path, R_OK) == 0) {
                        printf(".%s\n", next_relative_path); 
                    } else {
                        printf("Missing permissions for task!\n");
                    }
                }
            }
        }
    }

    closedir(dir);
    }
   
}


void execute_seek_command(char *command) {
    char *args[MAX_INPUT_LEN];
    int arg_count = 0;
    char *token = strtok(command, " \t");

    while (token != NULL && arg_count < MAX_INPUT_LEN - 1) {
        args[arg_count] = token;
        arg_count++;
        token = strtok(NULL, " \t");
    }
    args[arg_count] = NULL;

    int only_files = 0;
    int only_dirs = 0;
    int execute_flag = 0;
    int target_arg = 1;
    int target_dir_arg = 2;

    for (int i = 1; i < arg_count; i++) {
        if (strcmp(args[i], "-d") == 0) {
            if (only_files) {
                fprintf(stderr, "Invalid flags!\n");
                return;
            }
            only_dirs = 1;
        } else if (strcmp(args[i], "-f") == 0) {
            if (only_dirs) {
                fprintf(stderr, "Invalid flags!\n");
                return;
            }
            only_files = 1;
        } else if (strcmp(args[i], "-e") == 0) {
            execute_flag = 1;
        } else {
            target_arg = i;
            target_dir_arg = i + 1;
            break;
        }
    }

    char *target = args[target_arg];
    char* dup="./";
    char *target_dir_arg_value = target_dir_arg < arg_count ? args[target_dir_arg] : dup;
    char relative_path[MAX_PATH_LEN] = "";

    if (!only_files && !only_dirs) {
        only_files = 0;
        only_dirs = 0;
    }

    if (target_dir_arg_value && strcmp(target_dir_arg_value, "~") == 0) {
        char *home_dir = getenv("HOME");
        if (home_dir) {
            snprintf(relative_path, sizeof(relative_path), "./%s", home_dir);
        } else {
            fprintf(stderr, "Error: HOME environment variable not set.\n");
            return;
        }
    }

    seek_directory(target, target_dir_arg_value, only_files, only_dirs, execute_flag, relative_path);
}

