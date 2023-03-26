#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
    int rc = fork();
    if (rc < 0)
    {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0)
        printf("hello ");
    else
    {
        wait(NULL);
        printf("world\n");
    }
    return 0;
}
