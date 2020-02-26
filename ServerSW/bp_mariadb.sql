-- Adminer 4.6.2 MySQL dump

SET NAMES utf8;
SET time_zone = '+00:00';

SET NAMES utf8mb4;

DROP DATABASE IF EXISTS `bp_mariadb`;
CREATE DATABASE `bp_mariadb` /*!40100 DEFAULT CHARACTER SET utf8mb4 */;
USE `bp_mariadb`;

DROP TABLE IF EXISTS `Company`;
CREATE TABLE `Company` (
  `id` int(20) NOT NULL,
  `name` char(50) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;


DROP TABLE IF EXISTS `Component`;
CREATE TABLE `Component` (
  `id` int(10) NOT NULL,
  `machine_serial_number` char(50) NOT NULL,
  `name` char(50) NOT NULL,
  `type` char(50) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `machine_serial_number` (`machine_serial_number`),
  CONSTRAINT `Component_ibfk_1` FOREIGN KEY (`machine_serial_number`) REFERENCES `Machine` (`serial_number`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;


DROP TABLE IF EXISTS `Machine`;
CREATE TABLE `Machine` (
  `serial_number` char(50) NOT NULL,
  `company_id` int(20) NOT NULL,
  `name` char(50) NOT NULL,
  `type` char(50) NOT NULL,
  PRIMARY KEY (`serial_number`),
  KEY `company_id` (`company_id`),
  CONSTRAINT `Machine_ibfk_1` FOREIGN KEY (`company_id`) REFERENCES `Company` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;


DROP TABLE IF EXISTS `Sensor`;
CREATE TABLE `Sensor` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `sensor_index` int(8) NOT NULL,
  `type` char(10) NOT NULL,
  `sensitivity` float NOT NULL,
  `measuring` int(1) NOT NULL,
  `unit_uid` char(10) NOT NULL,
  `component_serial_number` int(10) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `unit_uid` (`unit_uid`),
  KEY `component_serial_number` (`component_serial_number`),
  CONSTRAINT `Sensor_ibfk_1` FOREIGN KEY (`unit_uid`) REFERENCES `Unit` (`uid`) ON DELETE CASCADE,
  CONSTRAINT `Sensor_ibfk_2` FOREIGN KEY (`component_serial_number`) REFERENCES `Component` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8mb4;


DROP TABLE IF EXISTS `Unit`;
CREATE TABLE `Unit` (
  `uid` char(10) NOT NULL,
  `adc_constant` float NOT NULL,
  `reading_speed` int(10) NOT NULL,
  `aggregated_data` int(1) NOT NULL,
  `raw_data` int(1) NOT NULL,
  `delta` float NOT NULL,
  `period` int(10) NOT NULL,
  `mean` int(10) NOT NULL,
  `raw_frequence` int(10) NOT NULL,
  `time_block` int(10) NOT NULL,
  PRIMARY KEY (`uid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;


-- 2020-02-26 12:31:02
