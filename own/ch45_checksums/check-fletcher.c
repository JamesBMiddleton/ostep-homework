#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>

void pexit()
{
    perror("errno:");
    exit(1);
}

int main(int argc, char* argv[])
{
    int fd = open(argv[argc-1], O_RDONLY); 
    if (fd == -1)
        pexit();

    unsigned char c0 = 0;
    unsigned char c1 = 0;
    unsigned char tmp;
    while(read(fd, &tmp, 1))
    {
        c0 = (c0 + tmp) % 255;
        c1 = (c1 + c0) % 255;
    }

    printf("checksum: 0x%02x 0x%02x\n", (int)c0, (int)c1);
}


