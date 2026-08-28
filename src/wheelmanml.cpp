#include <windows.h>
#include "config.h"

DWORD WINAPI CustomCodeThread(LPVOID lpParam) {
    MessageBoxA(NULL, "Injected successfully", PROJECT_NAME, MB_OK);
    return 0;
}