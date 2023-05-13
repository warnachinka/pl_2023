#include "app.h"

#include <algorithm>
#include <optional>

#include "dijkstra.h"
#include "general.h"
#include "memory_utils.h"

template <typename T, typename I>
static size_t getOrAdd(std::vector<T> &list, std::unordered_map<T, I> &index,
                       const T &value) {
    auto idx = index.find(value);

    if (idx == index.end()) {
        list.push_back(value);

        size_t i = list.size() - 1;
        index.insert({value, i});

        return i;
    } else {
        return idx->second;
    }
}

void Application::run(std::string filename) {
    _log.info("Starting...");

    memory_utils::logPeakUsage();

    timeit(&Application::readData, *this, filename);

    memory_utils::logPeakUsage();

    while (auto flow = selectFlow()) {
        start(flow);
    }

    memory_utils::logPeakUsage();
}

void Application::readData(std::string filename) {
    std::ifstream data(filename);

    if (!data.is_open()) {
        _log.severe("Failed to open data file.");
        return;
    }

    size_t linesCount = 0;

    for (std::string line; std::getline(data, line);) {
        auto hash = std::find(line.begin(), line.end(), '#');
        line.erase(hash, line.end());

        if (line.empty()) {
            continue;
        }

        linesCount++;

        std::string::iterator quotes[6], digits[4];
        uint8_t q = 0, d = 0;

        bool isInString = false;
        bool isInNumber = false;
        for (auto i = line.begin(); i <= line.end(); i++) {
            if ((std::isspace(*i) != 0 && !isInString) || i == line.end()) {
                if (isInNumber) {
                    isInNumber = false;
                    digits[d++] = i;
                }
                continue;
            }

            if (*i == '"') {
                isInString = !isInString;
                quotes[q++] = i + (int)isInString;
            }

            if (q == 6 && std::isdigit(*i) && !isInNumber) {
                isInNumber = true;
                digits[d++] = i;
            }
        }

        std::string sourceStr(quotes[0], quotes[1]),
            targetStr(quotes[2], quotes[3]), transportStr(quotes[4], quotes[5]);

        uint32_t time = std::stoi(std::string(digits[0], digits[1])),
                 fare = std::stoi(std::string(digits[2], digits[3]));

        city_t source = getOrAddCity(sourceStr),
               target = getOrAddCity(targetStr);

        transport_t transport = getOrAddTransport(transportStr);

        _graph.addNode(source, TripWithTarget(target, transport, time, fare));
    }

    _log.info("Proccessed", linesCount, "lines.");
}

void Application::findFastestCheapest() {
    city_t source = getCity(readSource());
    city_t target = getCity(readTarget());

    std::set<transport_t> transportBlacklist =
        getTransports(readTransportBlacklist());

    Dijkstra dijkstra = Dijkstra(_graph, transportBlacklist);

    auto optimalRoute =
        timeit(&Dijkstra::getFastestCheapestRoute, dijkstra, source, target);

    printRoute(optimalRoute);
}

void Application::findCheapest() {
    city_t source = getCity(readSource());
    city_t target = getCity(readTarget());

    std::set<transport_t> transportBlacklist =
        getTransports(readTransportBlacklist());

    Dijkstra dijkstra = Dijkstra(_graph, transportBlacklist);

    auto optimalRoute =
        timeit(&Dijkstra::getCheapestRoute, dijkstra, source, target);

    printRoute(optimalRoute);
}

void Application::findLeastTransfers() {
    city_t source = getCity(readSource());
    city_t target = getCity(readTarget());

    std::set<transport_t> transportBlacklist =
        getTransports(readTransportBlacklist());

    Dijkstra dijkstra = Dijkstra(_graph, transportBlacklist);

    auto optimalRoute =
        timeit(&Dijkstra::getShortestRoute, dijkstra, source, target);

    printRoute(optimalRoute);
}

void Application::findLimitCost() {
    city_t source = getCity(readSource());

    std::set<transport_t> transportBlacklist =
        getTransports(readTransportBlacklist());

    Dijkstra dijkstra = Dijkstra(_graph, transportBlacklist);

    uint32_t limitPrice = _ui.readNumber("Введите ограничение по стоимости.");

    auto optimalRoutes = timeit(&Dijkstra::getCheapestRoutesWithConstraint,
                                dijkstra, source, limitPrice);

    printRoutes(optimalRoutes);
}

void Application::findLimitTime() {
    city_t source = getCity(readSource());

    std::set<transport_t> transportBlacklist =
        getTransports(readTransportBlacklist());

    Dijkstra dijkstra = Dijkstra(_graph, transportBlacklist);

    uint32_t limitTime = _ui.readNumber("Введите ограничение по времени.");

    auto optimalRoutes = timeit(&Dijkstra::getFastestRoutesWithConstraint,
                                dijkstra, source, limitTime);

    printRoutes(optimalRoutes);
}

Application::Flow Application::selectFlow() {
    static const std::array<std::pair<Flow, const char *>, 5> choices = {
        {{Flow::fastestCheapest,
          "Среди кратчайших по времени путей между двумя городами найти путь "
          "минимальной стоимости."},
         {Flow::cheapest,
          "Среди путей между двумя городами найти путь минимальной стоимости."},
         {Flow::leastTransfers,
          "Найти путь между двумя городами, минимальный по числу посещённых "
          "городов."},
         {Flow::limitCost,
          "Найти множество городов, достижимых из города отправления не более "
          "чем за limit_cost денег."},
         {Flow::limitTime,
          "Найти множество городов, достижимых из города отправления не более "
          "чем за limit_time времени."}}};

    return _ui.select(choices, Flow::exit);
}

void Application::start(Flow flow) {
    switch (flow) {
        case Flow::fastestCheapest:
            findFastestCheapest();
            break;
        case Flow::cheapest:
            findCheapest();
            break;
        case Flow::leastTransfers:
            findLeastTransfers();
            break;
        case Flow::limitCost:
            findLimitCost();
            break;
        case Flow::limitTime:
            findLimitTime();
            break;
        default:
            break;
    }
}

city_t Application::getOrAddCity(std::string city) {
    return getOrAdd(_cities, _citiesIndex, city);
}

city_t Application::getCity(std::string city) { return _citiesIndex.at(city); }

bool Application::isValidCity(std::string city) {
    return _citiesIndex.find(city) != _citiesIndex.end();
}

std::string Application::readSource() {
    std::string source;
    while (
        !isValidCity(source = _ui.readString("Введите город отправления."))) {
        printNotFound(source);
    }

    return source;
}

std::string Application::readTarget() {
    std::string target;
    while (!isValidCity(target = _ui.readString("Введите город назначения."))) {
        printNotFound(target);
    }

    return target;
}

transport_t Application::getOrAddTransport(std::string transport) {
    return getOrAdd(_transport, _transportIndex, transport);
}

transport_t Application::getTransport(std::string transport) {
    return _transportIndex.at(transport);
}

std::set<transport_t> Application::getTransports(
    std::set<std::string> transport) {
    std::set<transport_t> result;
    for (auto t : transport) {
        result.insert(getTransport(t));
    }

    return result;
}

bool Application::isValidTransport(std::string transport) {
    return _transportIndex.find(transport) != _transportIndex.end();
}

std::set<std::string> Application::readTransportBlacklist() {
    while (true) {
        auto list =
            _ui.readList("Исключите виды транспорта. Укажите их через пробел.");

        std::optional<std::string> invalidTransport;
        for (std::string t : list) {
            if (!isValidTransport(t)) {
                invalidTransport = std::make_optional(t);
                printNotFound(invalidTransport.value());
                break;
            }
        }

        if (!invalidTransport.has_value()) {
            return std::set(list.begin(), list.end());
        }
    }
}

void Application::printRoute(const Route &route) {
    _ui.printMessage(route.toString(_cities, _transport));
    _log.info(route.toString(_cities, _transport));
}

void Application::printRoutes(const std::unordered_map<city_t, Route> &routes) {
    std::string message;
    for (const auto &[target, route] : routes) {
        message += route.toString(_cities, _transport) + "\n\n";
    }

    _ui.printMessage(message);
    _log.info(message);
}

void Application::printNotFound(std::string what) {
    _ui.printMessage(what + ": not found.");
}
