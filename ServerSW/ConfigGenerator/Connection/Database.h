//
// Created by root on 02.03.20.
//

#ifndef CONFIGGENERATOR_DATABASE_H
#define CONFIGGENERATOR_DATABASE_H

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <mysql_connection.h>
#include <list>
#include <mysql_driver.h>
#include "Structures.h"
#include "Logger/Logger.h"

class Database {
public:
    Database(Arguments config);
    UnitInfo getUnitInfo();
    std::list<SensorInfo> getSensorsInfo();

private:
    sql::Driver *driver;
    std::unique_ptr<sql::Connection> con;
    std::unique_ptr<sql::Statement> stmt;
    std::shared_ptr<sql::ResultSet> res;
    bool connected = false;
    Arguments config;

    UnitInfo unitInfo;
    std::list<SensorInfo> sensors;

    void connectToDatabase();
    void performUnitQuery();
    void performSensorsQuery();
    void parseUnitResults();
    void parseSensorResults();
    void disconnectFromDatabase();

    inline std::shared_ptr<sql::ResultSet> getNextStatement(std::shared_ptr<sql::ResultSet> res) {
        if (res && res->next())
            return res;
        else {
            return nullptr;
        }
    }

    inline std::shared_ptr<sql::ResultSet> queryStatementSelect(const std::string &statement) {
        try {
            return std::shared_ptr<sql::ResultSet>(stmt->executeQuery(statement));
        } catch (sql::SQLException &e) {
            std::string errorStr = e.what();
            Logger::logError("Error in function queryStatementSelect: {}" + errorStr);
            return nullptr;
        }
    }

};


#endif //CONFIGGENERATOR_DATABASE_H
