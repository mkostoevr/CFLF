#include <cflf.h>

// call DeleteObject on return.hHandle when don't need returned FULLBITMAP anymore
FULLBITMAP GetScreenBitmap() {
    FULLBITMAP output;
    BITMAPINFOHEADER biHeader = { 0 };
    BITMAPINFO bInfo = { 0 };
    HBITMAP hBitmap;
    HDC hDC;
    HDC hMemDC;
    LONG lWidth;
    LONG lHeight;
    BYTE *bBits = NULL;
    
    biHeader.biSize = sizeof(BITMAPINFOHEADER);
    biHeader.biBitCount = 24;
    biHeader.biCompression = BI_RGB;
    biHeader.biPlanes = 1;
    lWidth = GetSystemMetrics(SM_CXSCREEN);
    lHeight = GetSystemMetrics(SM_CYSCREEN);
    biHeader.biWidth = lWidth;
    biHeader.biHeight = lHeight;
    bInfo.bmiHeader = biHeader;
    hDC = GetDC(NULL);
    hMemDC = CreateCompatibleDC(hDC);
    hBitmap = CreateDIBSection(hDC, &bInfo, DIB_RGB_COLORS, (VOID **)&bBits, NULL, 0);
    SelectObject(hMemDC, hBitmap);
    BitBlt(hMemDC, 0, 0, lWidth, lHeight, hDC, 0, 0, SRCCOPY);
    DeleteDC(hMemDC);
    ReleaseDC(NULL, hDC);
    output.hHandle = hBitmap;
    output.pBits = bBits;
    output.lWidth = lWidth;
    output.lHeight = lHeight;
    return output;
}
