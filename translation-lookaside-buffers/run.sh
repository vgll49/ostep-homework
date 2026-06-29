#!/bin/bash



echo "pages,time" > $OUTPUT

pages=1
trials=1000

OUTPUT=data_$trials.csv
while [ $pages -le 4096 ]
do
    echo "Running with $pages pages..."

    result=$(./tlb $pages $trials | awk '{print $1}')

    echo "$pages,$result" >> $OUTPUT

    pages=$((pages * 2))
done

echo "Results written to $OUTPUT"