#include <iostream>
#include <Windows.h>
#include <string>

bool copyToClipboard(const std::wstring& str) {
    if (!OpenClipboard(nullptr)) {
        return false;
    }

    HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, (str.size() + 1) * sizeof(wchar_t));
    if (!hGlobal) {
        CloseClipboard();
        return false;
    }

    wchar_t* pGlobal = static_cast<wchar_t*>(GlobalLock(hGlobal));
    if (!pGlobal) {
        GlobalFree(hGlobal);
        CloseClipboard();
        return false;
    }

    wcscpy_s(pGlobal, str.size() + 1, str.c_str());
    GlobalUnlock(hGlobal);

    EmptyClipboard();
    SetClipboardData(CF_UNICODETEXT, hGlobal);
    CloseClipboard();
    return true;
}

std::wstring getHWIDFromRegistry() {
    HKEY hKey;
    std::wstring hwid;

 
    if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"Software\\Microsoft\\Cryptography", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        wchar_t buffer[256];
        DWORD bufferSize = sizeof(buffer);

   
        if (RegQueryValueExW(hKey, L"MachineGuid", nullptr, nullptr, reinterpret_cast<LPBYTE>(buffer), &bufferSize) == ERROR_SUCCESS) {
            hwid = buffer;
        }

        RegCloseKey(hKey);
    }

    return hwid;
}

int wmain() {
    std::wstring hwid = getHWIDFromRegistry();

    if (!hwid.empty()) {
        copyToClipboard(hwid);

        std::wcout << "HWID Copied To The Clipboard." << std::endl;
    }
    else {
        std::wcerr << L"Failed To Copy To The Clipboard." << std::endl;
    }

    return 1;
}
