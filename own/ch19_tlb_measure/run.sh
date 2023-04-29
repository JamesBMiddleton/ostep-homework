#!/bin/bash

gcc -m64 -O0 main.c  # no compiler optimizations
taskset 01 ./a.out # run on one CPU core
