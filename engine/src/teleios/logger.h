#ifndef TELEIOS_LOGGER
#define TELEIOS_LOGGER

#include <stdlib.h>
#include "teleios/types.h"
#include "teleios/diagnostic.h"

typedef enum {
    TL_LOG_LEVEL_FATAL,
    TL_LOG_LEVEL_ERROR,
    TL_LOG_LEVEL_WARN,
    TL_LOG_LEVEL_INFO,
    TL_LOG_LEVEL_DEBUG,
    TL_LOG_LEVEL_TRACE
} TLLogLevel;

TLAPI void tl_logger_console(TLLogLevel level, const char* message, ...);

#define TLFATAL(message, ...) { tl_logger_console(TL_LOG_LEVEL_FATAL, message, ##__VA_ARGS__); TLDIAGNOSTICS_PUSH; TLDIAGNOSTICS_PRINT; exit(99); }
#define TLERROR(message, ...) tl_logger_console(TL_LOG_LEVEL_ERROR, message, ##__VA_ARGS__)
#define TLWARN(message, ...) tl_logger_console(TL_LOG_LEVEL_WARN, message, ##__VA_ARGS__)
#define TLINFO(message, ...) tl_logger_console(TL_LOG_LEVEL_INFO, message, ##__VA_ARGS__)

#if defined(TL_BUILD_ALPHA) || defined(TL_BUILD_BETA)
#   define TLDEBUG(message, ...) tl_logger_console(TL_LOG_LEVEL_DEBUG, message, ##__VA_ARGS__)
#   define TLTRACE(message, ...) tl_logger_console(TL_LOG_LEVEL_TRACE, message, ##__VA_ARGS__)
#else
#   define TLDEBUG(message, ...) 
#   define TLTRACE(message, ...) 
#endif

#endif // TELEIOS_LOGGER
