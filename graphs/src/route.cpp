#include "Route.h"

Route Route::operator+(TripWithTarget tripWithTarget) const {
    std::vector<TripWithTarget> trips = _trips;
    trips.push_back(tripWithTarget);
    return Route(_source, trips);
}
