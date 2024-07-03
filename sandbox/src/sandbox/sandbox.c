#include "teleios/main.h"

void tl_application_configure(TLAppSpecification* spec) {
    spec->window.title = "Sandbox APP";
    spec->window.width = 1024;
    spec->window.height = 768;
}