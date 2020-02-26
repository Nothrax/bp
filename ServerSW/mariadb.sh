#!/bin/sh
sql_path="/var/www/html/sql"
php_version="7.4"
hostname="$(hostname)"
mysql_root_password="bp_mysql"
actual_user="nothrax"


LIST_OF_APPS="
		curl
		libnet-libidn-perl
		php$php_version
		php-cli
		php-curl
		php-fpm
		php-gd
		php-intl
		php-json-schema
		php-ldap
		php-mbstring
		php-mysql
		php-pear
		php-psr-log
		php-soap
		php-xml
		php-xmlrpc
		php-zip
		pkg-php-tools
		unzip
		libdbd-mysql-perl
		mariadb-client-10.3
		mariadb-client-core-10.3
		mariadb-common
		mariadb-server
		mariadb-server-10.3
		mariadb-server-core-10.3
		libnginx-mod-http-geoip
		libnginx-mod-http-image-filter
		libnginx-mod-mail
		libnginx-mod-stream
		nginx
		nginx-common"


sudo apt install software-properties-common
LC_ALL=C.UTF-8 add-apt-repository -y ppa:ondrej/php
sudo apt update

#install packages
apt-get update
apt-get install -y $LIST_OF_APPS

#create dirs for web
mkdir /var/www/html/sql


#get adminer
wget https://github.com/vrana/adminer/releases/download/v4.6.2/adminer-4.6.2.php --directory-prefix=$sql_path 
mv "$sql_path/adminer-4.6.2.php" "$sql_path/index.php"

#add web servers to sites_enabled

echo "server {
    listen 9999;
    server_name sql.cz;
    keepalive_timeout    70;
    root /var/www/html/sql;
    location / {
        include fastcgi_params;
        index index.html index.htm index.php;
        if (-f \$request_filename) {
            break;
        }
    }
    location ~ ^(.+\.php)(.*)$ {
        fastcgi_pass   unix:/run/php/php$php_version-fpm.sock;
        fastcgi_index  index.php;
        fastcgi_param  SCRIPT_FILENAME  \$document_root\$fastcgi_script_name;
        include fastcgi_params;
    }
}" >> /etc/nginx/sites-enabled/default

#edit /etc/hosts
sed -i "s/127.0.1.1	$hostname.*/127.0.1.1	$hostname sql.cz/" /etc/hosts

#edit /etc/nginx/nginx.conf?
sed -i "s/user www-data;/user $actual_user;/" /etc/nginx/nginx.conf
sed -i "s/http {/http { \n\tclient_max_body_size 500M;/" /etc/nginx/nginx.conf

#edit /etc/php/7.2/fpm/pool.d/www.conf
sed -i "s/user = www-data/user = $actual_user/" /etc/php/$php_version/fpm/pool.d/www.conf
sed -i "s/group = www-data/group = $actual_user/" /etc/php/$php_version/fpm/pool.d/www.conf
sed -i "s/listen.owner = www-data/listen.owner = $actual_user/" /etc/php/$php_version/fpm/pool.d/www.conf
sed -i "s/listen.group = www-data/listen.group = $actual_user/" /etc/php/$php_version/fpm/pool.d/www.conf

#edit /etc/php/7.2/fpm/php.ini
sed -i "s/memory_limit = .*/memory_limit = 1024M/" /etc/php/$php_version/fpm/php.ini
sed -i "s/post_max_size = .*/post_max_size = 1024M/" /etc/php/$php_version/fpm/php.ini
sed -i "s/upload_max_filesize = .*/upload_max_filesize = 1024M/" /etc/php/$php_version/fpm/php.ini


#restart services
service nginx reload 
systemctl restart php$php_version-fpm.service  
systemctl restart nginx 
systemctl restart mysql 

#set access rights
chown -R $actual_user:$actual_user /var/www/html

#BLACK MAGIC
#set mysql account
systemctl stop mysql
systemctl stop mariadb
mysqld_safe --skip-grant-tables --skip-networking &

echo "sleeping for 3 seconds"
sleep 3s

mysql -u root -e "UPDATE mysql.user 
  SET authentication_string=PASSWORD('0123'), 
      plugin='mysql_native_password'
WHERE User='root' AND Host='localhost';"

ps -ef | grep sql | grep -v grep | awk '{print $2}' | xargs kill

echo "sleep for 1s"
sleep 1s

systemctl start mariadb

ps -ef | grep sql | grep -v grep | awk '{print $2}' | xargs kill

echo "sleep for 2s"
sleep 2s

systemctl start mariadb

mysql -u root -p0123 -e "CREATE USER 'admin'@'localhost' IDENTIFIED BY 'bp_mysql';
GRANT ALL PRIVILEGES ON *.* TO 'admin'@'localhost';"


