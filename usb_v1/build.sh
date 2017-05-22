#!/bin/bash

if [ -f usb ]
then
    echo "清理生成的文件"
    rm -rf ./usb
fi

echo "开始编译"
gcc -g usb.c -o usb -lusb-1.0

if [ -f usb ]
then
    echo "重新编译成功"
else
    echo "重新编译失败."
fi




