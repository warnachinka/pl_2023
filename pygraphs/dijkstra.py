import heapq
import sys
from typing import Set, Dict, Callable

from graph import Graph
from general import City, Transport
from logger import Logger
from route import Route
from trip import Trip, TripWithTarget


class Dijkstra:
    def __init__(self, graph: Graph, transport_blacklist: Set[Transport]):
        self._graph: Graph = graph
        self._transport_blacklist: Set[Transport] = transport_blacklist
        self._log: Logger = Logger('Dijkstra')

    def get_cheapest_routes(self, source: City) -> Dict[City, Route]:
        return self._get_optimal_routes(source, Dijkstra._get_trip_fare)

    def get_fastest_cheapest_routes(self, source: City) -> Dict[City, Route]:
        return self._get_optimal_routes(source, Dijkstra._get_trip_time_fare)

    def get_fastest_routes(self, source: City) -> Dict[City, Route]:
        return self._get_optimal_routes(source, Dijkstra._get_trip_time)

    def get_shortest_routes(self, source: City) -> Dict[City, Route]:
        return self._get_optimal_routes(source, Dijkstra._count_transfer)

    
    def get_cheapest_routes_with_constraint(self, source: City, constraint: int) -> Dict[City, Route]:
        return self._get_optimal_routes(source, Dijkstra._get_trip_fare, None, constraint)

    def get_fastest_cheapest_routes_with_constraint(self, source: City, constraint: int) -> Dict[City, Route]:
        return self._get_optimal_routes(source, Dijkstra._get_trip_time_fare, None, constraint)

    def get_fastest_routes_with_constraint(self, source: City, constraint: int) -> Dict[City, Route]:
        return self._get_optimal_routes(source, Dijkstra._get_trip_time, None, constraint)

    def get_shortest_routes_with_constraint(self, source: City, constraint: int) -> Dict[City, Route]:
        return self._get_optimal_routes(source, Dijkstra._count_transfer, None, constraint)

    
    def get_cheapest_route(self, source: City, target: City) -> Route:
        return self._get_optimal_routes(source, Dijkstra._get_trip_fare, target)[target]

    def get_fastest_cheapest_route(self, source: City, target: City) -> Route:
        return self._get_optimal_routes(source, Dijkstra._get_trip_time_fare, target)[target]

    def get_fastest_route(self, source: City, target: City) -> Route:
        return self._get_optimal_routes(source, Dijkstra._get_trip_time, target)[target]

    def get_shortest_route(self, source: City, target: City) -> Route:
        return self._get_optimal_routes(source, Dijkstra._count_transfer, target)[target]


    def _get_optimal_routes(self, source: City, weight: Callable, target: City = None, constraint: int = None):
        optimal_routes: Dict[City, Route] = {}

        costs: Dict[City, int] = {v: sys.maxsize for v in self._graph._graph.keys()}
        costs[source] = 0

        queue = [(0, source)]
        heapq.heapify(queue)

        while len(queue) != 0:
            cost, u = heapq.heappop(queue)

            if cost > costs[u]:
                continue

            if target is not None and u == target:
                break

            if constraint is not None and costs[u] > constraint:
                continue

            if u not in optimal_routes:
                optimal_routes[u] = Route(source)

            for (neigh, trips) in self._graph[u].items():
                optimal_trip: Trip = None
                for trip in trips:
                    if trip.get_transport() in self._transport_blacklist:
                        continue

                    alt = costs[u] + weight(trip)
                    if alt < costs[neigh] and (constraint is None or alt <= constraint):
                        costs[neigh] = alt
                        heapq.heappush(queue, (alt, neigh))
                        optimal_trip = trip
                
                if optimal_trip is None:
                    continue

                optimal_routes[neigh] = optimal_routes[u] + TripWithTarget(neigh, optimal_trip.get_transport(), optimal_trip.get_time(), optimal_trip.get_fare())
            
        optimal_routes.pop(source)
        return optimal_routes

    @staticmethod
    def _get_trip_fare(trip: Trip) -> int:
        return trip.get_fare()
    
    @staticmethod
    def _get_trip_time_fare(trip: Trip) -> int:
        return (trip.get_time() << 32) + trip.get_fare()

    @staticmethod
    def _get_trip_time(trip: Trip) -> int:
        return trip.get_time()
    
    @staticmethod
    def _count_transfer(trip: Trip) -> int:
        return 1
