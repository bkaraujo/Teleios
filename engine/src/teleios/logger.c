#include "teleios/logger.h"
#include "teleios/platform.h"
#include "teleios/chrono.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#define USER_BUFFERSIZE 3000
#define SYS_BUFFERSIZE 3072
static const char* format = "%04d-%02d-%02d %02d:%02d:%02d,%03d %-5s - %s\n";
static const char* strings[6] = { "FATAL", "ERROR", "WARN", "INFO", "DEBUG", "TRACE" };

TLAPI void tl_logger_console(TLLogLevel level, const char* message, ...) {
    // ##############################################################################
    // 1 - Allocate 3k stack memory for the formatted user message
    // 2 - Parse the message into the allocated memory
    // ##############################################################################
    void* user_message = tl_platform_memory_salloc(USER_BUFFERSIZE);
    tl_platform_memory_set(user_message, USER_BUFFERSIZE, 0);

    va_list ap;
    va_start(ap, message);
    vsnprintf(user_message, USER_BUFFERSIZE, message, ap);
    va_end(ap);
    // ##############################################################################
    // 1 - Allocate 3k stack memory for the formatted final message
    // 2 - Parse the message into the allocated memory
    // 3 - Free the user formatted message
    // ##############################################################################
    TLTime time; tl_chrono_time_now(&time);

    void* logger_message = tl_platform_memory_salloc(SYS_BUFFERSIZE);
    tl_platform_memory_set(logger_message, SYS_BUFFERSIZE, 0);
    sprintf_s(logger_message, SYS_BUFFERSIZE, format, 
        time.year, time.month, time.day,
        time.hour, time.minute, time.seconds, time.millis,
        strings[level], 
        user_message
    );

    tl_platform_memory_sfree(user_message);
    // ##############################################################################
    // 1 - Display the final message in the console
    // 2 - Free the final message
    // ##############################################################################
    tl_platform_console(level, logger_message);
    tl_platform_memory_sfree(logger_message);
}
