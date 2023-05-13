from general import City
from typing import Dict, Set
from trip import Trip, TripWithTarget

class Graph:
    def __init__(self):
        self._graph: Dict[City, Dict[City, Set[Trip]]] = {}

    def add_node(self, source: City, trip: TripWithTarget):
        if source not in self._graph:
            self._graph[source] = {}
        
        if trip.get_target() not in self._graph:
            self._graph[trip.get_target()] = {}

        if trip.get_target() not in self._graph[source]:
            self._graph[source][trip.get_target()] = set()
        
        self._graph[source][trip.get_target()].add(trip)

    def __getitem__(self, source: City) -> Dict[City, Set[Trip]]:
        return self._graph.get(source, None)