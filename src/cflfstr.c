#include <cflf.h>

LPSTR StrCpy(LPSTR restrict szDestination, LPCSTR restrict szSource, SIZE_T bcBufferSize) {
    SIZE_T i;
    SIZE_T nMaxIndex = bcBufferSize - 1;
    for (i = 0; szSource[i] != '\0' && i < nMaxIndex; i++) {
        szDestination[i] = szSource[i];
    }
    szDestination[i] = '\0';
    return szDestination;
}

LPSTR StrCat(LPSTR restrict szDestination, LPCSTR restrict szSource, SIZE_T bcBufferSize) {
    SIZE_T nOffset = StrLen(szDestination);
    StrCpy(szDestination + nOffset, szSource, bcBufferSize);
    return szDestination;
}

SIZE_T StrLen(LPCSTR szString) {
    SIZE_T nLength = 0;
    while (szString[nLength] != '\0') {
        nLength++;
    }
    return nLength;
}
