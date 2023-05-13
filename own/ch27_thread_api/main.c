#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h> // all the pthread calls should be wrapped with asserts
                    // this is a clean but unsafe example code...

typedef struct __myarg_t 
{
	int a;
	int b;
} myarg_t;

typedef struct __myret_t 
{
	int x;
	int y;
} myret_t;

pthread_mutex_t lock;
pthread_cond_t cond;
int ready; // global for simplicity... should be passed to threads

void* foo(void* arg)
{
	pthread_mutex_lock(&lock);
	while (ready == 0)
		pthread_cond_wait(&cond, &lock);	
	pthread_mutex_unlock(&lock);

	myarg_t* m = (myarg_t*) arg;
	printf("p1 thread: %d %d\n", m->a, m->b);

	myret_t* r = malloc(sizeof(myret_t));
	r->x = 10;
	r->y = 20;

	return (void*) r;
}

void* bar(void* arg)
{
	printf("p2 thread: I always print first!\n");
 
	pthread_mutex_lock(&lock);
	ready = 1;
	pthread_cond_signal(&cond);
	pthread_mutex_unlock(&lock);

    return NULL;
}

int main(int argc, char* argv[])
{
	pthread_mutex_init(&lock, NULL);
	pthread_cond_init(&cond, NULL);
	ready = 0;

	pthread_t p1, p2;
	myarg_t args = {42, 84};
	pthread_create(&p1, NULL, foo, &args);
	pthread_create(&p2, NULL, bar, NULL);

	myret_t* r;
	pthread_join(p1, (void**) &r);
	pthread_join(p2, NULL);
	printf("main thread: p1 returned %d, %d to main\n", r->x, r->y);
	free(r);	
	return 0;	
}
