#!/bin/bash
gcc -o main main.c -lm

threads=(1 2 3 5 10 20)
repeticoes=(1)
N=(1000 10000 100000 1000000 10000000 100000000)

for r in ${repeticoes[@]}; do
    for n in ${N[@]}; do
        for t in ${threads[@]}; do
            ./main $n $t
        done
    done
done
