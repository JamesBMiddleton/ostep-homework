#include <stdio.h>  // fprintf()
#include <stdlib.h> // exit()
#include <unistd.h>  // write(), fork()
#include <string.h> // strlen()
#include <fcntl.h> // open()

int main(int argc, char* argv[])
{
    char* text;
    int fd = open("test.txt", O_CREAT | O_RDWR);
    int rc = fork();
    if (rc < 0)
    {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0)
    {
        text = "hello from child\n";
        write(fd, text, strlen(text));
    }
    else
    {
        text = "hello from parent\n";
        write(fd, text, strlen(text));
        close(fd);
    }
    // close(fd);
    return 0;
}
