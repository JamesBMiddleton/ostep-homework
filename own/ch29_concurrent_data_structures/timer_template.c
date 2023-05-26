#include <time.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct timespec timespec;

int main(int argc, char* argv[])
{

    timespec ts_start;
    timespec ts_end;
    clock_gettime(CLOCK_MONOTONIC, &ts_start);
    clock_gettime(CLOCK_MONOTONIC, &ts_end);
    
    printf("shortest time = %li nanoseconds", ts_end.tv_nsec - ts_start.tv_nsec);
    return 0;
}
