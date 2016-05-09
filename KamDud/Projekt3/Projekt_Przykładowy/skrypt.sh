#!/bin/bash
#
#Skrypt generujący plik z losowymi liczbami
#

for i in {1..10000}; do od -vAn -N4 -tu < /dev/urandom >> file.txt; done
