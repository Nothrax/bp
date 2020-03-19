#!/bin/bash
mkdir /mnt/sdcard
mount /dev/sdc2 /mnt/sdcard

sleep 5

sed -i '3ibash \/usr\/local\/bin\/RTPatch.sh' /mnt/sdcard/etc/rc.local
cp ./RTPatch.sh /mnt/sdcard/usr/local/bin/
cp ./init.sh /mnt/sdcard/usr/local/bin/
cp ./rp3-rt-kernel.tgz /mnt/sdcard/usr/local/bin/
cp ./rp3plus-rt-kernel.tgz /mnt/sdcard/usr/local/bin/
cp ./rp4-rt-kernel.tar.gz /mnt/sdcard/usr/local/bin/
cp ./bcm2835-1.60.tar.gz /mnt/sdcard/usr/local/bin/

sleep 5

umount /mnt/sdcard
rm -r /mnt/sdcard
