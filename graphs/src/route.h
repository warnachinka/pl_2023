#pragma once

#include <iostream>
#include <sstream>
#include <vector>

#include "trip.h"

class Route {
   public:
    Route() = default;

    Route(city_t source, std::vector<TripWithTarget> trips = {})
        : _source(source), _trips(trips) {
        uint64_t time = 0, price = 0, transfers = 0;
        for (auto trip : trips) {
            time += trip.getTime();
            price += trip.getFare();
            transfers += 1;
        }
        _time = time;
        _price = price;
        _transfersCount = transfers;
    }

    inline std::string toString(
        const std::vector<std::string> &cities,
        const std::vector<std::string> &transport) const {
        std::ostringstream stream;
        stream << cities[_source];
        for (const auto &trip : _trips) {
            stream << " -- " << transport[trip.getTransport()] << ", "
                   << trip.getTime() << " ч."
                   << ", " << trip.getFare() << "$"
                   << " ->\n"
                   << cities[trip.getTarget()];
        }

        return stream.str();
    }

    operator const char *() const {
        std::ostringstream stream;
        stream << "Route(" << _source;
        for (const auto &trip : _trips) {
            stream << " -> " << trip.getTarget() << "/" << trip;
        }
        stream << ")";

        return stream.str().c_str();
    }

    Route operator+(TripWithTarget tripWithTarget) const;

    inline TripWithTarget operator[](size_t i) const { return _trips.at(i); };

    inline size_t getTransfersCount() const { return _transfersCount; }

    inline uint64_t getTime() const { return _time; }

    inline uint64_t getPrice() const { return _price; }

    inline city_t getSource() const { return _source; }

   private:
    city_t _source;
    uint64_t _time, _price;
    size_t _transfersCount;

    std::vector<TripWithTarget> _trips;
};
