#include "stdio.h"

int main(int argc, char* argv[])
// argc = number of arguments passed
// argv = array of C-style strings (\0 terminated char arrays)
// argv[0] is always the name of the program, so argc is always at least 1
// >> ./main one two three  --> output = ./main one two three
{
    for (int i = 0; i < argc; ++i)
        printf("%s\n", argv[i]);
    return 0;
}


