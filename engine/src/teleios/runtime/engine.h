#ifndef TELEIOS_ENGINE
#define TELEIOS_ENGINE

#include "teleios/runtime/types.h"

TLAPI b8 tl_engine_pre_initialize(void);
TLAPI b8 tl_engine_initialize(void);
TLAPI b8 tl_engine_configure(TLAppSpecification* specification);
TLAPI b8 tl_engine_run(void);
TLAPI b8 tl_engine_terminate(void);

#endif // TELEIOS_ENGINE