//
// Created by Jakub Trubka on 27.11.19.
//

#include "Configuration.h"

Config Configuration::loadConfig(std::string configFilePath) {
    Config config;
    INIReader reader(configFilePath);
    try{
        std::string stringBuffer;
        //GENERAL section
        std::stringstream ss;
        ss << std::hex << reader.Get("general", "uid", "UNKNOWN");
        ss >> config.uid;
        config.logPath = reader.Get("general", "log_path", "UNKNOWN");
        config.verbose = reader.GetBoolean("general", "log_console", false);

        //SOURCE section
        stringBuffer = reader.Get("source", "data_source", "UNKNOWN");
        if(stringBuffer == "file"){
            config.dataSource = DATA_SOURCE_FILE;
        }else if(stringBuffer == "spi"){
            config.dataSource = DATA_SOURCE_SPI;
        }else{
            throw std::runtime_error("Wrong data source: " + stringBuffer + "\n");
        }
        config.dataFilePath = reader.Get("source", "file_path", "UNKNOWN");

        //READING section
        config.frequency = reader.GetInteger("reading", "reading_speed",-1);
        config.dataReadyPin = reader.GetInteger("reading", "data_ready_pin", -1);
        config.spiSpeed = reader.GetInteger("reading", "spi_speed", -1);
        config.restartAdcPin = reader.GetInteger("reading", "restart_adc_pin", -1);
        config.raw = reader.GetBoolean("reading", "raw", false);
        config.sdf = reader.GetBoolean("reading", "sdf", false);
        config.rawFrequency = reader.GetInteger("reading", "raw_frequency", -1);
        config.bufferLengthInSeconds = reader.GetInteger("reading", "buffer_length", -1);
        config.adcConstant = reader.GetFloat("reading", "adc_constant", -1.0);

        //SERVER sectiom
        config.udpIp = reader.Get("server", "udp_ip", "UNKNOWN");
        config.udpPort = reader.GetInteger("server", "udp_port", -1);
        config.tcpIp = reader.Get("server", "tcp_ip", "UNKNOWN");
        config.tcpPort = reader.GetInteger("server", "tcp_port", -1);

        //SENSOR section
        config.sensorActive[0] = reader.GetBoolean("sensor", "sensor_0", false);
        config.sensorActive[1] = reader.GetBoolean("sensor", "sensor_1", false);
        config.sensorActive[2] = reader.GetBoolean("sensor", "sensor_2", false);
        config.sensorActive[3] = reader.GetBoolean("sensor", "sensor_3", false);

        config.outputDisablePin[0] = reader.GetInteger("sensor", "sensor_0_output_disable_pin", -1);
        config.outputDisablePin[1] = reader.GetInteger("sensor", "sensor_1_output_disable_pin", -1);
        config.outputDisablePin[2] = reader.GetInteger("sensor", "sensor_2_output_disable_pin", -1);
        config.outputDisablePin[3] = reader.GetInteger("sensor", "sensor_3_output_disable_pin", -1);

        config.sensorType[0] = reader.GetInteger("sensor", "sensor_0_type", -1);
        config.sensorType[1] = reader.GetInteger("sensor", "sensor_1_type", -1);
        config.sensorType[2] = reader.GetInteger("sensor", "sensor_2_type", -1);
        config.sensorType[3] = reader.GetInteger("sensor", "sensor_3_type", -1);

        config.sensorSensitivity[0] = reader.GetFloat("sensor", "sensor_0_sensitivity", -1.0);
        config.sensorSensitivity[1] = reader.GetFloat("sensor", "sensor_1_sensitivity", -1.0);
        config.sensorSensitivity[2] = reader.GetFloat("sensor", "sensor_2_sensitivity", -1.0);
        config.sensorSensitivity[3] = reader.GetFloat("sensor", "sensor_3_sensitivity", -1.0);

        //AGGREGATION section
        config.delta = reader.GetFloat("aggregation", "delta", -1.0);
        config.period = reader.GetInteger("aggregation", "period", -1);
        config.average =reader.GetInteger("aggregation", "average", -1);

/*        std::cout << config.udpIp << " "<< config.udpPort << " "<< config.tcpIp << " "<< config.tcpPort << " "
                << config.adcConstant << " " << config.average << " "<< config.delta << " "<< config.period
                << config.logPath << " "<< config.dataFilePath << " "<< config.frequency<< " "<< config.backupPath
                << " "<< config.dataReadyPin<< " "<< config.spiSpeed<< " "<< config.restartAdcPin<< " "<< config.rawFrequency<< " "<< config.bufferLengthInSeconds  << " "<< " \n";*/

        if(config.logPath == "UNKNOWN" ||
           config.dataFilePath == "UNKNOWN" ||
           config.udpPort == -1 ||
           config.udpIp == "UNKNOWN" ||
           config.tcpPort == -1 ||
           config.tcpIp == "UNKNOWN" ||
           config.frequency == -1 ||
           config.dataReadyPin == -1 ||
           config.spiSpeed == -1 ||
           config.restartAdcPin == -1 ||
           config.rawFrequency == -1 ||
           config.bufferLengthInSeconds == -1 ||
           config.adcConstant == -1.0 ||
           config.delta == -1.0 ||
           config.period == -1 ||
           config.average == -1){
            throw std::runtime_error("Wrong or missing value in config file\n");
        }

        for(int i = 0; i < NUMBER_OF_SENSORS; i++){
            if(config.outputDisablePin[i] == -1 ||
               config.sensorType[i] == -1 ||
               config.sensorSensitivity[i] < 0.0){
                throw std::runtime_error("Wrong or missing value in config file\n");
            }
        }

        if(config.rawFrequency % config.bufferLengthInSeconds){
            throw std::runtime_error("Raw frequency has to be divisible by buffer size\n");
        }

        if(!config.raw && !config.sdf){
            throw std::runtime_error("Raw and sdf reading set to false\n");
        }

        if(!config.sensorActive[0] && !config.sensorActive[1] && !config.sensorActive[2] && !config.sensorActive[3]){
            throw std::runtime_error("All sensors disabled\n");
        }
    }catch(std::exception& e){
        std::cerr << "Log initialization failed: " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
    return config;
}
