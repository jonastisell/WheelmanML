#include <windows.h>
#include <string>

#pragma comment(lib, "user32.lib")

// 1. Definer funksjons-typer basert på de originale XInput-funksjonene
typedef DWORD(WINAPI* tXInputGetCapabilities)(DWORD dwUserIndex, DWORD dwFlags, void* pCapabilities);
typedef DWORD(WINAPI* tXInputGetDSoundAudioDeviceGuids)(DWORD dwUserIndex, GUID* pDSoundRenderGuid, GUID* pDSoundCaptureGuid);
typedef DWORD(WINAPI* tXInputGetState)(DWORD dwUserIndex, void* pState);
typedef DWORD(WINAPI* tXInputSetState)(DWORD dwUserIndex, void* pVibration);

// Globale funksjonspekere
tXInputGetCapabilities oXInputGetCapabilities = nullptr;
tXInputGetDSoundAudioDeviceGuids oXInputGetDSoundAudioDeviceGuids = nullptr;
tXInputGetState oXInputGetState = nullptr;
tXInputSetState oXInputSetState = nullptr;

// 2. Vanlige C-funksjoner (uten __declspec(dllexport) siden .def-filen håndterer dette)
extern "C" DWORD WINAPI XInputGetCapabilities(DWORD dwUserIndex, DWORD dwFlags, void* pCapabilities) {
    if (!oXInputGetCapabilities) return ERROR_DEVICE_NOT_CONNECTED;
    return oXInputGetCapabilities(dwUserIndex, dwFlags, pCapabilities);
}

extern "C" DWORD WINAPI XInputGetDSoundAudioDeviceGuids(DWORD dwUserIndex, GUID* pDSoundRenderGuid, GUID* pDSoundCaptureGuid) {
    if (!oXInputGetDSoundAudioDeviceGuids) return ERROR_DEVICE_NOT_CONNECTED;
    return oXInputGetDSoundAudioDeviceGuids(dwUserIndex, pDSoundRenderGuid, pDSoundCaptureGuid);
}

extern "C" DWORD WINAPI XInputGetState(DWORD dwUserIndex, void* pState) {
    if (!oXInputGetState) return ERROR_DEVICE_NOT_CONNECTED;
    return oXInputGetState(dwUserIndex, pState);
}

extern "C" DWORD WINAPI XInputSetState(DWORD dwUserIndex, void* pVibration) {
    if (!oXInputSetState) return ERROR_DEVICE_NOT_CONNECTED;
    return oXInputSetState(dwUserIndex, pVibration);
}

// 3. Finn og last den omdøpte originale filen
bool InitializeProxy(HMODULE hProxyModule) {
    wchar_t buffer[MAX_PATH];
    GetModuleFileNameW(hProxyModule, buffer, MAX_PATH);
    std::wstring path(buffer);
    
    size_t pos = path.find_last_of(L"\\/");
    if (pos != std::wstring::npos) {
        path = path.substr(0, pos + 1);
    }
    
    // Husket du å gi den originale spilfilen dette navnet?
    path += L"XINPUT9_1_0_original.dll";

    HMODULE hOriginalDll = LoadLibraryW(path.c_str());
    if (!hOriginalDll) {
        return false;
    }

    oXInputGetCapabilities = (tXInputGetCapabilities)GetProcAddress(hOriginalDll, "XInputGetCapabilities");
    oXInputGetDSoundAudioDeviceGuids = (tXInputGetDSoundAudioDeviceGuids)GetProcAddress(hOriginalDll, "XInputGetDSoundAudioDeviceGuids");
    oXInputGetState = (tXInputGetState)GetProcAddress(hOriginalDll, "XInputGetState");
    oXInputSetState = (tXInputSetState)GetProcAddress(hOriginalDll, "XInputSetState");

    return (oXInputGetCapabilities && oXInputGetDSoundAudioDeviceGuids && oXInputGetState && oXInputSetState);
}

DWORD WINAPI CustomCodeThread(LPVOID lpParam) {
    MessageBoxA(NULL, "Proxy og eksport-tabell lastet suksessfullt!", "Suksess", MB_OK);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        
        if (InitializeProxy(hModule)) {
            CreateThread(NULL, 0, CustomCodeThread, NULL, 0, NULL);
        } else {
            MessageBoxA(NULL, "Feil: Sjekk at XINPUT9_1_0_original.dll ligger i samme mappe!", "Proxy Feil", MB_ICONERROR);
            return FALSE; 
        }
    }
    return TRUE;
}
