#ifndef TELEIOS_FILESYSTEM
#define TELEIOS_FILESYSTEM

#include "teleios/runtime/types.h"

typedef struct {
    void* handle;
    const char* path;
    const u64 size;
    void* payload;
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
 * TLFile->payload
 */
void tl_filesystem_read(TLFile* file);

/**
 * Destroy the object.
 * 
 * Release internal storage
 * Close the file handle
 * Release the object
 */
void tl_filesystem_close(TLFile* file);

/**
 * Assuming that path is a NULL terminated string create a pointer
 * to the last path separator onward.
 */
const char* tl_filesystem_get_filename(const char* path);

#endif // TELEIOS_FILESYSTEM