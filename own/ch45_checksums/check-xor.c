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

    unsigned char checksum = 0;
    unsigned char tmp;
    while(read(fd, &tmp, 1))
        checksum = checksum ^ tmp;

    printf("checksum: 0x%02x\n", (int)checksum);
}


