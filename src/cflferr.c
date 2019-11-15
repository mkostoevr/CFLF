#include <cflf.h>

static LPSTR GetErrorString(DWORD dwErrorCode) {
    LPSTR szErrorString;
    LPSTR szErrorStringBuffer;
    DWORD dwErrorStringBufferSize;
    HLOCAL hFreeResult;

    if (dwErrorCode == 0) {
        szErrorString = HeapAlloc(process.hHeap, 0, sizeof(LOCAL_ERR_noError));
        if (szErrorString == NULL) {
            FatalLowLevelError(FILE_LINE);
        }
        lstrcpyA(szErrorString, LOCAL_ERR_noError);
        return szErrorString;
    }
    dwErrorStringBufferSize = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dwErrorCode,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPSTR)&szErrorStringBuffer,
        0,
        NULL
        );
    if (dwErrorStringBufferSize == 0) {
        FatalLowLevelError(FILE_LINE);
    }
    szErrorString = HeapAlloc(process.hHeap, 0, dwErrorStringBufferSize + 1);
    if (szErrorString == NULL) {
        FatalLowLevelError(FILE_LINE);
    }
    lstrcpyA(szErrorString, szErrorStringBuffer);
    hFreeResult = LocalFree(szErrorStringBuffer);
    if (hFreeResult == szErrorStringBuffer) {
        LowLevelError(FILE_LINE);
    }
    return szErrorString;
}

static LPSTR CreateErrorMessage(DWORD dwErrorCode, LPCSTR szErrorLocation) {
    CHAR szErrorCode[11];
    LPSTR szErrorInfo;
    SIZE_T bcMessage;
    LPSTR szMessage;
    BOOL bFreeResult;

    DwordToStr(dwErrorCode, szErrorCode, 16, "0x");
    szErrorInfo = GetErrorString(dwErrorCode);
    bcMessage = sizeof(LOCAL_ERR_errorNumber) - 1 // -1 cause sizeof counts terminating null
        + sizeof(szErrorCode) - 1
        + sizeof(LOCAL_ERR_atLocation) - 1
        + lstrlenA(szErrorLocation)
        + sizeof(LOCAL_ERR_moreInfo) - 1
        + lstrlenA(szErrorInfo)
        + 1; // one more byte for terminating null
    szMessage = HeapAlloc(process.hHeap, 0, bcMessage);
    if (szMessage == NULL) {
        FatalLowLevelError(FILE_LINE);
    }
    lstrcpyA(szMessage, LOCAL_ERR_errorNumber);
    lstrcatA(szMessage, szErrorCode);
    lstrcatA(szMessage, LOCAL_ERR_atLocation);
    lstrcatA(szMessage, szErrorLocation);
    lstrcatA(szMessage, LOCAL_ERR_moreInfo);
    lstrcatA(szMessage, szErrorInfo);
    bFreeResult = HeapFree(process.hHeap, 0, szErrorInfo);
    if (!bFreeResult) {
        LowLevelError(FILE_LINE);
    }
    return szMessage;
}

VOID Error(LPCSTR szErrorLocation) {
    DWORD dwErrorCode;
    LPSTR szErrorMessage;

    dwErrorCode = GetLastError();
    szErrorMessage = CreateErrorMessage(dwErrorCode, szErrorLocation);
    MessageBoxA(
        userInterface.cMainWindow.hHandle,
        szErrorMessage,
        userInterface.cMainWindow.lpWindowName,
        MB_ICONERROR | MB_OK
        );
    HeapFree(process.hHeap, 0, szErrorMessage);
}

VOID FatalError(LPCSTR szErrorLocation) {
    Error(szErrorLocation);
    ExitProcess(-1);
}

VOID LowLevelError(LPCSTR szErrorLocation) {
    DWORD dwErrorCode;
    CHAR szErrorCode[11];

    dwErrorCode = GetLastError();
    DwordToStr(dwErrorCode, szErrorCode, 16, "0x");
    MessageBoxA(0, szErrorCode, szErrorLocation, MB_ICONERROR | MB_OK);
}

VOID FatalLowLevelError(LPCSTR szErrorLocation) {
    LowLevelError(szErrorLocation);
    ExitProcess(-1);
}
