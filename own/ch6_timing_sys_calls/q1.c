#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h> // open()

/* Time how long a system call takes by reading nothing from an empty file
 * in a loop */

int main(int argc, char* argv[])
{
    struct timeval starttime;
    struct timeval endtime;
    gettimeofday(&starttime, NULL);

    int fd = open("empty.txt", O_CREAT | O_RDWR);
    char empty[0];
    int num_calls = 1000000;
    for (int i = 0; i < num_calls; ++i)
        read(fd, empty, 0);
    
    gettimeofday(&endtime, NULL);

    double time_per_call = (double)(endtime.tv_usec - starttime.tv_usec)
                            / num_calls;

    printf("time per system call: %f microseconds\n", time_per_call);

    return 0;
}
