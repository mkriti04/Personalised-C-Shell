#include "headers.h"
void print_background_processes() {
    for (int i = 0; i < process_count; i++) {
        if(process_list.pid[i]!=INT_MIN && process_list.command_name[i]!=NULL)
        {
            if (process_states[i] == 0) {
                printf("%d : %s - Running\n", process_list.pid[i], process_list.command_name[i]);
            } else {
                printf("%d : %s - Stopped\n", process_list.pid[i], process_list.command_name[i]);
            }
        }
    }
}
