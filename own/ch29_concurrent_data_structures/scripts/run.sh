#!/bin/bash

echo node_lock_llist
gcc node_lock_llist.c -pthread -Wall -g3
./a.out

echo

echo concur_llist
gcc concur_llist.c -pthread -Wall -g3
./a.out
