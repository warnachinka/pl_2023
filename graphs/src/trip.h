#pragma once

#include <iostream>
#include <sstream>

#include "general.h"

class Trip {
   public:
    Trip(transport_t transport, uint32_t time, uint32_t fare)
        : _transport(transport), _time(time), _fare(fare) {}

    inline transport_t getTransport() const { return _transport; }
    inline uint32_t getTime() const { return _time; }
    inline uint32_t getFare() const { return _fare; }

    virtual operator const char *() const {
        std::ostringstream stream;
        stream << "Trip(transport=" << _transport << ", time=" << _time
               << ", fare=" << _fare << ")";

        return stream.str().c_str();
    }

    bool operator==(const Trip &other) const;
    bool operator<(const Trip &other) const;
    bool operator>(const Trip &other) const;

   protected:
    transport_t _transport;
    uint32_t _time;
    uint32_t _fare;
};

class TripWithTarget : public Trip {
   public:
    TripWithTarget(city_t target, const Trip &trip)
        : Trip(trip), _target(target) {}

    TripWithTarget(city_t target, transport_t transport, uint32_t time = 0,
                   uint32_t fare = 0)
        : Trip(transport, time, fare), _target(target) {}

    inline city_t getTarget() const { return _target; }

   private:
    city_t _target;
};
