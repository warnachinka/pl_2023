#include "dijkstra.h"

#include <queue>

std::unordered_map<city_t, Route> Dijkstra::getOptimalRoutes(
    city_t source, uint64_t (*weight)(const Trip &),
    std::optional<city_t> target, std::optional<uint64_t> constraint) const {
    std::unordered_map<city_t, Route> optimalRoutes;

    std::unordered_map<city_t, uint64_t> costs;

    for (const auto &[v, _] : _graph) {
        costs[v] = std::numeric_limits<uint64_t>::max();
    }

    costs[source] = 0;

    using cost_n_city = std::pair<uint64_t, city_t>;
    std::priority_queue<cost_n_city, std::vector<cost_n_city>,
                        std::greater<cost_n_city>>
        queue;
    queue.push({0, source});

    while (!queue.empty()) {
        auto [cost, u] = queue.top();
        queue.pop();

        if (cost > costs[u]) {
            continue;
        }

        if (target.has_value() && u == target.value()) {
            break;
        }

        if (constraint.has_value() && costs[u] > constraint.value()) {
            continue;
        }

        optimalRoutes.try_emplace(u, source);

        for (const auto &[neigh, trips] : _graph[u]) {
            std::optional<Trip> optimalCruise;
            for (const auto &trip : trips) {
                if (_transportblacklist.find(trip.getTransport()) !=
                    _transportblacklist.end()) {
                    continue;
                }

                uint64_t alt = costs[u] + weight(trip);
                if (alt < costs[neigh] &&
                    (!constraint.has_value() || alt <= constraint.value())) {
                    costs[neigh] = alt;
                    queue.push({alt, neigh});
                    optimalCruise = trip;
                }
            }

            if (!optimalCruise.has_value()) {
                continue;
            }

            optimalRoutes[neigh] =
                optimalRoutes[u] + TripWithTarget(neigh, optimalCruise.value());
        }
    }

    optimalRoutes.erase(source);
    return optimalRoutes;
}
