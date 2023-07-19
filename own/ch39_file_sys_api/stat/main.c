#include <sys/stat.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    if (argc == 2)
    {
        struct stat stats;
        stat(argv[argc-1], &stats);
        
        printf("File: %s\nSize: %ld\nBlocks: %ld\nIO block: %ld\n",
                argv[argc-1], stats.st_size, stats.st_blocks,
                stats.st_blksize);
    }
    else
    {
        printf("Provide a directory or file name as input.");
    }
    return 0;
}

