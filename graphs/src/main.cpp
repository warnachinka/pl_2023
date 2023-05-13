#include <fstream>

#include "app.h"
#include "general.h"
#include "logger.h"
#include "memory_utils.h"

int main(int argc, char **argv) {
    setlocale(LC_ALL, "ru_RU.UTF-8");

    if (argc < 2) {
        std::cout
            << "Please provide the file name!\n";
        return 0;
    }

    std::fstream log;
    log.open("log.txt", std::fstream::out);
    Logger::setStream(log);

    Application().run(argv[1]);

    log.close();
    return 0;
}
