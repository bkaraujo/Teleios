#include "teleios/teleios.h"

const char* tl_filesystem_get_filename(const char* path) {
    TLDIAGNOSTICS_PUSH;
    if (path == NULL) { TLDIAGNOSTICS_POP; return NULL; }
    
    u64 length = tl_string_length(path);
    char* filename = (char*)(path + length);

    for (u64 i = length ; i >= 0 ; --i) {
        filename--;
        if (*filename == '\0') continue;
        if (*filename == '/') break;
        if (*filename == '\\') break;
        if (*filename == ':') break;
    }

    filename++;
    TLDIAGNOSTICS_POP;
    return filename;
}