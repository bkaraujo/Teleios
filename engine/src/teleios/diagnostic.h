#ifndef TELEIOS_DIAGNOSTIC
#define TELEIOS_DIAGNOSTIC

#include "teleios/types.h"
#include "teleios/logger.h"

b8 tl_diagnostic_initialize(void);
b8 tl_diagnostic_terminate(void);

#if defined(TELEIOS_BUILD_ALPHA) || defined(TELEIOS_BUILD_BETA)
#   define TLDPOP tl_diagnostics_pop()
#   define TLDPRINT tl_diagnostics_print()
#   define TLDPUSH \
    { \
        TLDiagnostic diagnostic; \
        diagnostic.filename = __FILE__; \
        diagnostic.linenumber = __LINE__; \
        diagnostic.function = __FUNCTION__; \
        tl_diagnostics_push(&diagnostic); \
    }
#else
#   define TLDPUSH
#   define TLDPOP
#   define TLDPRINT
#endif // defined(TELEIOS_BUILD_ALPHA) || defined(TELEIOS_BUILD_BETA)

void tl_diagnostics_push(const TLDiagnostic* diagnostic);
void tl_diagnostics_pop(void);
void tl_diagnostics_print(void);
TLDiagnostic* tl_diagnostics_peek(void);

#define TLDRE { TLDPOP; return; }
#define TLDRV(value) { TLDPOP; return value; }

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

#endif // TELEIOS_DIAGNOSTIC
