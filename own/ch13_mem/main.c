#include <stdio.h>
#include <stdlib.h>

// I got carried away with understanding VLAs in this one...

void func(int sz)
/* variable length arrays only allowed from c99 onwards, not allowed in C++11
 * but they've always been an extension supported by GCC so 'gcc -std=c89'
 * won't complain, you need 'gcc -std=c89 -pedantic' for it to complain 
 * https://stackoverflow.com/questions/4159746/variable-length-arrays-in-c89
 * whether you should use them appears to be a hot topic...
 * https://stackoverflow.com/questions/1887097/why-arent-variable-length-arrays-part-of-the-c-standard */
{
    int arr[sz];
    printf("sizeof arr: %lu\n", sizeof(arr)); 
}

int main(int argc, char* argv[])
{
    int size = atoi(argv[1]);
    int i;
    for (i=0; i<size; ++i)
    {
        func(i);
    }
    printf("size = %d\n", size);
    while(1) {}
    return 0;
}
