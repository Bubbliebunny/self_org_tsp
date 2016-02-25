#!/bin/bash 

data_set=(
#a280 
#ali535 
#att48 
#att532 
#berlin52 
#bier127 
#burma14 
#ch130 
#ch150 
#d198 
#d493
#d657 
#d1291 
#d1655 
#d2103 
eil51 
#eil76 
#eil101 
#fl417 
#fl1400 
#fl1577 
#kroA100 
#kroA150 
#kroA200 
#kroB100 
#kroB150 
#kroB200 
#kroC100 
#kroD100 
#kroE100 
#lin105 
#lin318 
#p654 
#pcb442
)

gcc init.c tsplib.c selforg.c draw.c -o selforg -Wall -lm -l SDL2 

for i in "${data_set[@]}" 
do 
	for y in {1..50}
	do
		n=1
		./selforg -f $i -n $n -r 4 -c $y
		while [ $? -eq 0 ]
		do
			 n=`expr $n + 1`
			./selforg -f $i -n $n -r 4 -c $y
		done
 	done
done 

