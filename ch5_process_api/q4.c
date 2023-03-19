#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
    int rc = fork();
    if (rc < 0)
    {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0)
    {
        char* args[] = {"ls", NULL}; // first argv is always program name
        execvp(args[0], args);
    }
    else
    {
        wait(NULL);
        printf("ls call completed\n");
    }
    return 0;
}
