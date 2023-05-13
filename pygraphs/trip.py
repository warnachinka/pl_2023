from general import City, Transport

class Trip:
    def __init__(self, transport: Transport, time: int, fare: int):
        self._transport: Transport = transport
        self._time: int = time
        self._fare: int = fare

    def __str__(self) -> str:
        return f"Cruise(transport={self._transport}, time={self._time}, fare={self._fare})"

    def get_transport(self) -> Transport:
        return self._transport

    def get_time(self) -> int:
        return self._time

    def get_fare(self) -> int:
        return self._fare

    def __gt__(self, other) -> bool:
        if self._transport != other._transport:
            return self._transport > other._transport
        elif self._time != other._time:
            return self._time > other._time
        elif self._fare != other._fare:
            return self._fare > other._fare
        else:
            return False

    def __eq__(self, other) -> bool:
        return self._transport == other._transport and self._time == other._time and self._fare == other._fare

    def __lt__(self, other) -> bool:
        if self._transport != other._transport:
            return self._transport < other._transport
        elif self._time != other._time:
            return self._time < other._time
        elif self._fare != other._fare:
            return self._fare < other._fare
        else:
            return False
        
    def __hash__(self) -> int:
        return hash((self._transport, self._time, self._fare))


class TripWithTarget(Trip):
    def __init__(self, target: City, transport: Transport, time: int, fare: int):
        super().__init__(transport, time, fare)
        self._target: City = target

    def get_target(self) -> City:
        return self._target
    
    def __eq__(self, other) -> bool:
        return super().__eq__(other) and self._target == other._target

    def __hash__(self) -> int:
        return hash((self._target, super()))
