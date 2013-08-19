#!/bin/sh

#Find the Process ID for syncapp running instance

PID=`ps -ef | grep screen | awk '{print $2}'`

if [[ -n "$PID" ]]; then
	kill -9 $PID
	#killall screen 
fi
#killall screen 
