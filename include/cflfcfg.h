#pragma once

#include <cflf.h>

#include <windows.h>

BOOL SaveConfigDword(LPCSTR szName, DWORD dwValue);
BOOL LoadConfigDword(LPCSTR szName, DWORD *dwValue, DWORD dwDefaultValue);
