#!/bin/sh

if [ -f f.jpg ]; then
./djpeg -bmp -outfile f.bmp f.jpg
else
   echo f.jpg is missing.
   echo Exiting...
   exit 1
fi

./bmpshow f.bmp
