#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // system calls?
#include <string.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
    int p_pid = getpid();
    int rc = fork();
    if (rc < 0)
    {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0)
    {
        int pid = waitpid(p_pid, NULL, 0);
        // close(STDOUT_FILENO);
        printf("child (pid = %d) waiting for parent (pid = %d)\n", 
                getpid(), p_pid); 
    }
    else
    {
        printf("parent printing parent pid = %d\n", getpid());
    }
    return 0;
}
