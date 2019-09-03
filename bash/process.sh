#!/bin/bash

for f in `ls log*`
do
  echo $f
  ~/sourcecode/tools/bin/best -i $f | awk '{if (NR==1) base = $3; else print $1,$2,base/$3,base,$3}'
done
