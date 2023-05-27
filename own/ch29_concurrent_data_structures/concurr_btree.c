#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <assert.h>

/* potentially buggy single-lock concurrent b-tree. This is a comparison with
 * node lock btree - I've added an artificial pause at each node lookup to
 * show a scenario where the node lock implementation performs better despite
 * the per node lock/unlock overhead */



#define NUM_ELEMS 100000
#define NUM_THREADS 4

typedef struct __node_t
{
    int key;
    struct __node_t* left;
    struct __node_t* right;
} node_t;

typedef struct __btree_t
{
    node_t* root;
    pthread_mutex_t mutex;
} btree_t;


void init(btree_t* tree)
{ 
    tree->root = NULL;
    pthread_mutex_init(&tree->mutex, NULL);
}

node_t* insert(btree_t* tree, const int key)
{
    pthread_mutex_lock(&tree->mutex);
    node_t* new = (node_t*)malloc(sizeof(node_t));
    new->left = new->right = NULL;
    new->key = key;

    node_t* curr = tree->root;
    node_t* next = curr;
    if (tree->root)
    {
        while (next)
        {
            curr = next;
            if (key == curr->key)
                break;
            if (key > curr->key)
                next = curr->right;
            else
                next = curr->left;
        }
        if (key > curr->key)
            curr->right = new;
        else
            curr->left = new;
    }
    else
        tree->root = new;
    pthread_mutex_unlock(&tree->mutex);
    return next;
}

    
node_t* find(btree_t* tree, const int key)
{
    pthread_mutex_lock(&tree->mutex);
    node_t* curr = tree->root;
    while (curr)
    {
        for (int i = 0; i < 1000; ++i) ; //artificial pause
        if (key == curr->key)
            break;
        else if (key > curr->key)
            curr = curr->right;
        else
            curr = curr->left;
    }
    pthread_mutex_unlock(&tree->mutex);
    return curr;
}

btree_t* tree;
int val;

void* test_btree(void* args)
{
    assert(find(tree, val) != 0);
    return NULL;
}


int main(int argc, char* argv[])
{
    srand(time(NULL));
    tree = (btree_t*)malloc(sizeof(btree_t));
    val = rand() % NUM_ELEMS;
    init(tree);

    for (int i = 0; i < NUM_ELEMS; ++i)
        insert(tree, rand() % NUM_ELEMS);
    insert(tree, val);


    struct timespec ts_start;
    struct timespec ts_end;
    pthread_t threads[NUM_THREADS];

    clock_gettime(CLOCK_MONOTONIC, &ts_start);
    for (int i=0; i<NUM_THREADS; ++i)
        pthread_create(&threads[i], NULL, test_btree, NULL);

    for (int i=0; i<NUM_THREADS; ++i)
        pthread_join(threads[i], NULL);
    clock_gettime(CLOCK_MONOTONIC, &ts_end);
    
    printf("time taken = %li seconds, %li nanoseconds", 
            ts_end.tv_sec - ts_start.tv_sec,
            ts_end.tv_nsec - ts_start.tv_nsec);
        
    return 0;
}
