#! /bin/bash

printf "\n\nCzas dla programu operujacego na watkach:\n"

{ time ./thr.o $1 $2 $3 $4; } #|& grep -i real

printf "\nCzas dla programu operujacego na procesach:\n"

{ time ./pro.o $1 $2 $3 $4; } #|& grep -i real

printf "\n"
