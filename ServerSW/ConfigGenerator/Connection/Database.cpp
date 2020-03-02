//
// Created by root on 02.03.20.
//

#include "Database.h"

Database::Database(Arguments config){
    this->config = config;
}


void Database::connectToDatabase() {
    try {
        if(!connected) {
            /* Create a connection */
            driver = get_driver_instance();
            con = std::unique_ptr<sql::Connection>(driver->connect(config.db_address, config.db_user, config.db_password));
            /* Connect to the MySQL test database */
            con->setSchema(config.db_database);
            stmt = std::unique_ptr<sql::Statement>(con->createStatement());

            connected = true;
        }
    } catch (sql::SQLException &e) {
        Logger::logError("SQLException in " + std::string(__FILE__)
        + " (" + std::string(__FUNCTION__)
        + ") on line " + std::to_string(__LINE__) + ", "
        + e.what() + " (MySQL error code: " + std::to_string(e.getErrorCode()) + ", SQLSstate " + e.getSQLState());

        throw std::runtime_error("Unable to connectToDatabase");
    }
}

void Database::disconnectFromDatabase() {
    try{
        if(connected) {
            con->close();
            driver->threadEnd();

            res.reset();
            stmt.reset();
            con.reset();
            connected = false;
        }
    } catch (sql::SQLException &e){
        Logger::logError("Failed To Close Connection to DataBase Server " + std::string(e.what()));
    }

}

UnitInfo Database::getUnitInfo() {
    connectToDatabase();
    performUnitQuery();
    parseUnitResults();
    disconnectFromDatabase();
    return unitInfo;
}

std::list<SensorInfo> Database::getSensorsInfo() {
    connectToDatabase();
    performSensorsQuery();
    parseSensorResults();
    disconnectFromDatabase();
    return sensors;
}

void Database::parseSensorResults() {
    if(res == nullptr){
        Logger::logWarning("No sensor on unit with uid " + config.uid);
    }
    while((res = getNextStatement(res)) != nullptr){
        SensorInfo sensorInfo;
        sensorInfo.sensorIndex = res->getInt("sensor_index");
        bool active = res->getInt("measuring");
        if(active){
            sensorInfo.active = "true";
        }else{
            sensorInfo.active = "false";
        }
        sensorInfo.type = res->getString("type");
        sensorInfo.sensitivity = std::to_string(res->getDouble("sensitivity"));
        sensors.push_back(sensorInfo);
    }
}

void Database::parseUnitResults() {
    if(res == nullptr){
        Logger::logError("No unit with uid " + config.uid);
    }

    while((res = getNextStatement(res)) != nullptr){
        unitInfo.readingSpeed = std::to_string(res->getInt("reading_speed"));
        bool raw = res->getInt("raw_data");
        if(raw){
            unitInfo.raw = "true";
        }else{
            unitInfo.raw = "false";
        }

        bool aggregated = res->getInt("aggregated_data");
        if(aggregated){
            unitInfo.aggregated = "true";
        }else{
            unitInfo.aggregated = "false";
        }
        unitInfo.rawFrequency = std::to_string(res->getInt("raw_frequence"));
        unitInfo.bufferLength = std::to_string(res->getInt("time_block"));
        unitInfo.adcConstant = std::to_string(res->getDouble("adc_constant"));
        unitInfo.delta = std::to_string(res->getDouble("delta"));
        unitInfo.period = std::to_string(res->getDouble("period"));
        unitInfo.meanAverage = std::to_string(res->getDouble("mean"));
    }
}

void Database::performSensorsQuery() {
    std::string query = "SELECT * FROM Sensor WHERE unit_uid = " + config.uid;
    res = queryStatementSelect(query);
}

void Database::performUnitQuery() {
    std::string query = "SELECT * FROM Unit WHERE uid = " + config.uid;
    res = queryStatementSelect(query);
}

