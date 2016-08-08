#!/bin/sh

make -B
ITER=20
echo "ITER = $ITER"
echo "==============">>sgemv
echo "==============">>dgemv

for i in $(seq 0 35)
do
size=$(expr 512 + $i \* 512)
echo "size is $size"
./gemv  $size $size 1 $ITER 0 >> sgemv
./gemv  $size $size 1 $ITER 1 >> dgemv
done
