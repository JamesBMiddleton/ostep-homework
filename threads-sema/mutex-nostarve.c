#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "common_threads.h"

//
// Here, you have to write (almost) ALL the code. Oh no!
// How can you show that a thread does not starve
// when attempting to acquire this mutex you build?
//
//  https://github.com/xxyzz/ostep-hw/blob/master/31/mutex-nostarve.c
//  I couldn't work this one out but this implementation makes sense
//  Basically if you have 2 rooms, and without acquiring the actual lock threads
//  can put themselves in room 1. While there's threads waiting in room 1,
//  no thread can obtain the lock, instead another lock required to move from
//  room 1 to room 2 is released and all threads in room 1 move themselves
//  into room 2. Once room 1 is empty we hold the lock for entering room 2, so
//  no more threads can move from room 1 to room 2. We then go through each 
//  thread in room 2, each having a turn with the actual lock, until
//  room 2 is empty, at which point the lock for entering room 2 is released

typedef __ns_mutex_t {
} ns_mutex_t;

void ns_mutex_init(ns_mutex_t *m) {
}

void ns_mutex_acquire(ns_mutex_t *m) {
}

void ns_mutex_release(ns_mutex_t *m) {
}


void *worker(void *arg) {
    return NULL;
}

int main(int argc, char *argv[]) {
    printf("parent: begin\n");
    printf("parent: end\n");
    return 0;
}

