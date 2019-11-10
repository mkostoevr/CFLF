#include <cflf.h>

// call DeleteObject(pBot->fbmp.hHandle); after the functin return when don't need bitmap anymore
FULLBITMAP GetScreenBitmap() {
    FULLBITMAP output;
    BITMAPINFOHEADER biHeader = { 0 };
    BITMAPINFO bInfo = { 0 };
    HGDIOBJ hTempBitmap;
    HBITMAP hBitmap;
    BITMAP bAllDesktops = { 0 };
    HDC hDC, hMemDC;
    LONG lWidth, lHeight;
    BYTE *bBits = NULL;
    
    hDC = GetDC(NULL);
    hTempBitmap = GetCurrentObject(hDC, OBJ_BITMAP);
    GetObjectW(hTempBitmap, sizeof(BITMAP), &bAllDesktops);
    // get dimensions
    lWidth = bAllDesktops.bmWidth;
    lHeight = bAllDesktops.bmHeight;
    // delete temporary object
    DeleteObject(hTempBitmap);
    // init bfHeader
    biHeader.biSize = sizeof(BITMAPINFOHEADER);
    biHeader.biBitCount = 24;
    biHeader.biCompression = BI_RGB;
    biHeader.biPlanes = 1;
    biHeader.biWidth = lWidth;
    biHeader.biHeight = lHeight;
    // init binfo
    bInfo.bmiHeader = biHeader;
    // get hbitmap
    hMemDC = CreateCompatibleDC(hDC);
    hBitmap = CreateDIBSection(hDC, &bInfo, DIB_RGB_COLORS, (VOID **)&bBits, NULL, 0);
    SelectObject(hMemDC, hBitmap);
    BitBlt(hMemDC, 0, 0, lWidth, lHeight, hDC, 0, 0, SRCCOPY);
    // clean up
    DeleteDC(hMemDC);
    ReleaseDC(NULL, hDC);
    // generate output
    output.hHandle = hBitmap;
    output.pBits = bBits;
    output.lWidth = lWidth;
    output.lHeight = lHeight;
    return output;
}
