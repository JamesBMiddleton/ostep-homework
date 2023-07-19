#include <dirent.h>
#include <stdio.h>

int main(int argc, char* argv[])
{

    DIR* dp = opendir(".");
    struct dirent* d;
    while((d = readdir(dp)) != NULL)
    {
        if ((int)d->d_type == 4) // is a directory
            printf("%s/\n", d->d_name);
        else
            printf("%s\n", d->d_name);
    }
    closedir(dp);
    return 0;
}

