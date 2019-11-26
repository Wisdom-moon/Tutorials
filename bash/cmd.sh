#!/bin/bash

SIZE="64 1024 65536 4194304 268435456" 
STREAMS="1 2 4 8 16"

echo "" > log.dat

for sz in $SIZE; do
  for sn in $STREAMS; do
    for i in {0..50}; do
    ./vecadd $sz $sn $sn >> log.dat
    done
  done
done

#echo the firs arguments of this script.
echo $1

