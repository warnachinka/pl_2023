from general import City
from trip import TripWithTarget
from typing import List


class Route:
    def __init__(self, source: City, trips: List[TripWithTarget] = []):
        self._source: City = source
        self._trips: List[TripWithTarget] = trips

        time, price, n_transfers = 0, 0, 0
        for trip in trips:
            time += trip.get_time()
            price += trip.get_fare()
            n_transfers += 1

        self._time: int = time
        self._price: int = price
        self._n_transfers: int = n_transfers

    def __str__(self):
        return "Route(" + sum([f" -> {trip.get_target()}/{trip}" for trip in self._trips]) + ")"

    def to_string(self, cities: List[str], transport: List[str]):
        return cities[self._source] + "".join([f" -- {transport[trip.get_transport()]}, {trip.get_time()} ч., {trip.get_fare()}$ ->\n{cities[trip.get_target()]}" for trip in self._trips])

    def __add__(self, trip_with_target: TripWithTarget):
        return Route(self._source, self._trips + [trip_with_target])

    def __getitem__(self, i: int) -> TripWithTarget:
        return self._trips[i]

    def __len__(self) -> int:
        return len(self._trips)

    def get_n_transfers(self):
        return self._n_transfers

    def get_time(self):
        return self._time

    def get_price(self):
        return self._price

    def get_source(self):
        return self._source
