#include "teleios/main.h"

void tl_application_configure(TLAppSpecification* spec) {
    spec->rootfs = "X:/c/Teleios/sandbox/assets";
    
    spec->window.title = "SRzé";
    spec->window.width = 1024;
    spec->window.height = 768;

    spec->graphics.clear_color.r = 0.75f;
    spec->graphics.clear_color.g = 0.13f;
    spec->graphics.clear_color.b = 0.75f;
    spec->graphics.clear_color.a = 1.00f;
}