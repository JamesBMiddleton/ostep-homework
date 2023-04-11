#include <stdlib.h>
#include <stdio.h>

struct IntVector
{
   int* arr; 
   int size;
   int capacity;
};
typedef struct IntVector IntVector;

int initialise_vector(IntVector* vec)
{
    vec->size = 0;
    vec->capacity = 10; 
    vec->arr = (int*)malloc(sizeof(int)*vec->capacity);
    return 0;
}

int push_back(IntVector* vec, int value)
{
    if (vec->size == vec->capacity)
    {
        vec->capacity *= 2;
        vec->arr = realloc(vec->arr, sizeof(int)*vec->capacity);
    }
    vec->arr[vec->size++] = value;
    return 0;
}

int at(IntVector* vec, int i)
{
    if (i >= vec->size)
        return -1;
    else
        return vec->arr[i];
}

int destruct_vector(IntVector* vec)
{
    free(vec->arr);
    return 0;
}


int main(int argc, char* argv[])
{
    IntVector vec;
    initialise_vector(&vec);
    for (int i = 0; i<20; ++i)
        push_back(&vec, i);

    for (int i = 0; i<20; ++i)
        printf("%d\n", at(&vec, i));

    destruct_vector(&vec);

    return 0;
} 
