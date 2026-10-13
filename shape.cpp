#include "pch.h"
#include "framework.h"
#include "shape.h"

void Shape::Set(long x1, long y1, long x2, long y2)
{
	xstart = x1; ystart = y1;
	xend = x2; yend = y2;
}

HPEN Shape::CreateShapePen() const
{
	if (rubber)
		return CreatePen(PS_DOT, RUBBER_THICKNESS, RGB(0, 0, 0));
	return CreatePen(PS_SOLID, SHAPE_THICKNESS, RGB(0, 0, 0));
}

void Point::Show(HDC hdc) {
	long radius = SHAPE_THICKNESS / 2;
	long x1 = xstart - radius;
	long y1 = ystart - radius;
	long x2 = xstart + radius + 1;
	long y2 = ystart + radius + 1;

	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(BLACK_BRUSH));

	Rectangle(hdc, x1, y1, x2, y2);

	SelectObject(hdc, hOldBrush);
}

void Line::Show(HDC hdc) {
	HPEN hPen = CreateShapePen();
	HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

	MoveToEx(hdc, xstart, ystart, NULL);
	LineTo(hdc, xend, yend);

	SelectObject(hdc, hOldPen);
	DeleteObject(hPen);
}

void Rect::Show(HDC hdc) {
	HPEN hPen = CreateShapePen();
	HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

	Rectangle(hdc, xstart, ystart, xend, yend);

	SelectObject(hdc, hOldPen);
	SelectObject(hdc, hOldBrush);
	DeleteObject(hPen);
}

void Elips::Show(HDC hdc) {
	HPEN hPen = CreateShapePen();
	HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

	Ellipse(hdc, xstart, ystart, xend, yend);

	SelectObject(hdc, hOldPen);
	SelectObject(hdc, hOldBrush);
	DeleteObject(hPen);
}
