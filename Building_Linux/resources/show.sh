#!/bin/sh

if [ -f a.jpg -a -f f.jpg ]; then
./djpeg -bmp -outfile a.bmp a.jpg
./djpeg -bmp -outfile f.bmp f.jpg
else
   echo At least one of input files a.jpg or f.jpg is missing.
   echo Exiting...
   exit 1
fi

count=${1:-10}
while :
do
./h264_user_4p.x &
h264_pid=$!
./bmpshow -sx 340 -sy 280 f.bmp
sleep 0.5
./bmpshow -sx 340 -sy 280 a.bmp
wait $h264_pid

count=`expr $count - 1`
if test $count -eq 0
then
    break
fi
done
