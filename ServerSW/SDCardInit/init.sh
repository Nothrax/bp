#!/bin/bash
git_http="https://github.com/Nothrax/bp.git"
local_path="/usr/local/bin/UnitSW"
pi_password="0123456789"

#deactivate script
sleep 10
#sudo chmod -x /etc/rc.local
sed -i 's#bash /usr/local/bin/init.sh  2>\&1 | tee /usr/local/bin/install_log.txt#echo "Starting device"#g' /etc/rc.local
sudo apt-get -y update
#SSH 
echo "Configuring ssh"
sudo apt install -y openssh-server
sudo update-rc.d ssh defaults
sudo systemctl enable ssh.socket
sudo systemctl enable ssh.service

#Install curl
sudo apt install -y pkg-config libcurlpp-dev libcurl4-openssl-dev
sudo apt install -y libboost1.62-*
sudo apt install -y pigpio


#Watchdog
sudo modprobe bcm2835_wdt
echo "bcm2835_wdt" | sudo tee -a /etc/modules
sudo apt-get install watchdog
sudo update-rc.d watchdog defaults
sed -i '/#watchdog-device/s/^#//g' /etc/watchdog.conf
sed -i '/#max-load-1[[:space:]]*= 24/s/^#//g' /etc/watchdog.conf
sed -i '/#max-load-5[[:space:]]*= 18/s/^#//g' /etc/watchdog.conf
sed -i '/#max-load-15[[:space:]]*= 12/s/^#//g' /etc/watchdog.conf
sed -i '/#timeout[[:space:]]*6/s/^#//g' /etc/watchdog.conf
echo "watchdog-timeout        = 15" >> /etc/watchdog.conf
echo "retry-timeout        = 15" >> /etc/watchdog.conf

sudo service watchdog start

#Allocation of cores
sed -i 's/rootwait/isolcpus=2,3 rootwait/' /boot/cmdline.txt

#bcm2835
cd /usr/local/bin/
tar xvfz bcm2835-1.60.tar.gz
cd bcm2835-1.60
./configure
make
sudo make install
cd ..
sudo rm -r bcm2835-1.60.tar.gz


#Unit software
sudo apt-get -y install git
sudo apt-get -y install cmake

#UnitSW
cd /usr/local/bin/
sudo mkdir UnitSW
git clone $git_http $local_path

sudo mv /usr/local/bin/UnitSW/bp/UnitSW/* /usr/local/bin/UnitSW/bp/UnitSW/
sudo rm -r /usr/local/bin/UnitSW/bp

sudo mkdir /usr/local/bin/UnitSW/Reader/build
sudo mkdir /usr/local/bin/UnitSW/Aggregation/build

cd /usr/local/bin/UnitSW/Reader/build
cmake ..
make
cd /usr/local/bin/UnitSW/Aggregation/build
cmake ..
make

sudo chmod u+x /usr/local/bin/UnitSW/run.sh
sudo chmod u+x /usr/local/bin/UnitSW/start.sh

#UID setup
uid_new=$(cat /sys/class/net/eth0/address | tr -d ':' | tail -c 7)
uid_new="c0${uid_new}"
echo $uid_new | sudo tee -a /usr/local/bin/UnitSW/uid  > /dev/null




#Config downloader
sudo apt-get -y install jq
chmod +x /usr/local/bin/UnitSW/getconfig.sh
(crontab -l 2>/dev/null; echo "*/5 * * * * /usr/local/bin/UnitSW/getconfig.sh -with args") | crontab -
(crontab -l 2>/dev/null; echo "0 0 * * * sudo systemctl restart UnitSW.service") | crontab -
bash /usr/local/bin/UnitSW/Reader/getconfig.sh

#Scripts as services
sudo echo '[Unit]
Description=Unit service

[Service]
ExecStart=/usr/local/bin/UnitSW/run.sh

[Install]
WantedBy=multi-user.target' >> /lib/systemd/system/UnitSW.service

sleep 5

sudo systemctl daemon-reload

sleep 5

sudo systemctl enable UnitSW.service

#Activate SPI
sudo raspi-config nonint do_spi 0

#filter
echo 0 > /proc/sys/net/ipv4/conf/all/rp_filter
sudo chown -R pi:pi /usr/local/bin/UnitSW/config.ini

find /usr/local/bin/UnitSW/Reader -name "*.c" -type f -delete
find /usr/local/bin/UnitSW/Reader -name "*.h" -type f -delete
find /usr/local/bin/UnitSW/Aggregation -name "*.c" -type f -delete
find /usr/local/bin/UnitSW/Aggregation -name "*.h" -type f -delete
rm -r /usr/local/bin/RTPatch.sh

#new user
sudo usermod -l adminPi pi
sudo usermod -m -d /home/adminPi adminPi
echo -e "$pi_password\n$pi_password" | passwd adminPi

sleep 10

sudo reboot
