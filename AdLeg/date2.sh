#!/bin/bash

##
##	Pobierz [dzień miesiąca] [godzina] [minuta] [sekunda] i porównaj z aktualną datą. Jeśli wpisana data jest:
##		a.) Późniejsza od katualnej to wypiszą ją
##		b.) Wcześniejsza to zrób odliczanie do aktualnej w momencie sprawdzania 
##
##	./date2.sh 9 9 9 9
##

inputDate=$(date -d "$(date +"%Y-%m-")$1 $2:$3:$4" +%s)
now=$(date +%s)

if [ $now -lt $inputDate ]
then
	echo "Wprowadzona data to: $(date -d @$inputDate)"

else
	while [ $now -gt $inputDate ]
	do
		date -d @$inputDate
		inputDate=$(($inputDate+1))
		sleep 1
	done
fi
