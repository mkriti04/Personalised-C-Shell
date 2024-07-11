#include "headers.h"

void print_elapsed_time(struct timeval start_time, struct timeval end_time) {
    long int elapsed_seconds = end_time.tv_sec - start_time.tv_sec;
    if (elapsed_seconds >= 2) {
        printf("%s : ",prev_command);
        printf("%lds", elapsed_seconds);
        printf("$ ");
        fflush(stdout);
    }
    else
    {
        printf("$ ");
        fflush(stdout);
    }
}