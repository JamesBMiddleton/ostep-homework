#include <stdlib.h>

int main(int argc, char* argv[])
{
    int* i = (int*)malloc(sizeof(int)*100);
    *i = 0;
    free(i+50);
    return 0;
} 
