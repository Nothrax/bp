#!/bin/bash
revision=$(awk '/^Revision/ {sub("^1000", "", $3); print $3}' /proc/cpuinfo)

#deactivate start script
sleep 10
sed -i 's#bash /usr/local/bin/RTPatch.sh#bash /usr/local/bin/init.sh  2>\&1 | tee /usr/local/bin/install_log.txt#g' /etc/rc.local
#RT kernel patch
sleep 10

if [ "$revision" = "a02082" ] || [ "$revision" = "a22082" ] || [ "$revision" = "a32082" ]; then
	echo "Raspberry 3b rt patch"

	sudo rm -r /usr/local/bin/rp3plus-rt-kernel.tgz
	sudo rm -r /usr/local/bin/rp4-rt-kernel.tar.gz

	sudo rm -r /boot/overlays/
	sudo rm -r /lib/firmware/
	cd /tmp
	mv /usr/local/bin/rp3-rt-kernel.tgz /tmp/
	tar xzf rp3-rt-kernel.tgz
	cd boot
	sudo cp -rd * /boot/
	cd ../lib
	sudo cp -dr * /lib/
	sudo /sbin/reboot
fi
if [ "$revision" = "a020d3" ];then
	echo "Raspberry 3b+ rt patch"

	sudo rm -r /usr/local/bin/rp3-rt-kernel.tgz
	sudo rm -r /usr/local/bin/rp4-rt-kernel.tar.gz

	cd /tmp 
	mv /usr/local/bin/rp3plus-rt-kernel.tgz /tmp/
	tar xzf rp3plus-rt-kernel.tgz 
	cd boot/ 
	sudo cp -rd * /boot/ 
	cd ../lib 
	sudo cp -dr * /lib/ 
	cd ../overlays 
	sudo cp -d * /boot/overlays 
	cd .. 
	sudo cp -d bcm* /boot/ 
	sudo sh -c "echo ' kernel=vmlinuz-4.14.52-rt34-v7+' >> /boot/config.txt" 
	sudo reboot 
fi

if [ "$revision" = "a03111" ] || [ "$revision" = "b03111" ] || [ "$revision" = "c03111" ]; then
	echo "Raspberry 4 rt patch"
	sudo rm -r /usr/local/bin/rp3plus-rt-kernel.tgz
	sudo rm -r /usr/local/bin/rp3-rt-kernel.tgz
	
	cd /
	mv /usr/local/bin/rp4-rt-kernel.tar.gz /
	sudo tar xzf rp4-rt-kernel.tar.gz 
	cd boot/ 
	sudo reboot 
fi
