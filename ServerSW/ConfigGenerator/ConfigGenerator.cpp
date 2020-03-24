//
// Created by root on 02.03.20.
//

#include <fstream>
#include "ConfigGenerator.h"
#define SENSOR_TYPE_ACC 173

ConfigGenerator::ConfigGenerator(Arguments arguments){
    this->arguments = arguments;
}

std::string ConfigGenerator::getConfigString() {
    std::string configString;
    configString += "[general]\n";
    configString += "uid=";
    configString += unitInfo.uid + "\n";
    configString += "log_path=";
    configString += unitInfo.logPath + "\n";
    configString += "log_console=";
    configString += unitInfo.logConsole + "\n";
    configString += "\n";

    configString += "[source]\n";
    configString += "data_source=";
    configString += unitInfo.dataSource + "\n";
    configString += "file_path=";
    configString += unitInfo.filePath + "\n";
    configString += "\n";

    configString += "[reading]\n";
    configString += "reading_speed=";
    configString += unitInfo.readingSpeed + "\n";
    configString += "data_ready_pin=";
    configString += unitInfo.dataReadyPin + "\n";
    configString += "spi_speed=";
    configString += unitInfo.spiSpeed + "\n";
    configString += "restart_adc_pin=";
    configString += unitInfo.restartADCPin + "\n";
    configString += "raw=";
    configString += unitInfo.raw + "\n";
    configString += "sdf=";
    configString += unitInfo.aggregated + "\n";
    configString += "raw_frequency=";
    configString += unitInfo.rawFrequency + "\n";
    configString += "buffer_length=";
    configString += unitInfo.bufferLength + "\n";
    configString += "adc_constant=";
    configString += unitInfo.adcConstant + "\n";
    configString += "\n";

    configString += "[aggregation]\n";
    configString += "delta=";
    configString += unitInfo.delta + "\n";
    configString += "period=";
    configString += unitInfo.period + "\n";
    configString += "average=";
    configString += unitInfo.meanAverage + "\n";
    configString += "\n";

    configString += "[server]\n";
    configString += "udp_ip=";
    configString += unitInfo.udpIp + "\n";
    configString += "udp_port=";
    configString += unitInfo.udpPort + "\n";
    configString += "tcp_ip=";
    configString += unitInfo.tcpIp + "\n";
    configString += "tcp_port=";
    configString += unitInfo.tcpPort + "\n";
    configString += "\n";

    configString += "[security]\n";
    configString += "\n";

    configString += "[sensor]\n";
    for(SensorInfo &sensor: sensorsInfo){
        configString += "sensor_" + std::to_string(sensor.sensorIndex) + "=";
        configString += sensor.active + "\n";
        configString += "sensor_" + std::to_string(sensor.sensorIndex) + "_output_disable_pin=";
        configString += sensor.outputDisablePin + "\n";
        configString += "sensor_" + std::to_string(sensor.sensorIndex) + "_sensitivity=";
        if(sensor.sensitivity == "acc"){
            configString += std::to_string(SENSOR_TYPE_ACC) + "\n";
        }
        configString += "sensor_" + std::to_string(sensor.sensorIndex) + "_type=";
        configString += sensor.type + "\n";
    }
    configString += "\n";

    return configString;
}

void ConfigGenerator::generateConfig() {
    auto database = new Database(arguments);
    unitInfo = database->getUnitInfo();
    sensorsInfo = database->getSensorsInfo();
    delete database;

    unitInfo.uid = arguments.uid;
    unitInfo.logPath = "/var/log/";
    unitInfo.logConsole = "false";
    unitInfo.dataSource = "spi";
    unitInfo.filePath = "/usr/local/bin/data";
    unitInfo.dataReadyPin = "22";
    unitInfo.spiSpeed = "19000000";
    unitInfo.restartADCPin = "12";
    unitInfo.udpIp = "192.168.1.16";
    unitInfo.tcpIp = "92.168.1.16";
    unitInfo.tcpPort = "9944";
    unitInfo.udpPort = "9943";

    for(SensorInfo &sensor: sensorsInfo){
        sensor.outputDisablePin = std::to_string(odPins[sensor.sensorIndex]);
    }
}

void ConfigGenerator::saveConfig(std::string path) {
    std::ofstream fileToWrite;
    fileToWrite.open (unitInfo.uid);
    fileToWrite << getConfigString();
    fileToWrite.close();
}
