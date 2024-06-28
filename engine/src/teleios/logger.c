#include "teleios/logger.h"
#include "teleios/platform/detector.h"

#include "teleios/platform/console.h"

TLAPI void tl_logger_console(TLLogLevel level, const char* message, ...) {
    tl_platform_console(message);
}