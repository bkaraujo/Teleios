#include "teleios/runtime/platform.h"
#include "teleios/runtime/core.h"
#include "teleios/filesystem.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#define USER_BUFFERSIZE 3000
#define SYS_BUFFERSIZE 3072
static const char* format = "%04d-%02d-%02d %02d:%02d:%02d,%03d %-5s %s:%lu - %s\n";
static const char* strings[6] = { "FATAL", "ERROR", "WARN", "INFO", "DEBUG", "TRACE" };

TLAPI void tl_logger_console(TLLogLevel level, const char* filename, u32 linenumber, const char* message, ...) {
    // =================================================================
    // 1 - Allocate 3k stack memory for the formatted user message
    // 2 - Parse the message into the allocated memory
    // =================================================================
    void* user_message = tl_platform_memory_salloc(TL_MEMORY_ENGINE_LOGGER, USER_BUFFERSIZE);
    tl_platform_memory_set(user_message, USER_BUFFERSIZE, 0);

    va_list ap;
    va_start(ap, message);
    vsnprintf(user_message, USER_BUFFERSIZE, message, ap);
    va_end(ap);
    // =================================================================
    // 1 - Allocate 3k stack memory for the formatted final message
    // 2 - Parse the message into the allocated memory
    // 3 - Free the user formatted message
    // =================================================================
    TLCalendar calendar;
    tl_chrono_calendar_get(&calendar);

    void* logger_message = tl_platform_memory_salloc(TL_MEMORY_ENGINE_LOGGER, SYS_BUFFERSIZE);
    tl_platform_memory_set(logger_message, SYS_BUFFERSIZE, 0);
    sprintf_s(logger_message, SYS_BUFFERSIZE, format, 
        calendar.year, calendar.month, calendar.day,
        calendar.hour, calendar.minute, calendar.seconds, calendar.millis,
        strings[level], 
        tl_filesystem_get_filename(filename),
        linenumber,
        user_message
    );

    tl_platform_memory_sfree(user_message);
    // =================================================================
    // 1 - Display the final message in the console
    // 2 - Free the final message
    // =================================================================
    tl_platform_console(level, logger_message);
    tl_platform_memory_sfree(logger_message);

    if (level == TL_LOG_LEVEL_FATAL) {
        exit(99);
    }
}
