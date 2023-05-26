#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <assert.h>

/* counter with a single lock, you can vary the number of threads to see
 * changes in performance. Each thread is just incrementing the global count
 * by the same amount. */


#define NUM_THREADS 2
#define NUM_INCREMENTS 1000

typedef struct
{
    int count;
    pthread_mutex_t mut;
} Counter;

Counter counter;

void init_counter(Counter* counter)
{
    counter->count = 0;
    assert(pthread_mutex_init(&counter->mut, NULL) == 0);
}

void increment_counter(Counter* counter)
{
    pthread_mutex_lock(&counter->mut);
    counter->count++;
    pthread_mutex_unlock(&counter->mut);
}

int get_count(Counter* counter)
{
    pthread_mutex_lock(&counter->mut);
    int rc = counter->count++;
    pthread_mutex_unlock(&counter->mut);
    return rc;
}

void* thread_count_incrementer(void* args)
{
    for (int i=0; i<NUM_INCREMENTS; ++i)
        increment_counter(&counter);
    return NULL;
}
    



typedef struct timespec timespec;

int main(int argc, char* argv[])
{
    init_counter(&counter);
    timespec ts_start;
    timespec ts_end;

    pthread_t threads[NUM_THREADS];

    clock_gettime(CLOCK_MONOTONIC, &ts_start);
    for (int i=0; i<NUM_THREADS; ++i)
        pthread_create(&threads[i], NULL, thread_count_incrementer, NULL);

    for (int i=0; i<NUM_THREADS; ++i)
        pthread_join(threads[i], NULL);
    clock_gettime(CLOCK_MONOTONIC, &ts_end);
    
    printf("count: %d\n", get_count(&counter));
    printf("time taken = %li nanoseconds", ts_end.tv_nsec - ts_start.tv_nsec);
    return 0;
}
