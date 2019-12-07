#include <cflf.h>

static BOOL CreateRegistryKey(HKEY hKeyParent, LPCSTR subkey) {
    DWORD dwDisposition;
    HKEY  hKey;
    DWORD Ret;

    Ret = RegCreateKeyExA(hKeyParent, subkey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition);
    if (Ret != ERROR_SUCCESS) {
        return FALSE;
    }
    RegCloseKey(hKey);
    return TRUE;
}


static BOOL WriteDwordInRegistry(HKEY hKeyParent, LPCSTR subkey, LPCSTR valueName, DWORD data) {
    DWORD Ret;
    HKEY hKey;
    
    Ret = RegOpenKeyExA(hKeyParent, subkey, 0, KEY_WRITE, &hKey);
    if (Ret == ERROR_SUCCESS) {
        if (ERROR_SUCCESS != RegSetValueExA(hKey, valueName, 0, REG_DWORD, (LPBYTE)&data, sizeof(data))) {
            RegCloseKey(hKey);
            return FALSE;
        }
        RegCloseKey(hKey);
        return TRUE;
    }
    return FALSE;
}


static BOOL ReadDwordFromRegistry(HKEY hKeyParent, LPCSTR subkey, LPCSTR valueName, DWORD *readData) {
    HKEY hKey;
    DWORD Ret;
    
    Ret = RegOpenKeyExA(hKeyParent, subkey, 0, KEY_READ, &hKey);
    if (Ret == ERROR_SUCCESS) {
        DWORD data;
        DWORD len = sizeof(DWORD);
        
        Ret = RegQueryValueExA(hKey, valueName, NULL, NULL, (LPBYTE)&data, &len);
        if (Ret == ERROR_SUCCESS) {
            RegCloseKey(hKey);
            (*readData) = data;
            return TRUE;
        }
        RegCloseKey(hKey);
        return TRUE;
    } else {
        return FALSE;
    }
}

BOOL SaveConfigDword(LPCSTR szName, DWORD dwValue) {
    if (!CreateRegistryKey(HKEY_CURRENT_USER, "SOFTWARE\\CFLF")) {
        return FALSE;
    }
    if (!WriteDwordInRegistry(HKEY_CURRENT_USER, "SOFTWARE\\CFLF", szName, dwValue)) {
        return FALSE;
    }
    return TRUE;
}

BOOL LoadConfigDword(LPCSTR szName, DWORD *dwValue, DWORD dwDefaultValue) {
    DWORD readData = dwDefaultValue;
    
    *dwValue = dwDefaultValue;
    if (!CreateRegistryKey(HKEY_CURRENT_USER, "SOFTWARE\\CFLF")) {
        return FALSE;
    }
    if (!ReadDwordFromRegistry(HKEY_CURRENT_USER, "SOFTWARE\\CFLF", szName, &readData)) {
        return FALSE;
    }
    *dwValue = readData;
    return TRUE;
}

