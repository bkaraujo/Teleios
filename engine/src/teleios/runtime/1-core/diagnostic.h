#ifndef TELEIOS_RUNTIME_CORE_DIAGNOSTIC
#define TELEIOS_RUNTIME_CORE_DIAGNOSTIC

#include "teleios/types.h"
#include "teleios/runtime/core.h"

b8 tl_diagnostic_initialize(void);
b8 tl_diagnostic_terminate(void);
void tl_diagnostics_push(const TLStack* diagnostic);
void tl_diagnostics_pop(void);
void tl_diagnostics_print(void);
TLStack* tl_diagnostics_peek(void);

#if defined(TELEIOS_BUILD_ALPHA) || defined(TELEIOS_BUILD_BETA)
#   define TLDPRINT tl_diagnostics_print()
#   define TLDPUSH \
    { \
        TLStack diagnostic; \
        diagnostic.filename = __FILE__; \
        diagnostic.linenumber = __LINE__; \
        diagnostic.function = __FUNCTION__; \
        tl_diagnostics_push(&diagnostic); \
    }

#   define TLDRE { tl_diagnostics_pop(); return; }
#   define TLDRV(value) { tl_diagnostics_pop(); return value; }
#else
#   define TLDPUSH
#   define TLDPRINT
#   define TLDRE { return; }
#   define TLDRV(value) { return value; }
#endif // defined(TELEIOS_BUILD_ALPHA) || defined(TELEIOS_BUILD_BETA)

#define TLDERE(message) { TLERROR(message); TLDRE; }
#define TLDWRE(message) { TLWARN(message); TLDRE; }
#define TLDIRE(message) { TLINFO(message); TLDRE; }
#define TLDDRE(message) { TLDEBUG(message); TLDRE; }
#define TLDTRE(message) { TLTRACE(message); TLDRE; }

#define TLDERV(message, value) { TLERROR(message); TLDRV(value); }
#define TLDWRV(message, value) { TLWARN (message); TLDRV(value); }
#define TLDIRV(message, value) { TLINFO (message); TLDRV(value); }
#define TLDDRV(message, value) { TLDEBUG(message); TLDRV(value); }
#define TLDTRV(message, value) { TLTRACE(message); TLDRV(value); }

#endif // TELEIOS_RUNTIME_CORE_DIAGNOSTIC
