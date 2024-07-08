#include "teleios/diagnostic.h"
#include "teleios/platform.h"
#include "teleios/logger.h"

static u8 max = 0;
static u8 used = 0;
static u8 length = 10;
static TLDiagnostic* registry;

b8 tl_diagnostic_initialize(void) {
    // =================================================================
    // Preallocate 10 positions
    // =================================================================
    registry = (TLDiagnostic*) tl_platform_memory_halloc(length * sizeof(TLDiagnostic));
    return true;
}

void tl_diagnostics_push(const TLDiagnostic* diagnostic) {
    if (used >= length) {
        // =================================================================
        // Create the new array
        // Copy the content from the old to the new
        // =================================================================
        TLDiagnostic* created = (TLDiagnostic*)tl_platform_memory_halloc((length + 10) * sizeof(TLDiagnostic));
        tl_platform_memory_copy(registry, length * sizeof(TLDiagnostic), created);
        // =================================================================
        // Release the old array
        // Reassing the pointer to the newly created array
        // =================================================================
        tl_platform_memory_hfree(registry);
        registry = created;
        length += 10;
    }
    // =================================================================
    // Append the new diagnostic to the array
    // Increase the length counter
    // =================================================================
    tl_platform_memory_copy((void*)diagnostic, sizeof(TLDiagnostic), (void*) (registry + used));
    used++;

    if (used > max) max = used;
}

void tl_diagnostics_pop(void) {
    used--;
    tl_platform_memory_set((void*)(registry + used), sizeof(TLDiagnostic), 0);
}

void tl_diagnostics_print(void) {
    if (used == 0) return;

    TLERROR("Stacktrace: ");
    for (int i = used - 1; i >=  0 ; --i) {
        TLDiagnostic entry = registry[i];
        TLERROR("  at %s:%d %s", entry.filename, entry.linenumber, entry.function);
    }
}

b8 tl_diagnostic_terminate(void) {
    TLTRACE("Diagnostic stack max depth: %d", max);
    if (registry == NULL) { return true; }
    tl_platform_memory_hfree(registry);
    registry = NULL;
    length = used = max = 0;
    return true;
}