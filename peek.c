#include "headers.h"

void peek_directory(const char *path)
{
    struct dirent *entry;
    DIR *dir = opendir(path);

    if (dir == NULL)
    {
        fprintf(stderr, "ERROR: Could not open directory '%s'\n", path);
        return;
    }

    while ((entry = readdir(dir)))
    {
        if (!show_hidden_files && entry->d_name[0] == '.')
            continue; 
        char full_path[MAX_PATH_LEN];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        struct stat st;
        if (stat(full_path, &st) == 0)
        {
            printf("%s", entry->d_name);
            if (show_extra_info)
            {
                printf(" (%s, %ld bytes, %s)", S_ISDIR(st.st_mode) ? "directory" : "file", (long)st.st_size, ctime(&st.st_mtime));
            }
            printf("\n");
        }
        else
        {
            perror("stat");
        }
    }

    closedir(dir);
}