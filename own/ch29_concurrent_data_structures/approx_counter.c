#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/syscall.h>
#include <unistd.h>

/* approximate counter using a lock per CPU, which reaches a threshold then
 * updates the global counter. performance is better cus more than one thread
 * can increment at once, but global count not always accurate */

#define NUM_THREADS 4
#define NUMCPUS 4
#define NUM_INCREMENTS 100000

typedef struct __counter_t {
  int global;                     // global count
  pthread_mutex_t glock;          // global lock
  int local[NUMCPUS];             // per-CPU count
  pthread_mutex_t llock[NUMCPUS]; // ... and locks
  int threshold;                  // update frequency
} counter_t;

// init: record threshold, init locks, init values
// of all local counts and global count
void init(counter_t *c, int threshold) {
  c->threshold = threshold;
  c->global = 0;
  pthread_mutex_init(&c->glock, NULL);
  int i;
  for (i = 0; i < NUMCPUS; i++) {
    c->local[i] = 0;
    pthread_mutex_init(&c->llock[i], NULL);
  }
}

// update: usually, just grab local lock and update
// local amount; once local count has risen ’threshold’,
// grab global lock and transfer local values to it
void update(counter_t *c, int threadID, int amt) {
  // printf("threadID: %li\n", syscall(SYS_gettid));
  int cpu = threadID % NUMCPUS;
  pthread_mutex_lock(&c->llock[cpu]);
  c->local[cpu] += amt;
  // for (int i=0; i<NUMCPUS; ++i)
  //     printf("%d ", c->local[i]);
  // printf("\n");
  if (c->local[cpu] >= c->threshold) {
    // transfer to global (assumes amt>0)
    pthread_mutex_lock(&c->glock);
    c->global += c->local[cpu];
    pthread_mutex_unlock(&c->glock);
    c->local[cpu] = 0;
  }
  pthread_mutex_unlock(&c->llock[cpu]);
}

// get: just return global amount (approximate)
int get(counter_t *c) {
  pthread_mutex_lock(&c->glock);
  int val = c->global;
  pthread_mutex_unlock(&c->glock);
  return val; // only approximate!
}


counter_t counter;
void* thread_count_incrementer(void* args)
{
    for (int i=0; i<NUM_INCREMENTS; ++i)
        update(&counter, syscall(SYS_gettid), 1);
    return NULL;
}


typedef struct timespec timespec;


int main(int argc, char *argv[]) {
  init(&counter, 1024);
  timespec ts_start;
  timespec ts_end;

  pthread_t threads[NUM_THREADS];

  clock_gettime(CLOCK_MONOTONIC, &ts_start);
  for (int i = 0; i < NUM_THREADS; ++i)
    pthread_create(&threads[i], NULL, thread_count_incrementer, NULL);

  for (int i = 0; i < NUM_THREADS; ++i)
    pthread_join(threads[i], NULL);
  clock_gettime(CLOCK_MONOTONIC, &ts_end);

  printf("count: %d\n", get(&counter));
  printf("time taken = %li nanoseconds", ts_end.tv_nsec - ts_start.tv_nsec);
  return 0;
}
