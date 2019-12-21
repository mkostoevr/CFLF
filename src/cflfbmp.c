#include <cflf.h>

COLORREF GetBitmapPixel(FULLBITMAP fbmp, UINT x, UINT y) {
    // set S value of SRGB structure if x and y are out of bounds
    if (x >= fbmp.lWidth || y >= fbmp.lHeight) {
        return 0xff000000;
    }
    y = (fbmp.lHeight - 1) - y;
    return *(COLORREF *)&fbmp.pBits[y * (((24 * fbmp.lWidth + 31)&~31) / 8) + x * 3] & 0xffffff;
}
