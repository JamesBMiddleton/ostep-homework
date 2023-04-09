#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    int size = atoi(argv[1]);
    int arr[size]; // VLA! 
    while(1)
    {
        for (int i=0; i<size; ++i)
            ++arr[i];
    }
    return 0;
}
