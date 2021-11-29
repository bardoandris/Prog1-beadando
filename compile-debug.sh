#!/bin/bash
gcc *.c -Wall -g -D DEBUG -o  debug.e && gdb debug.e && rm debug.e