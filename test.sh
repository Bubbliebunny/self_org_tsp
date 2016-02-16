
for i in 3 5 7 9 11 13 15 17 19 21 23 25 27 29 31 33 35 37 39 41 43 45 47 49 51 53 55 57 59 61
do
	for y in 2 100
	do
	./selforg -f $1 -c10000 -n $i -j $y
	done
done
