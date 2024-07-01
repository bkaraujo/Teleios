#include "teleios/platform/detector.h"

#ifdef TLPLATFORM_WINDOWS
#include "teleios/platform/console.h"
#include "teleios/platform/memory.h"
#include "teleios/platform/windows/w32extern.h"

void tl_platform_console(u8 level, const char* message) {
    static u8 levels[6] = { 64, 4, 6, 2, 1, 8 };
    // ##############################################################################
    // Convert from char* to wchar_t*
    // ##############################################################################
    i32 length = MultiByteToWideChar(CP_OEMCP, MB_COMPOSITE, message, -1, NULL, 0);
    void* unicode = tl_platform_memory_salloc(length * sizeof(wchar_t));
    MultiByteToWideChar(CP_OEMCP, MB_COMPOSITE, message, -1, unicode, length);
    // ##############################################################################
    // Change the console color
    // Write the message
    // Rollback the console color
    // ##############################################################################
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(e_hconsole, &csbi);
    SetConsoleTextAttribute(e_hconsole, levels[level]);
    WriteConsole(e_hconsole, unicode, (DWORD)length, NULL, NULL);
    SetConsoleTextAttribute(e_hconsole, csbi.wAttributes);
    // ##############################################################################
    // Destroy the wchar_t temporary object
    // ##############################################################################
    tl_platform_memory_sfree(unicode);
}

#endif // TLPLATFORM_WINDOWS
