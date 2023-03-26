#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // system calls?
#include <string.h>

int main(int argc, char* argv[])
{
    int x = 100;
    int rc = fork();
    if (rc < 0)
    {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0)
        printf("value of x in child process = %d\n", x);
    else
        printf("value of x in parent process = %d\n", x);
    return 0;
}
