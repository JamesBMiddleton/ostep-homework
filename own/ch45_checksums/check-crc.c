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
    const int k = 255;
    long unsigned int tmp;
    read(fd, &tmp, 8);
    checksum = tmp % k; // its more complicated when you're computing it over more data...
                        // https://stackoverflow.com/questions/15169387/definitive-crc-for-c

    printf("checksum: 0x%02x\n", (int)checksum);
}


