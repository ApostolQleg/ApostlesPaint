#include "pch.h"
#include "framework.h"
#include "shape.h"

const int POINT_RADIUS = 5;

void Shape::Set(long x1, long y1, long x2, long y2)
{
	xstart = x1; ystart = y1;
	xend = x2; yend = y2;
}

void PointShape::Show(HDC hdc) {
    long x1 = xstart - POINT_RADIUS;
    long y1 = ystart - POINT_RADIUS;
    long x2 = xstart + POINT_RADIUS;
    long y2 = ystart + POINT_RADIUS;
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(BLACK_BRUSH));
    Rectangle(hdc, x1, y1, x2, y2);
    SelectObject(hdc, hOldBrush);
}

void LineShape::Show(HDC hdc) {
	MoveToEx(hdc, xstart, ystart, NULL);
	LineTo(hdc, xend, yend);
}

void RectShape::Show(HDC hdc) {
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, xstart, ystart, xend, yend);
    SelectObject(hdc, hOldBrush);
}

void EllipseShape::Show(HDC hdc) {
    long xcentre = (2 * xstart) - xend;
    long ycentre = (2 * ystart) - yend;
    HBRUSH hBrush = (HBRUSH)CreateSolidBrush(RGB(255, 0, 255));
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
    Ellipse(hdc, xcentre, ycentre, xend, yend);
    SelectObject(hdc, hOldBrush);
    DeleteObject(hBrush);
}