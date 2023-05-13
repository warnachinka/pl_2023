#pragma once

#include <algorithm>
#include <iterator>
#include <limits>
#include <optional>
#include <set>
#include <unordered_map>

#include "general.h"
#include "graph.h"
#include "logger.h"
#include "route.h"
#include "trip.h"

class Dijkstra {
   public:
    Dijkstra(const Graph &graph, const std::set<transport_t> transportBlacklist)
        : _graph(graph), _transportblacklist(transportBlacklist) {}

    inline std::unordered_map<city_t, Route> getFastestCheapestRoutes(
        city_t source) const {
        return getOptimalRoutes(source, getCruiseTimeFare);
    }

    inline std::unordered_map<city_t, Route> getCheapestRoutes(
        city_t source) const {
        return getOptimalRoutes(source, getCruiseFare);
    }

    inline std::unordered_map<city_t, Route> getFastestRoutes(
        city_t source) const {
        return getOptimalRoutes(source, getCruiseTime);
    }

    inline std::unordered_map<city_t, Route> getShortestRoutes(
        city_t source) const {
        return getOptimalRoutes(source, countTransfer);
    }

    inline std::unordered_map<city_t, Route>
    getFastestCheapestRoutesWithConstraint(city_t source,
                                           uint64_t constraint) const {
        return getOptimalRoutes(source, getCruiseTimeFare, std::nullopt,
                                constraint);
    }

    inline std::unordered_map<city_t, Route> getCheapestRoutesWithConstraint(
        city_t source, uint64_t constraint) const {
        return getOptimalRoutes(source, getCruiseFare, std::nullopt,
                                constraint);
    }

    inline std::unordered_map<city_t, Route> getFastestRoutesWithConstraint(
        city_t source, uint64_t constraint) const {
        return getOptimalRoutes(source, getCruiseTime, std::nullopt,
                                constraint);
    }

    inline std::unordered_map<city_t, Route> getShortestRoutesWithConstraint(
        city_t source, uint64_t constraint) const {
        return getOptimalRoutes(source, countTransfer, std::nullopt,
                                constraint);
    }

    inline Route getFastestCheapestRoute(city_t source, city_t target) const {
        return getOptimalRoutes(source, getCruiseTimeFare, target)[target];
    }

    inline Route getCheapestRoute(city_t source, city_t target) const {
        return getOptimalRoutes(source, getCruiseFare, target)[target];
    }

    inline Route getFastestRoute(city_t source, city_t target) const {
        return getOptimalRoutes(source, getCruiseTime, target)[target];
    }

    inline Route getShortestRoute(city_t source, city_t target) const {
        return getOptimalRoutes(source, countTransfer, target)[target];
    }

   private:
    std::unordered_map<city_t, Route> getOptimalRoutes(
        city_t source, uint64_t (*weight)(const Trip &),
        std::optional<city_t> target = std::nullopt,
        std::optional<uint64_t> constraint = std::nullopt) const;

    inline static uint64_t getCruiseFare(const Trip &trip) {
        return trip.getFare();
    }
    inline static uint64_t getCruiseTimeFare(const Trip &trip) {
        return ((uint64_t)trip.getTime() << 32) + trip.getFare();
    }
    inline static uint64_t getCruiseTime(const Trip &trip) {
        return trip.getTime();
    }
    constexpr static uint64_t countTransfer(const Trip &trip) { return 1U; }

    const Graph &_graph;
    const Logger _log = Logger("Dijkstra");

    const std::set<transport_t> _transportblacklist;
};
