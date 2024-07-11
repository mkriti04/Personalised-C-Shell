#ifndef __PROMPT_H
#define __PROMPT_H

// void prompt(const char *username, const char *systemname, const char *current_path);
void prompt(const char *username, const char *systemname, const char *current_path, struct timeval command_start_time, struct timeval command_end_time);

#endif