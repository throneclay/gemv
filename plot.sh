#!/bin/sh
# this file is aim to compare the different cluster/memory mode in same benchmark.

folders=(ALLtoALL Hemisphere Quadrant SNC2 SNC4)
folder=(cache flat hybrid)

bench=(dgemm hpcg hplinpack linpack sgemm stream)
mkdir -p tmp

for i in $(seq 0 2)
do
for j in $(seq 0 5)
do
echo ${folder[i]}
#micpplot ${folder[i]}_cache/RAW/*_${bench[j]}*pkl ${folder[i]}_flat/RAW/*_${bench[j]}*pkl ${folder[i]}_hybrid/RAW/*_${bench[j]}*pkl
#micpplot ALLtoALL_${folder[i]}/RAW/*_${bench[j]}*pkl Hemisphere_${folder[i]}/RAW/*_${bench[j]}*pkl Quadrant_${folder[i]}/RAW/*_${bench[j]}*pkl SNC2_${folder[i]}/RAW/*_${bench[j]}*pkl SNC4_${folder[i]}/RAW/*_${bench[j]}*pkl
for k in $(seq 0 4)
do
awk '{if($0~_'${bench[j]}') gsub("_'${bench[j]}'","_'${folders[k]}_${bench[j]}'"); print $0;}'  ${folders[k]}_${folder[i]}/RAW/*_${bench[j]}.pkl >> tmp/$k.pkl
done
micpplot tmp/*.pkl
rm tmp/*

done
done
