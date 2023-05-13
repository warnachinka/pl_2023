#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "graph.h"
#include "logger.h"
#include "ui.h"

class Application {
   public:
    void run(std::string filename);

   private:
    void readData(std::string filename);

    void findFastestCheapest();
    void findCheapest();
    void findLeastTransfers();
    void findLimitCost();
    void findLimitTime();

    enum Flow {
        exit = false,
        fastestCheapest,
        cheapest,
        leastTransfers,
        limitCost,
        limitTime
    };

    Flow selectFlow();
    void start(Flow flow);

    city_t getOrAddCity(std::string city);
    city_t getCity(std::string city);
    bool isValidCity(std::string city);

    std::string readSource();
    std::string readTarget();

    transport_t getOrAddTransport(std::string transport);
    transport_t getTransport(std::string transport);
    std::set<transport_t> getTransports(std::set<std::string> transport);
    bool isValidTransport(std::string transport);

    std::set<std::string> readTransportBlacklist();

    void printRoute(const Route &route);
    void printRoutes(const std::unordered_map<city_t, Route> &routes);

    void printNotFound(std::string what);

    Graph _graph;
    UI _ui;
    Logger _log = Logger("Application");

    std::vector<std::string> _cities, _transport;
    std::unordered_map<std::string, city_t> _citiesIndex, _transportIndex;
};
