#include "teleios/platform/detector.h"

#ifdef TLPLATFORM_WINDOWS
#include <malloc.h>
#include "teleios/platform/console.h"
#include "teleios/platform/windows/w32extern.h"

void tl_platform_console(const char* message) {
    static unsigned char levels[6] = { 64, 4, 6, 2, 1, 8 };
    // ##############################################################################
    // Convert from char* to wchar_t*
    // ##############################################################################
    int length = MultiByteToWideChar(CP_OEMCP, MB_COMPOSITE, message, -1, NULL, 0);
    void* unicode = _malloca(length * sizeof(wchar_t));
    MultiByteToWideChar(CP_OEMCP, MB_COMPOSITE, message, -1, unicode, length);
    // ##############################################################################
    // Change the console color
    // Write the message
    // Rollback the console color
    // ##############################################################################
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hconsole, &csbi);
    SetConsoleTextAttribute(hconsole, levels[3]);
    WriteConsole(hconsole, unicode, (DWORD)length, NULL, NULL);
    SetConsoleTextAttribute(hconsole, csbi.wAttributes);
    // ##############################################################################
    // Destroy the wchar_t temporary object
    // ##############################################################################
    _freea(unicode);
}

#endif // TLPLATFORM_WINDOWS
