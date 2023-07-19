#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
	int fd = open(argv[argc-1], O_RDONLY, S_IRUSR|S_IWUSR);
    lseek(fd, -(atoi(argv[argc-2])), SEEK_END);

	char buf[32];
	while(read(fd, buf, 32))
        write(1, buf, 32);

    close(fd);
    return 0;
}

