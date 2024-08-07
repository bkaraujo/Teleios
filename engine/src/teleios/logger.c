#include "teleios/platform.h"
#include "teleios/filesystem.h"
#include "teleios/chrono.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#define BUFFERSIZE 4096

static const char* strings[6] = { "FATAL", "ERROR", "WARN", "INFO", "DEBUG", "TRACE" };

TLAPI void tl_logger_console(TLLogLevel level, const char* filename, u32 linenumber, const char* message, ...) {
    // ==================================================================================================================
    // Allocate BUFFERSIZE bytes into the stack
    // ==================================================================================================================
    void* buffer = tl_platform_memory_salloc(TL_MEMORY_ENGINE_LOGGER, BUFFERSIZE);
    tl_platform_memory_set(buffer, BUFFERSIZE, 0);
    // ==================================================================================================================
    // Write the fixed line beginning
    // ==================================================================================================================
    TLCalendar calendar; tl_chrono_calendar_get(&calendar);
    i32 written = sprintf_s(buffer, BUFFERSIZE, "%04d-%02d-%02d %02d:%02d:%02d,%03d %-5s %s:%lu - ", 
        calendar.year, calendar.month, calendar.day,
        calendar.hour, calendar.minute, calendar.seconds, calendar.millis,
        strings[level], 
        tl_filesystem_get_filename(filename),
        linenumber
    );
    // ==================================================================================================================
    // Write the user message
    // ==================================================================================================================
    va_list ap;
    va_start(ap, message);
    vsnprintf(buffer + written, BUFFERSIZE - written, message, ap);
    va_end(ap);
    // ==================================================================================================================
    // Write the new line
    // ==================================================================================================================
    sprintf_s(buffer, BUFFERSIZE, "%s\n", buffer);
    tl_platform_console(level, buffer);
    // ==================================================================================================================
    // Clear the memory
    // Exit if its a FATAL
    // ==================================================================================================================
    tl_platform_memory_sfree(buffer);
    if (level == TL_LOG_LEVEL_FATAL) {
        exit(99);
    }
}
