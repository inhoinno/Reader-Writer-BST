#!/bin/bash
echo "*** Run lab2_bst Routine START**"
echo " 	threads [2:16] , count [1000:1000000, *10] "

iter=0
threads=(2 3 4 5 6 7 8 9 10 11 12 13 14 15 16)
count=(1000 10000 100000 1000000)

for i in ${threads[@]};do
	for j in ${count[@]};do
		echo ${i} ${j}
		./lab2_bst -t ${i} -c ${j}
	done
done

echo "************ TEST END ***********"

