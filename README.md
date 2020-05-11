#Bakalářská práce
Téma: Zpracování velkých dat v oblasti průmyslu 4.0\
Autor: Jakub Trubka (xtrubk00)\
Vedoucí: doc.RNDr.Pavel Smrž,Ph.D.\
##Obsah disku a popis
##/
###config
Konfigurační soubor vygenerovaný při testech programem pro generování konfiguračního souboru z relační databáze.
###data.csv
Soubor s hodnotami použitými při testu v sekci 7.6 (detekce překročení limitu historickými daty).
###install_log.txt
Soubor vytvořený při testu 7.1. (Test instalačních skriptů a kompletní funkce systému) obsahuje terminálový výstup automatické instalace jednotky.
###plakat.pdf a plakat.odp
Vytvořený plakát k poslednímu bodu zadání ve formátu pdf a odp.
###sslcert.pem
SSL certifikát využitý při testech zabezpečení TCP spojení.
###bp.pdf
Soubor pdf obsahující text práce.

##/BP_latex/
Složka obsahuje zdrojové kódy latexu k textu bakalářské práce.

##/ServerSW/
Složka obsahuje programy a skripty využívané serverovou stranou. Potřebné knihovny pro všechny programy lze nainstalovat pomocí:
```bash
sudo apt install -y libzip-dev cmake git libmysqlcppconn-dev pkg-config libcurlpp-dev libcurl4-openssl-dev libboost-all-dev libssl-dev
```
Programu se překládájí se ze složky programu příkazy:
```bash
mkdir build
cd build
cmake ..
make
```
###/ServerSW/AggregationServer/
Serverová aplikace pro příjem UDP zpráv. Konfigurační soubor umístěný v místě překladu nastavuje přihlašovací údaje do databáze, port a výpis do konzole.
Program se spouští příkazem:
```bash
./AggregationServer
```

###/ServerSW/RawServer/
Serverová aplikace pro příjem TCP zpráv. Program se spouští příkazem:
```bash
./RawServer -v -p <port>
```
Kde `-v` zapíná do konzole a `-p <port>` určuje port serveru.

###/ServerSW/ConfigGenerator/
Aplikace sloužící pro vygenerování konfiguračního souboru jednotky z mariaDB databáze. Spouští se příkazem:
```bash
./ConfigGenerator uid
```
Kde `uid` je unikátní identifikátor jednotky pro kterou je určen konfigurační soubor. Program obsahuje soubor config který obsahuje přihlašovací údaje do databáze.

###/ServerSW/SDCardInit/
Složka obsahuje instalační skript jednotky. Na SD kartě je potřeba vytvořit obraz operačního systému raspbian-lite a poté spustit skript 'DiskCreator.sh' (ve kterém je potřeba změnit device). Skript nakopíruje zbylé skripty na sd kartu a ta se při prvním spuštení inicializuje jednotku.

###/ServerSW/mariadb.sh
Skript který provede instalaci a inicializaci relační databáze mariaDB.

###/ServerSW/bp_mariadb.sql
Databáze využitá v práci.

###/ServerSW/privileges.sh
Skript přidávající práva uživatelům na nově vytvořené databáze v influxDB.

##/UnitSW/
Složka obsahuje programy a skripty využívané jednotkou. Tyto programy jsou z gitu instalovány inicializačním skriptem a jsou přeložitelné a spustitelné pouze na Raspberry Pi 3b s rozšiřující deskou Chipmunk od společnosti 4dot.
 
###/UnitSW/Aggregation a /UnitSW/Reader
Programy jsou popsány v práci.

###/UnitSW/config.ini
Vzorový konfigurační soubor.

###/UnitSW/config_header.ini
Soubor sloužící k porovnání hlavičky HTTP dotazu na konfigurační soubor.

###/UnitSW/getconfig.sh
Skript pro automatické stahování konfiguračního souboru ze serveru.

###/UnitSW/run.sh a /UnitSW/start.sh 
SLouží pro spouštění čtecího programu. 

##/Testing/
Složka obsahuje soubory a programy využité v testech. Jejich funkcionalita s spouštění je popsáno v práci.

###/Testing/CSVimport/
Program slouží pro importování dat z csv souboru do databáze.

###/Testing/DataCompress/
Program slouží k testování kompresních algoritmů.

###/Testing/Encoder/
Program slouží k testování zabezpečení UDP a TCP spojení.

###/Testing/MessageGenerator/
Program pro testování zatížení UPD a TCP serveru.

###/Testing/output/
Složka obsahuje výstupy zátěžových testů.

###/Testing/3MB.data 10MB.data a 500kB.data
Soubory využité v kompresních testech

###/Testing/compress.sh
Testovací skript pro určité kompresní algoritmy.

###/Testing/createTestDatabases.sh
Skript pro vytváření datových databází v influxDB pro účely testů.

###/Testing/evaluateLoadTest.sh
Skript pro vyhodnocení zátěžových testů.
