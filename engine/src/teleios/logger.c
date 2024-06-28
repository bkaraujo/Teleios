#include "teleios/logger.h"
#include "teleios/platform/detector.h"

#include "stdio.h"

TLAPI void tl_logger_console(TLLogLevel level, const char* message, ...) {
    printf("%s\n", message);
}