#!/bin/bash

rm mtximage.bin
cp _mtximage.bin mtximage.bin
echo compiling
as86 -o s.o s.s
bcc  -c t.c
echo linking
ld86 -d s.o t.o /usr/lib/bcc/libc.a
# as86 -o s.o s.s
# bcc  -c main.c		
# ld86 -d s.o main.o /usr/lib/bcc/libc.a	
mount -o loop mtximage.bin fd0	# mount mtximage.bin in fd0 directory, which I created 
cp a.out fd0/boot/mtx
umount fd0
echo done!
