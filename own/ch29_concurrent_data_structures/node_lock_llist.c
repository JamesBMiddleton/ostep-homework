#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <unistd.h>

#define NUM_THREADS 4
#define NUM_INCREMENTS 100000

typedef struct __node_t
{
    int value;
    struct __node_t* next;
    pthread_mutex_t mut;
} node_t;

typedef struct
{
    node_t* head;
    pthread_mutex_t mut;
} llist_t;


void init(llist_t* list)
{
    list->head = NULL;
    assert(pthread_mutex_init(&list->mut, NULL) == 0);
}

void insert(llist_t* list, const int value)
{
    pthread_mutex_lock(&list->mut);
    node_t* new = (node_t*)malloc(sizeof(node_t));
    assert(new != NULL);
    pthread_mutex_init(&new->mut, NULL);
    new->value = value;
    new->next = list->head;
    list->head = new;
    pthread_mutex_unlock(&list->mut);
}

node_t* find(llist_t* list, const int value)
{
    node_t* n = list->head;
    pthread_mutex_lock(&n->mut);
    while(1)
    {
        if (n->value == value || n->next == NULL)
        {
            pthread_mutex_unlock(&n->mut);
            break;
        }
        for (int i=0; i<100; ++i); // artificially long time on each node
        pthread_mutex_lock(&n->next->mut);
        pthread_mutex_unlock(&n->mut);
        n = n->next;
    }
    return n;
}

llist_t list;

void* find_the_2(void* args)
{
    find(&list, 2);
    return NULL;
}

int main(int argc, char* argv[])
{
    struct timespec ts_start;
    struct timespec ts_end;

    pthread_t threads[NUM_THREADS];

    insert(&list, 2);
    for (int i=0; i<NUM_INCREMENTS; ++i)
        insert(&list, 1);

    clock_gettime(CLOCK_MONOTONIC, &ts_start);
    for (int i=0; i<NUM_THREADS; ++i)
        pthread_create(&threads[i], NULL, find_the_2, NULL);

    for (int i=0; i<NUM_THREADS; ++i)
        pthread_join(threads[i], NULL);
    clock_gettime(CLOCK_MONOTONIC, &ts_end);
    
    printf("time taken = %li seconds, %li nanoseconds", 
            ts_end.tv_sec - ts_start.tv_sec,
            ts_end.tv_nsec - ts_start.tv_nsec);
    return 0;
}
