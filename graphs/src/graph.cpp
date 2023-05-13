#include "graph.h"

void Graph::addNode(city_t source, TripWithTarget trip) {
    _graph.try_emplace(source);
    _graph.try_emplace(trip.getTarget());

    _graph.at(source).try_emplace(trip.getTarget());
    _graph.at(source).at(trip.getTarget()).insert(trip);
}