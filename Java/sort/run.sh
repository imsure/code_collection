#!/bin/sh

inputs=$(ls *.txt)

for input in $inputs
do
    java -jar MySort.jar hqim $input
done

sorted=$(ls ./output/*.sorted)
for file in $sorted
do
    if cmp -s "$file" "ds_sorted_asc.txt" > /dev/null; then
	echo "Sorted file " $file "is correct!"
    else
	echo "Sorted file " $file "is not correct!"
    fi
done