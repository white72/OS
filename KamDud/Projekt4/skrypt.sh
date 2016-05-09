#!/bin/bash

printf "\n=======Część wykonywana w całości w Bashu=======\n\n	Czas: 3s\n\n"

for i in {1..16}
do
	if [ "$i" -eq 1 ] 
	then
		printf "	Dla ciągu $i cyfry wystąpień: "
	else

		if [ "$i" -ge 2 -a "$i" -le 9 ]
		then
			printf "	Dla ciągu $i  cyfr wystąpień: "
		else
			printf "	Dla ciągu $i cyfr wystąpień: "
		fi

	fi

	tabA[$i]=$((timeout 3 cat /dev/urandom | tr -dc 'A-Za-z0-9' | fold -w16 | grep "[0-9]\{$i,\}") | wc -l)
	printf "${tabA[$i]}\n"

done

printf "\n==============================================\n\n=====Część z wykorzystaniem programu (C)======\n"

for i in {1..16}
do
	if [ "$i" -eq 1 ] 
	then
		printf "	Dla ciągu $i cyfry wystąpień: "
	else

		if [ "$i" -ge 2 -a "$i" -le 9 ]
		then
			printf "	Dla ciągu $i  cyfr wystąpień: "
		else
			printf "	Dla ciągu $i cyfr wystąpień: "
		fi

	fi

	tabB[$i]=$((timeout 3 ./program.o | grep "[0-9]\{$i,\}") | wc -l)
	printf "${tabB[$i]}\n"
done

printf "\n==============================================\n"

rm -f OUT_BASH.txt
rm -f OUT_C.txt

for i in {1..16}
do
	printf "$i ${tabA[$i]}\n" >> OUT_BASH.txt
	printf "$i ${tabB[$i]}\n" >> OUT_C.txt
done

gnuplot -p -e "set title 'Wykres'; set xlabel 'Ilość cyfr w ciągu'; set ylabel 'Ilość wystąpień'; plot '< cat OUT_BASH.txt' w lines title 'Bash', '< cat OUT_C.txt' w lines title 'C'"
