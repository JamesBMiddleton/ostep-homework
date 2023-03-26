#define _GNU_SOURCE // CPU_ZERO
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sched.h>

/* Time how long it takes to switch between two process by creating a child 
 * process, setting up two pipes between the parent and child process and
 * continually writing to one pipe in child and reading from it from parent, 
 * then writing to pipe from parent and reading from pipe in child.
 *
 * the context switches between parent and child each loop - I'm not entirely
 * sure what is triggering the switch, the idea in the book was after each
 * write to pipe, the process would wait on the other to read it, but I don't
 * know how you tell a process to wait for another process to read from the
 * pipe you just wrote to...*/

int main(int argc, char* argv[])
{
    cpu_set_t set; // void array representing CPU
    CPU_ZERO(&set); // initialize it correctly
    CPU_SET(1, &set); // add 1 single cpu to the set

    char* pipe_input_text = "hello world";
    int pipedes_1[2]; // array to store fds for pipe output [0] and input [1] 
    if (pipe(pipedes_1) < 0)  // populate pipedes - create the pipe
        exit(1);

    int pipedes_2[2]; 
    if (pipe(pipedes_2) < 0)
        exit(1);

    int num_switches = 10000;

    struct timeval starttime;
    struct timeval endtime;
    gettimeofday(&starttime, NULL);


    int pid = fork();
    if (pid < 0)
        exit(1);
    else if (pid == 0)
    {
        if (sched_setaffinity(getpid(), sizeof(set), &set) == -1)
            exit(1); // make child process only run on cpu 1
        char pipe_output_text[11];
        for (int i = 0; i < num_switches; ++i)
        {
            // printf("child\n");
            write(pipedes_1[1], pipe_input_text, strlen(pipe_input_text));
            // printf("child\n");
            read(pipedes_2[0], pipe_output_text, strlen(pipe_input_text));
            // printf("child\n");
        }
    }
    else
    {
        if (sched_setaffinity(getpid(), sizeof(set), &set) == -1)
            exit(1); // make parent process only run on cpu 1
        char pipe_output_text[11];
        for (int i = 0; i < num_switches; ++i)
        {
            // printf("parent\n");
            read(pipedes_1[0], pipe_output_text, strlen(pipe_input_text));
            // printf("parent\n");
            write(pipedes_2[1], pipe_input_text, strlen(pipe_input_text));
            // printf("parent\n");
        }
    }
    waitpid(pid, NULL, 0);

    gettimeofday(&endtime, NULL);

    double time_per_switch = (double)(endtime.tv_usec - starttime.tv_usec)
                            / num_switches;

    printf("time per context switch: %f microseconds\n", time_per_switch);
    return 0;
}
