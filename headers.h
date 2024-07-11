#ifndef HEADERS_H_
#define HEADERS_H_
#include "fcntl.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/wait.h>
#include <sys/utsname.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/sysinfo.h>
#include <proc/readproc.h>
#include <sys/time.h>
#include <termios.h>
#include <signal.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h> 
#include "prompt.h"

#define MAX_BUFFER_SIZE 4096
#define MAX_PATH_LEN 1024
#define MAX_INPUT_LEN 2048
#define MAX_HISTORY 15
#define MAX_PROCESS_COUNT 100
#define MAX_BACKGROUND_PROCESSES 100

pid_t foreground_process_pid;
char* foreground_process_command_name[256];
struct BackgroundProcess {
    pid_t pid;
    char command_name[MAX_INPUT_LEN];
};

struct BackgroundProcess background_processes[MAX_BACKGROUND_PROCESSES];
int background_process_count = 0;
volatile sig_atomic_t stop_neonate = 0;
int process_states[MAX_PROCESS_COUNT]={0}; 
struct ProcessInfo {
    int* pid;
    char* command_name[256];
};
struct ProcessInfo process_list;
int process_count = 0;
int found_2=0;
char* prev_back_process[MAX_INPUT_LEN];
char prev_command[MAX_INPUT_LEN];
int prev_back_pid;
char initial_dir[MAX_PATH_LEN];
char current_dir[MAX_PATH_LEN];
char history_dirs[MAX_HISTORY][MAX_PATH_LEN];
char history_cmds[MAX_HISTORY][MAX_INPUT_LEN];
void print_background_processes();
void load_pastevents_from_file();
void get_process_info(pid_t pid);
void prompt(const char *username, const char *systemname, const char *current_path, struct timeval command_start_time, struct timeval command_end_time);
void save_pastevents_to_file();
void print_elapsed_time(struct timeval start_time, struct timeval end_time);
void execute_piped_commands(char *pipe_command,struct timeval *command_start_time, struct timeval *command_end_time);
void execute_single_command(char *command, struct timeval *command_start_time, struct timeval *command_end_time);
void execute_command(char *command, bool semicolon, bool ampersand, struct timeval *command_start_time, struct timeval *command_end_time);
void execute_command_2(char *command, bool semicolon, bool ampersand);
void execute_command_3(char *command, bool semicolon, bool ampersand);
void execute_command_1(char *command);
void warp_command(char *new_path);
void execute_pastevent(int index);
void update_history(const char *path, const char *cmd);
void peek_directory(const char *path);
void add_to_pastevents(const char *command1,const char *args1);
void seek_directory(const char *target, const char *target_dir, int only_files, int only_dirs, int execute_flag, const char *relative_path);
void execute_seek_command(char *command);
int seek_recursive(char **comands, int size, char *target, char *prompt_array, char *dir_path, int is_directory, int is_file, int is_execute, char *home, char **dir1, char **prev_dir);
// void execute_seek_command(char *command, char **dir, char **prev_dir);
void bring_to_foreground(char* command);
void execute_single_command_1(char *command);
void execute_command_4(char *command,int pid, bool semicolon, bool ampersand);
void change_to_running_background(char* command);
void send_signal_to_process(pid_t pid, int signal_number);
void execute_ping_command(char *command);
void neonate(char* command);
int history_index = -1;
struct CommandHistory
{
    char command[MAX_INPUT_LEN];
    char args[MAX_INPUT_LEN];
};

struct CommandHistory pastevents[MAX_HISTORY];
int pastevents_count = 0;
int pastevents_index = 0;
int show_hidden_files = 0;
int show_extra_info = 0;
#endif