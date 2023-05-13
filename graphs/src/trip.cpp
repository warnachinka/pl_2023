#include "trip.h"

bool Trip::operator>(const Trip &other) const {
    if (_transport != other._transport) {
        return _transport > other._transport;
    } else if (_time != other._time) {
        return _time > other._time;
    } else if (_fare != other._fare) {
        return _fare > other._fare;
    } else {
        return false;
    }
}

bool Trip::operator==(const Trip &other) const {
    return _transport == other._transport && _time == other._time &&
           _fare == other._fare;
}

bool Trip::operator<(const Trip &other) const {
    if (_transport != other._transport) {
        return _transport < other._transport;
    } else if (_time != other._time) {
        return _time < other._time;
    } else if (_fare != other._fare) {
        return _fare < other._fare;
    } else {
        return false;
    }
}
