#include <windows.h>
#include "comctl32.h"

void ReadColors()
{
	comctl32_color.clrBtnHighlight     =  GetSysColor(COLOR_BTNHIGHLIGHT);
	comctl32_color.clrBtnShadow        =  GetSysColor(COLOR_BTNSHADOW);
	comctl32_color.clrBtnText          =  GetSysColor(COLOR_BTNTEXT);
	comctl32_color.clrBtnFace          =  GetSysColor(COLOR_BTNFACE);
	comctl32_color.clrHighlight        =  GetSysColor(COLOR_HIGHLIGHT);
	comctl32_color.clrHighlightText    =  GetSysColor(COLOR_HIGHLIGHTTEXT);
	//comctl32_color.clrHotTrackingColor =  GetSysColor(COLOR_BTNFACE);
	comctl32_color.clr3dHilight        =  GetSysColor(COLOR_3DHILIGHT);
	comctl32_color.clr3dShadow         =  GetSysColor(COLOR_3DSHADOW);
	comctl32_color.clr3dDkShadow       =  GetSysColor(COLOR_3DDKSHADOW);
	comctl32_color.clr3dFace           =  GetSysColor(COLOR_3DFACE);
	comctl32_color.clrWindow           =  GetSysColor(COLOR_WINDOW);
	comctl32_color.clrWindowText       =  GetSysColor(COLOR_WINDOWTEXT);
	comctl32_color.clrGrayText         =  GetSysColor(COLOR_GRAYTEXT);
	comctl32_color.clrActiveCaption    =  GetSysColor(COLOR_ACTIVECAPTION);
	comctl32_color.clrInfoBk           =  GetSysColor(COLOR_INFOBK);
	comctl32_color.clrInfoText         =  GetSysColor(COLOR_INFOTEXT);
}

BOOL imagelist_has_alpha( HIMAGELIST himl, UINT index )
{
	//IMAGEINFO info{};
	//ImageList_GetImageInfo(himl, index, &info);
	//return info. & ILIF_ALPHA;
	//return himl->item_flags[index] & ILIF_ALPHA;
	return false;
}

/***********************************************************************
* COMCTL32_DrawInsertMark [NOT AN API]
*
* Draws an insertion mark (which looks similar to an 'I').
*
* PARAMS
*     hDC           [I] Device context to draw onto.
*     lpRect        [I] Co-ordinates of insertion mark.
*     clrInsertMark [I] Colour of the insertion mark.
*     bHorizontal   [I] True if insert mark should be drawn horizontally,
*                       vertical otherwise.
*
* RETURNS
*     none
*
* NOTES
*     Draws up to but not including the bottom co-ordinate when drawing
*     vertically or the right co-ordinate when horizontal.
*/
void COMCTL32_DrawInsertMark(HDC hDC, const RECT *lpRect, COLORREF clrInsertMark, BOOL bHorizontal)
{
    HPEN hPen = CreatePen(PS_SOLID, 1, clrInsertMark);
    HPEN hOldPen;
    static const DWORD adwPolyPoints[] = {4,4,4};
    LONG lCentre = (bHorizontal ? 
        lpRect->top + (lpRect->bottom - lpRect->top)/2 : 
        lpRect->left + (lpRect->right - lpRect->left)/2);
    LONG l1 = (bHorizontal ? lpRect->left : lpRect->top);
    LONG l2 = (bHorizontal ? lpRect->right : lpRect->bottom);
    const POINT aptInsertMark[] =
    {
        /* top (V) or left (H) arrow */
        {lCentre    , l1 + 2},
        {lCentre - 2, l1    },
        {lCentre + 3, l1    },
        {lCentre + 1, l1 + 2},
        /* middle line */
        {lCentre    , l2 - 2},
        {lCentre    , l1 - 1},
        {lCentre + 1, l1 - 1},
        {lCentre + 1, l2 - 2},
        /* bottom (V) or right (H) arrow */
        {lCentre    , l2 - 3},
        {lCentre - 2, l2 - 1},
        {lCentre + 3, l2 - 1},
        {lCentre + 1, l2 - 3},
    };
    hOldPen = (HPEN)SelectObject(hDC, hPen);
    PolyPolyline(hDC, aptInsertMark, adwPolyPoints, ARRAY_SIZE(adwPolyPoints));
    SelectObject(hDC, hOldPen);
    DeleteObject(hPen);
}

/***********************************************************************
* COMCTL32_EnsureBitmapSize [internal]
*
* If needed, enlarge the bitmap so that the width is at least cxMinWidth and
* the height is at least cyMinHeight. If the bitmap already has these
* dimensions nothing changes.
*
* PARAMS
*     hBitmap       [I/O] Bitmap to modify. The handle may change
*     cxMinWidth    [I]   If the width of the bitmap is smaller, then it will
*                         be enlarged to this value
*     cyMinHeight   [I]   If the height of the bitmap is smaller, then it will
*                         be enlarged to this value
*     cyBackground  [I]   The color with which the new area will be filled
*
* RETURNS
*     none
*/
void COMCTL32_EnsureBitmapSize(HBITMAP *pBitmap, int cxMinWidth, int cyMinHeight, COLORREF crBackground)
{
    int cxNew, cyNew;
    BITMAP bmp;
    HBITMAP hNewBitmap;
    HBITMAP hNewDCBitmap, hOldDCBitmap;
    HBRUSH hNewDCBrush;
    HDC hdcNew, hdcOld;

    if (!GetObjectW(*pBitmap, sizeof(BITMAP), &bmp))
        return;
    cxNew = (cxMinWidth > bmp.bmWidth ? cxMinWidth : bmp.bmWidth);
    cyNew = (cyMinHeight > bmp.bmHeight ? cyMinHeight : bmp.bmHeight);
    if (cxNew == bmp.bmWidth && cyNew == bmp.bmHeight)
        return;

    hdcNew = CreateCompatibleDC(NULL);
    hNewBitmap = (HBITMAP)CreateBitmap(cxNew, cyNew, bmp.bmPlanes, bmp.bmBitsPixel, NULL);
    hNewDCBitmap = (HBITMAP)SelectObject(hdcNew, hNewBitmap);
    hNewDCBrush = (HBRUSH)SelectObject(hdcNew, CreateSolidBrush(crBackground));

    hdcOld = CreateCompatibleDC(NULL);
    hOldDCBitmap = (HBITMAP)SelectObject(hdcOld, *pBitmap);

    BitBlt(hdcNew, 0, 0, bmp.bmWidth, bmp.bmHeight, hdcOld, 0, 0, SRCCOPY);
    if (bmp.bmWidth < cxMinWidth)
        PatBlt(hdcNew, bmp.bmWidth, 0, cxNew, bmp.bmHeight, PATCOPY);
    if (bmp.bmHeight < cyMinHeight)
        PatBlt(hdcNew, 0, bmp.bmHeight, bmp.bmWidth, cyNew, PATCOPY);
    if (bmp.bmWidth < cxMinWidth && bmp.bmHeight < cyMinHeight)
        PatBlt(hdcNew, bmp.bmWidth, bmp.bmHeight, cxNew, cyNew, PATCOPY);

    SelectObject(hdcNew, hNewDCBitmap);
    DeleteObject(SelectObject(hdcNew, hNewDCBrush));
    DeleteDC(hdcNew);
    SelectObject(hdcOld, hOldDCBitmap);
    DeleteDC(hdcOld);

    DeleteObject(*pBitmap);    
    *pBitmap = hNewBitmap;
    return;
}