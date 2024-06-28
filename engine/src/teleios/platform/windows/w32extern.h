#ifndef TELEIOS_PLATFORM_WINDOWS
#define TELEIOS_PLATFORM_WINDOWS

#include <Windows.h>

extern HINSTANCE hinstance;
extern HANDLE hconsole;
extern HANDLE heap;
extern LARGE_INTEGER frequency;
extern SYSTEMTIME unix_epoch;

#endif // TELEIOS_PLATFORM_WINDOWS