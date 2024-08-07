#ifndef TELEIOS_ULID
#define TELEIOS_ULID

#include "teleios/types.h"

TLUlid* tl_ulid_generate(void);
TLUlid* tl_ulid_wrap(const char*);
void tl_ulid_destroy(TLUlid* ulid);
b8 tl_ulid_equals(TLUlid* first, TLUlid* second);

#endif // TELEIOS_ULID