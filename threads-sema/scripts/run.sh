#!/bin/bash


gcc reader-writer-prioritise_writers.c -pthread
./a.out 4 4 10
