#!/bin/sh

make -B
ITER=100
echo "ITER = $ITER"
echo "==============">>sgemv
echo "==============">>dgemv

for i in $(seq 0 45)
do
size=$(expr 512 + $i \* 512)
echo "size is $size"
./gemv 1 $size $size $ITER 0 >> sgemv
./gemv 1 $size $size $ITER 1 >> dgemv
done
