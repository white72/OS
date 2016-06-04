#!/bin/bash

function nuke(){
	printf "\n\n	( ͡° ʖ̯ ͡°)\n\n" 
	kill $loopPid
	kill $prog1Pid
}

trap nuke INT

time ./prog1 &
prog1Pid=$!

(for(( ; ; )); do uptime; sleep 5; done ) &
loopPid=$!

readarray -t pidTab <<< "$(jobs -p)"

wait $prog1Pid

kill $loopPid
