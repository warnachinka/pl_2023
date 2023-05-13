#include "general.h"

#include <sstream>

#include "logger.h"

static const Logger log = Logger("Timeit");

void __timeit_log_call__(const char *name, std::chrono::milliseconds time,
                         const char *file, int line) {
    std::ostringstream stream;
    stream << name << " took " << time.count() << "ms to execute."
           << "(" << file << ":" << line << ")";

    log.info(stream.str());
}