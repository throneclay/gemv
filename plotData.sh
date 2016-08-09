#!/bin/sh
# this file is atim to copy the pkl data to a new one with appropriate tags

source=/Users/zhangshuai20/Documents/XeonPhi_Benchmark
dest=/Users/zhangshuai20/Documents/Statistic/plotData

folders=(ALLtoALL_cache    Hemisphere_flat   Quadrant_hybrid   SNC4_cache
ALLtoALL_flat     Hemisphere_hybrid SNC2_cache        SNC4_flat
ALLtoALL_hybrid   Quadrant_cache    SNC2_flat         SNC4_hybrid
Hemisphere_cache  Quadrant_flat     SNC2_hybrid)

bench=(dgemm hpcg hplinpack linpack sgemm stream)
for j in $(seq 0 5)
do
for i in $(seq 0 14)
do
echo 'test is here '${bench[j]}''
awk '{if($0~_'${bench[j]}') gsub("_'${bench[j]}'","_'${folders[i]}_${bench[j]}'"); print $0;}'  $source/${folders[i]}/RAW/*_${bench[j]}.pkl >> $dest/${folders[i]}_${bench[j]}.pkl
done
micpplot *.pkl
rm *.pkl
done
