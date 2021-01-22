#!/bin/bash
program=$1
delay=$2

while true; do
    time1="$(date +%s)"
    $program > output 2> err
    time2="$(date +%s)"
    let delta=$time2-$time1
    let dt=$delay%$delta

    sleep $dt
done