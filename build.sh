#!/bin/bash

if [ -f usb ]
then
    rm -rf ./usb 
else
    echo "There is no usb."
fi

gcc -g usb.c -o usb -lusb-1.0



