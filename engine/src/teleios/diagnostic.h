#ifndef TELEIOS_DIAGNOSTIC
#define TELEIOS_DIAGNOSTIC

#include "teleios/types.h"

b8 tl_diagnostic_initialize(void);
b8 tl_diagnostic_terminate(void);

typedef struct {
    const char* function;
    const char* filename;
    u32 linenumber;
} TLDiagnostic;

#if defined(TL_BUILD_ALPHA) || defined(TL_BUILD_BETA)
#define TLDIAGNOSTICS_PUSH \
{ \
    TLDiagnostic diagnostic; \
    diagnostic.filename = __FILE__; \
    diagnostic.linenumber = __LINE__; \
    diagnostic.function = __FUNCTION__; \
    tl_diagnostics_push(&diagnostic); \
}


#define TLDIAGNOSTICS_POP tl_diagnostics_pop()
#define TLDIAGNOSTICS_PRINT tl_diagnostics_print()
#else
#define TLDIAGNOSTICS_PUSH
#define TLDIAGNOSTICS_POP
#define TLDIAGNOSTICS_PRINT
#endif

void tl_diagnostics_push(const TLDiagnostic* diagnostic);
void tl_diagnostics_pop(void);
void tl_diagnostics_print(void);

#endif // TELEIOS_DIAGNOSTIC
