#ifndef TELEIOS_ULID
#define TELEIOS_ULID

#include "teleios/types.h"

const char* tl_ulid_generate(void);
void tl_ulid_destroy(const char* ulid);

#endif // TELEIOS_ULID