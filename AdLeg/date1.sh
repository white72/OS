#!/bin/bash

##
##	Wyświetl najmłodszy i najstarszy plik z wybranego katalogu [$1]:
##	[nazwa pliku] [właściciel] [typ pliku]
##	Na końcu wyświetl liczbę minut różnicy ich ostatniej modyfikacji.
##
##	=	=	=	=	=	=	=	=	=	=	=	=	=	=	=	=	=
##
##	ls -t => sort by modyfication time, newset first
##	head -n 1 => pierwsza linijka
##	tail -n 1 => ostatnia linijka
##	stat -c=FORMAT:
##		stat -c %U => nazwa właściciela
##		stat -c %Y => data ostatniej modyfikacji (timestamp)
##		stat -c %y => data ostatniej modyfikacji (human-readable)
##		stat -c %F => typ pliku
##

orginalDir=$(pwd)

cd $1

new=$(ls -t $1| head -n 1)
old=$(ls -t $1 | tail -n 1)
timeDelta=$(($(date -r $new +%s) - $(date -r $old +%s)))
timeDeltaM=$(($timeDelta / 60))

stat $new -c '['%n'] ['%U'] ['%F']'
stat $old -c '['%n'] ['%U'] ['%F']'

printf "\nRóżnica czasów ostatnich modyfikacji to $timeDeltaM m.\n\n"

cd $orginalDir
