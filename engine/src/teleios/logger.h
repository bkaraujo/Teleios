#ifndef TELEIOS_LOGGER
#define TELEIOS_LOGGER

#include <stdlib.h>
#include "teleios/types.h"
#include "teleios/diagnostic.h"

TLAPI void tl_logger_console(TLLogLevel level, const char* filename, u32 linenumber, const char* message, ...);

#define TLFATAL(message, ...) { tl_logger_console(TL_LOG_LEVEL_FATAL, __FILE__, __LINE__, message, ##__VA_ARGS__); TLDPUSH; TLDPRINT; exit(99); }
#define TLERROR(message, ...) tl_logger_console(TL_LOG_LEVEL_ERROR, __FILE__, __LINE__, message, ##__VA_ARGS__)
#define TLWARN(message, ...) tl_logger_console(TL_LOG_LEVEL_WARN, __FILE__, __LINE__, message, ##__VA_ARGS__)
#define TLINFO(message, ...) tl_logger_console(TL_LOG_LEVEL_INFO, __FILE__, __LINE__, message, ##__VA_ARGS__)

#if defined(TL_BUILD_ALPHA) || defined(TL_BUILD_BETA)
#   define TLDEBUG(message, ...) tl_logger_console(TL_LOG_LEVEL_DEBUG, __FILE__, __LINE__, message, ##__VA_ARGS__)
#   define TLTRACE(message, ...) tl_logger_console(TL_LOG_LEVEL_TRACE, __FILE__, __LINE__, message, ##__VA_ARGS__)
#else
#   define TLDEBUG(message, ...) 
#   define TLTRACE(message, ...) 
#endif

#endif // TELEIOS_LOGGER
