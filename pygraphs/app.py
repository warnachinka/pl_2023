from enum import Enum
from typing import Set, Dict, List

from logger import Logger
from general import City, Transport
from trip import TripWithTarget
from ui import UI
from graph import Graph
from route import Route
import memory_utils
from dijkstra import Dijkstra

class Application:

    def __init__(self):
        self._log = Logger('Application')
        
        self._cities: List[str] = []
        self._cities_index: Dict[str, City] = {}
        
        self._transport: List[str] = []
        self._transport_index: Dict[str, Transport] = {}

        self._graph: Graph = Graph()

        self._ui: UI = UI()

    def run(self, filename: str):
        self._log.info('Starting...')

        memory_utils.log_peak_usage()

        self._read_data(filename)

        while True:
            flow = self._select_flow()
            if flow == Application.Flow.EXIT:
                break

            self._start(flow)

        memory_utils.log_peak_usage()

    def _read_data(self, filename: str):
        with open(filename, 'r') as data:
            n_lines = 0

            for line in data:
                line = line[:line.find('#')]

                if(len(line) == 0):
                    continue

                n_lines += 1

                quotes: List[int] = []
                digits: List[int] = []

                is_in_word: bool = False
                is_in_number: bool = False
                for i in range(len(line) + 1):
                    char = line[i] if i < len(line) else None

                    if i == len(line) or char.isspace() and is_in_word == False:
                        if is_in_number == True:
                            is_in_number = False
                            digits.append(i)
                        continue

                    if char == '"':
                        is_in_word = not is_in_word
                        quotes.append(i + int(is_in_word))

                    if len(quotes) == 6 and char.isdigit() and is_in_number == False:
                        is_in_number = True
                        digits.append(i)

                source_str = line[quotes[0]:quotes[1]]
                target_str = line[quotes[2]:quotes[3]]
                transport_str = line[quotes[4]:quotes[5]]

                time = int(line[digits[0]:digits[1]])
                fare = int(line[digits[2]:digits[3]])

                source = self._get_or_add_city(source_str)
                dest = self._get_or_add_city(target_str)
                transport = self._get_or_add_transport(transport_str)

                self._log.info(source_str, target_str, transport_str, time, fare)

                self._graph.add_node(source, TripWithTarget(dest, transport, time, fare))

            self._log.info('Processed', n_lines, 'lines.')

    class Flow(Enum):
        EXIT = False
        FASTEST_CHEAPEST = 1
        CHEAPEST = 2
        LEAST_TRANSFERS = 3
        LIMIT_PRICE = 4
        LIMIT_TIME = 5
    
    def _select_flow(self) -> Flow:
        options = [
            (Application.Flow.FASTEST_CHEAPEST, "Среди кратчайших по времени путей между двумя городами найти путь минимальной стоимости."),
            (Application.Flow.CHEAPEST, "Среди путей между двумя городами найти путь минимальной стоимости."),
            (Application.Flow.LEAST_TRANSFERS, "Найти путь между двумя городами, минимальный по числу посещённых городов."),
            (Application.Flow.LIMIT_PRICE, "Найти множество городов, достижимых из города отправления не более чем за limit_cost денег."),
            (Application.Flow.LIMIT_TIME, "Найти множество городов, достижимых из города отправления не более чем за limit_time времени.")
        ]
        return self._ui.select(options, Application.Flow.EXIT)

    def _read_source(self) -> str:
        while self.is_valid_city(source := self._ui.read_string("Введите город отправления.")) == False:
            self._print_not_found(source)
        return source

    def _read_destionation(self) -> str:
        while self.is_valid_city(target := self._ui.read_string("Введите город назначения.")) == False:
            self._print_not_found(target)
        return target

    def _read_transport_blacklist(self) -> Set[str]:
        while True:
            transport = self._ui.read_list("Исключите виды транспорта. Укажите их через пробел.")

            invalid_transport = None
            for t in transport:
                if self.is_valid_transport(t) == False:
                    invalid_transport = t
                    self._print_not_found(t)
                    break
            
            if invalid_transport is None:
                return set(transport)

    def _print_not_found(self, what: str):
        self._ui.print_message(what + ": not found.")

    def _print_route(self, route: Route):
        self._ui.print_message(route.to_string(self._cities, self._transport))
        self._log.info(route.to_string(self._cities, self._transport))

    def _print_routes(self, routes: Dict[City, Route]):
        message = ''
        for (distination, route) in routes.items():
            message += route.to_string(self._cities, self._transport) + '\n\n'

        self._ui.print_message(message)
        self._log.info(message)

    def _start(self, flow: Flow):
        if flow == Application.Flow.FASTEST_CHEAPEST:
            self._find_fastest_cheapest()
        elif flow == Application.Flow.CHEAPEST:
            self._find_cheapest()
        elif flow == Application.Flow.LEAST_TRANSFERS:
            self._find_least_transfers()
        elif flow == Application.Flow.LIMIT_PRICE:
            self._find_with_limit_price()
        elif flow == Application.Flow.LIMIT_TIME:
            self._find_with_limit_time()

    def _find_fastest_cheapest(self):
        source = self._get_city(self._read_source())
        target = self._get_city(self._read_destionation())

        transport_blacklist = self._get_transports(self._read_transport_blacklist())

        dijkstra = Dijkstra(self._graph, transport_blacklist)

        optimal_route = dijkstra.get_fastest_cheapest_route(source, target)

        self._print_route(optimal_route)

    def _find_cheapest(self):
        source = self._get_city(self._read_source())
        target = self._get_city(self._read_destionation())

        transport_blacklist = self._get_transports(self._read_transport_blacklist())

        dijkstra = Dijkstra(self._graph, transport_blacklist)

        optimal_route = dijkstra.get_cheapest_route(source, target)

        self._print_route(optimal_route)

    def _find_least_transfers(self):
        source = self._get_city(self._read_source())
        target = self._get_city(self._read_destionation())

        transport_blacklist = self._get_transports(self._read_transport_blacklist())

        dijkstra = Dijkstra(self._graph, transport_blacklist)

        optimal_route = dijkstra.get_shortest_route(source, target)

        self._print_route(optimal_route)

    def _find_with_limit_price(self):
        source = self._get_city(self._read_source())

        transport_blacklist = self._get_transports(self._read_transport_blacklist())

        dijkstra = Dijkstra(self._graph, transport_blacklist)

        limit_price = self._ui.read_number("Введите ограничение по стоимости.")

        optimal_routes = dijkstra.get_cheapest_routes_with_constraint(source, limit_price)

        self._print_routes(optimal_routes)

    def _find_with_limit_time(self):
        source = self._get_city(self._read_source())

        transport_blacklist = self._get_transports(self._read_transport_blacklist())

        dijkstra = Dijkstra(self._graph, transport_blacklist)

        limit_time = self._ui.read_number("Введите ограничение по времени.")

        optimal_routes = dijkstra.get_fastest_routes_with_constraint(source, limit_time)

        self._print_routes(optimal_routes)

    def _get_or_add_city(self, city: str) -> City:
        if city not in self._cities_index: 
            self._cities.append(city)
            
            i = len(self._cities) - 1
            self._cities_index[city] = i
            
            return i
        else:
            return self._cities_index[city]

    def _get_or_add_transport(self, transport: str) -> Transport:
        if transport not in self._transport_index: 
            self._transport.append(transport)
            
            i = len(self._transport) - 1
            self._transport_index[transport] = i
            
            return i
        else:
            return self._transport_index[transport]

    def _get_city(self, city: str) -> City:
        return self._cities_index[city]

    def _get_transport(self, transport: str) -> Transport:
        return self._transport_index[transport]

    def _get_transports(self, transport: Set[str]) -> Set[Transport]:
        return { self._get_transport(t) for t in transport }

    def is_valid_city(self, city: str) -> bool:
        return city in self._cities_index

    def is_valid_transport(self, transport: str) -> bool:
        return transport in self._transport_index
