#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>

void recursive_print(char* dirname, int depth)
{
    DIR* dp = opendir(dirname);
    struct dirent* d;
    while((d = readdir(dp)) != NULL)
    {   
        if (strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0)
            continue;
        for (int i = 0; i < depth; ++i)
            printf("    "); 
        if (d->d_type == 4) // is a directory
        {
            printf("%s/\n", d->d_name);
            char dir[1024];
            sprintf(dir, "%s/%s", dirname, d->d_name);
            recursive_print(dir, depth+1);
        }
        else
        {
            printf("%s\n", d->d_name);
        }
    }
    closedir(dp);
}

int main(int argc, char* argv[])
{
    // printf("%s", argv[argc -1]);
    if (argc == 2)
        recursive_print(argv[argc-1], 0);
    else
        recursive_print(".", 0); 
    return 0;
}

