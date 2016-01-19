#!/bin/sh

count=${1:-10}
while :
do
./h264_user_4p.x &
h264_pid=$!
wait $h264_pid
count=`expr $count - 1`
if test $count -eq 0
then
    break
fi
done
