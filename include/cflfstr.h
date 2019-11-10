#pragma once

#include <cflf.h>

#include <windows.h>

FUNDEC LPSTR StrCpy(LPSTR restrict szDestination, LPCSTR restrict szSource, SIZE_T nBufferSize);
FUNDEC LPSTR StrCat(LPSTR restrict szDestination, LPCSTR restrict szSource, SIZE_T nBufferSize);
FUNDEC SIZE_T StrLen(LPCSTR szString);
