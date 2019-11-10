#pragma once

#include <cflf.h>

#include <windows.h>

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#define FILE_LINE "(" __FILE__ ": " STR(__LINE__) ")"

FUNDEC VOID Error(LPCSTR szErrorLocation);
FUNDEC VOID FatalError(LPCSTR szErrorLocation);
FUNDEC VOID EarlyError(LPCSTR szErrorLocation);
FUNDEC VOID EarlyFatalError(LPCSTR szErrorLocation);
