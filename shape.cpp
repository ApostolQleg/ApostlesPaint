#include "pch.h"
#include "framework.h"
#include "shape.h"

#define POINT_RADIUS 5

void Shape::Set(long x1, long y1, long x2, long y2)
{
	xstart = x1; ystart = y1;
	xend = x2; yend = y2;
}

void PointShape::Show(HDC hdc) {
    for (int x = -POINT_RADIUS; x <= POINT_RADIUS; x++) {
        for (int y = -POINT_RADIUS; y <= POINT_RADIUS; y++) {
            SetPixel(hdc, xstart+x, ystart+y, RGB(0, 0, 0));
        }
    }
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
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Ellipse(hdc, xstart, ystart, xend, yend);
    SelectObject(hdc, hOldBrush);
}