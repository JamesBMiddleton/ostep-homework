#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <unistd.h>


/* Regular concurrent linked list - one lock for the whole list. This test
 * just adds a load of 1's then a 2, then find()s the 2, so basically a 
 * convoluted way of traversing the a long list, and we're editing the list so
 * its already thread-safe... but oh well. This is a comparison with the
 * hand-over-hand locking list in node_lock_llist.c, which has a lock per 
 * node_t. The overhead from lock/unlock on every node means usually its not 
 * faster than this basic single lock implementation, however if the time 
 * spent within each node is long, e.g. if instead of find()ing we were 
 * updating the node in some way, then it can be faster. There's an
 * artificially long wait on each node, which make the hand-over-hand locking
 * implementation faster. Why? you're essentially reducing the relative
 * overhead of obtaining the lock - if the node lookup takes 1 second and
 * obtaining the lock takes 10 seconds, its going to take 11 seconds to traverse
 * the list 4 times in parallel with locks, compared to 4 seconds sequentially
 * without locks. If it nows takes 20 seconds for the node lookup and 10 seconds
 * to obtain the lock, it takes 30 seconds in parallel, but 80 seconds
 * sequentially. */

#define NUM_THREADS 4
#define NUM_INCREMENTS 100000

typedef struct __node_t
{
    int value;
    struct __node_t* next;
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
    new->value = value;
    new->next = list->head;
    list->head = new;
    pthread_mutex_unlock(&list->mut);
}

node_t* find(llist_t* list, const int value)
{
    pthread_mutex_lock(&list->mut);
    node_t* n = list->head;
    while(n)
    {
        if (n->value == value)
            break;
        n = n->next;
        for (int i=0; i<100; ++i); // artifically long time on each node
    }
    pthread_mutex_unlock(&list->mut);
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
    for (int i = 0; i < NUM_INCREMENTS; ++i)
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
