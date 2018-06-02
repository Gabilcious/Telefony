#!/usr/bin/env bash

echo "NEW a" > tmp1
cat $2 >> tmp1
echo "? $3" >> tmp1

echo "NEW a" > tmp2
cat $2 >> tmp2
y=$(cat tmp1 | ./$1)

declare -a tab
a=0
for i in $y; do
	echo " $i ? " >> tmp2
	tab[a]=$i
	((a++))
done

x=$(cat tmp2 | ./$1)
# echo $x
a=0
for i in $x; do
	tab2[a]=$i
	((a++))
done
((a--))
for i in `seq 0 $a`; do
	if [[ ${tab2[i]} == $3 ]]; then
		echo ${tab[i]}
	fi
done

rm tmp1 tmp2
