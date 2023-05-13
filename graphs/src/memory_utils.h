#pragma once

#include <sys/resource.h>

#include <string>

#include "logger.h"

namespace memory_utils {

inline long getPeakUsage() {
    rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return usage.ru_maxrss;
}

void logPeakUsage();
};  // namespace memory_utils