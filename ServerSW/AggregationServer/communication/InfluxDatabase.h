//
// Created by Jakub Trubka on 05.08.19.
//

#ifndef AGGREGATIONSERVER_INFLUXDATABASE_H
#define AGGREGATIONSERVER_INFLUXDATABASE_H


#include <string>

#include "Message.h"
#include "Structures.h"
#include "spdlog/spdlog.h"

#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Infos.hpp>


class InfluxDatabase {
public:
    static void transferDataToInflux(Message *message, Database database);
};


#endif //AGGREGATIONSERVER_INFLUXDATABASE_H
