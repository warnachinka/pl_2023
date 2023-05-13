#pragma once

#include <set>
#include <unordered_map>

#include "trip.h"

class Graph {
   public:
    Graph() = default;

    void addNode(city_t source, TripWithTarget tripWithTarget);

    inline std::unordered_map<city_t, std::set<Trip>> operator[](
        city_t source) const {
        return _graph.at(source);
    }

    inline size_t size() const { return _graph.size(); }

    inline auto cbegin() const { return _graph.cbegin(); }
    inline auto cend() const { return _graph.cend(); }

    inline auto begin() const { return _graph.begin(); }
    inline auto end() const { return _graph.end(); }

    inline auto find(city_t source) const { return _graph.find(source); }

   private:
    std::unordered_map<city_t, std::unordered_map<city_t, std::set<Trip>>>
        _graph;
};
