#include "teleios/diagnostic/lifecycle.h"
#include "teleios/diagnostic/stacktrace.h"
#include "teleios/platform/memory.h"
#include "teleios/logger.h"

static i16 length = 0;
static TLDiagnostic* registry;

b8 tl_diagnostic_initialize(void) {
    return true;
}

void tl_diagnostics_push(TLDiagnostic* diagnostic) {
    // ########################################################################
    // Create the new array
    // Copy the content from the old to the new
    // ########################################################################
    TLDiagnostic* created = (TLDiagnostic*)tl_platform_memory_halloc((length + 1) * sizeof(TLDiagnostic));
    tl_platform_memory_copy(registry, created, length * sizeof(TLDiagnostic));
    // ########################################################################
    // Release the old array
    // Reassing the pointer to the newly created array
    // ########################################################################
    tl_platform_memory_hfree(registry);
    registry = created;
    // ########################################################################
    // Append the new diagnostic to the array
    // Increase the length counter
    // ########################################################################
    tl_platform_memory_copy((void*)diagnostic, (void*) (registry + length), sizeof(TLDiagnostic));
    length++;

}

void tl_diagnostics_pop(void) {
    TLDiagnostic diagnostic = registry[length - 1];
    // ########################################################################
    // Create the new array
    // Copy the content from the old to the new
    // ########################################################################
    TLDiagnostic* created = (TLDiagnostic*)tl_platform_memory_halloc((length - 1) * sizeof(TLDiagnostic));
    tl_platform_memory_copy(registry, created, (length - 1) * sizeof(TLDiagnostic));
    // ########################################################################
    // Release the old array
    // Reassing the pointer to the newly created array
    // Decrease the length counter
    // ########################################################################
    tl_platform_memory_hfree(registry);
    registry = created;
    length--;
}

void tl_diagnostics_print(void) {
    if (length == 0) return;

    TLERROR("Stacktrace: ");
    for (int i = length - 1; i >=  0 ; --i) {
        TLDiagnostic entry = registry[i];
        TLERROR("  at %s:%d %s", entry.filename, entry.linenumber, entry.function);
    }
}

b8 tl_diagnostic_terminate(void) {
    TLDIAGNOSTICS_PUSH;
    if (registry == NULL) {
        TLDIAGNOSTICS_POP;
        return true;
    }

    TLDIAGNOSTICS_POP;
    tl_platform_memory_hfree(registry);
    registry = NULL;
    length = 0;

    return true;
}