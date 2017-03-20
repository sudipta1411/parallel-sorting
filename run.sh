#!/bin/sh

MAX_ITER=12
CUR_ITER=0
NUM=16
while [ "$CUR_ITER" -lt "$MAX_ITER" ]
do
    ./parallel-sorting "$NUM"
    CUR_ITER=`expr $CUR_ITER + 1`
    NUM=`expr 2 \* $NUM`
done
