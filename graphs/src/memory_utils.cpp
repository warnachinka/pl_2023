#include "memory_utils.h"

namespace memory_utils {
static const Logger _log = Logger("Memory");

void logPeakUsage() {
    _log.info("Peak memory usage is", getPeakUsage(), "KiB");
}
}  // namespace memory_utils
