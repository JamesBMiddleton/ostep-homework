#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // system calls?
#include <string.h>

int main(int argc, char* argv[])
{
    int pid = fork();
    if (pid < 0)
    {
        fprintf(stderr, "fork 1 failed\n");
        exit(1);
    }
    else if (pid == 0)
    {
        printf("child 1\n");
    }
    else
    {
        printf("parent after first fork\n");
        int pid = fork();
        if (pid < 0)
        {
            fprintf(stderr, "fork 1 failed\n");
            exit(1);
        }
        else if (pid == 0)
        {
            printf("child 2\n");
        }
        else
        {
            printf("parent after second fork\n");
        }
    }
    return 0;
}
