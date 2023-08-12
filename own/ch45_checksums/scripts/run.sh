#!/bin/bash

gcc check-xor.c -o xor
echo xor:
./xor lorum.txt
./xor ipsum.txt

gcc check-fletcher.c -o fletcher
echo fletcher:
./fletcher lorum.txt
./fletcher ipsum.txt

gcc check-crc.c -o crc
echo scuffed crc:
./crc lorum.txt
./crc ipsum.txt
