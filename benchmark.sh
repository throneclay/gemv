#!/bin/sh

make -B

for i in $(seq 0 25)
do
size=$(expr 512 + $i \* 512)
echo "size is $size"
./gemv  $size $size 1 10 0 >> sgemv
./gemv  $size $size 1 10 1 >> dgemv
done
