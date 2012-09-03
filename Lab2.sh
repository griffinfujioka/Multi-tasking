#!/bin/bash

as86 -o s.o s.s
bcc  -c main.c
ld86 -d s.o main.o /usr/lib/bcc/libc.a
mount /dev/fd0 /fd0
cp a.out /fd0/boot/mtx
umount /fd0
