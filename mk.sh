echo compiling
   as86 -o s.o s.s
   bcc  -c t.c
echo linking
   ld86 -d s.o t.o /usr/lib/bcc/libc.a

#   mount /dev/fd0 /fd0
#   cp a.out /fd0/boot/mtx
#   umount /fd0

echo cp a.out to mtximage/boot/mtx
   mount -o loop /root/dosemu/mtximage /fd0
   cp a.out /fd0/boot/mtx
   umount /fd0
echo done

#  Use YOUR MTX booter of LAB#1 to boot up MTX


