#!/bin/bash
gcc *.c -Wall -g -o debug.e && gdb debug.e && rm debug.e