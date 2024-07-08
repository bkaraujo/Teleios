#ifndef TELEIOS_FILESYSTEM
#define TELEIOS_FILESYSTEM

#include "teleios/types.h"

typedef struct {
    void* handle;
    const char* path;
    const u64 size;
    const char* string;
} TLFile;

/**
 * Create the object if the file exists.
 * 
 * Populate the fields:
 * TLFile->path
 * TLFile->size
 */
TLFile* tl_filesystem_open(const char* path);

/**
 * Load the entire file content into the object.
 * 
 * Populate the field:
 * TLFile->string
 */
void tl_filesystem_string(TLFile* file);

/**
 * Destroy the object.
 * 
 * Release internal storage
 * Close the file handle
 * Release the object
 */
void tl_filesystem_close(TLFile* file);

#endif // TELEIOS_FILESYSTEM