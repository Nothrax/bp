//
// Created by Jakub Trubka on 05.08.19.
//

#include <iomanip>
#include "InfluxDatabase.h"


void InfluxDatabase::transferDataToInflux(Message *message, Database database) {
    try{
        message->parseMessage();
        std::string writeQuery;
        for(int sensorIndex = 0; sensorIndex < message->getFormattedMessage().numberOfSensors; sensorIndex++){
            if(message->getFormattedMessage().sensor[sensorIndex].dataSize == 0){
                continue;
            }
            for(int j = 0; j < message->getFormattedMessage().sensor[sensorIndex].dataSize; j++){
                std::stringstream offsetSS;
                offsetSS << std::setw(9) << std::setfill('0') << message->getFormattedMessage().sensor[sensorIndex].data[j].offset;

                writeQuery += "sensor_" + std::to_string(sensorIndex) + "_" + "rms_delta"
                              + " value=" + std::to_string(message->getFormattedMessage().sensor[sensorIndex].data[j].value)
                              + " " + std::to_string(message->getFormattedMessage().timestamp) + offsetSS.str() + "\n";
            }
        }
        curlpp::Easy request;
        request.reset();
        std::string url = "http://" + database.db_address + ":8086/write?&db=" + message->getUidString() + "&u=" + database.db_user + "&p=" + database.db_password;
        request.setOpt(new curlpp::options::Url(url));

        //request.setOpt(new curlpp::options::Verbose(true));

        request.setOpt(new curlpp::options::PostFields(writeQuery));
        request.perform();
    }catch(std::exception &e){
        spdlog::get("Aggregation_server")->error(e.what());
    }

}





