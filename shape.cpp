#include "pch.h"
#include "framework.h"
#include "shape.h"

void Shape::Set(long xstart, long ystart, long xend, long yend)
{
	x1 = xstart; y1 = ystart;
	x2 = xend; y2 = yend;
}

HPEN Shape::CreateShapePen() const
{
	if (rubber)
		return CreatePen(PS_DOT, 1, RGB(0, 0, 0));
	else if (highlight)
		return CreatePen(PS_SOLID, SHAPE_THICKNESS, RGB(255, 0, 0));
	return CreatePen(PS_SOLID, SHAPE_THICKNESS, RGB(0, 0, 0));
}

void Point::Show(HDC hdc) {
	long radius = SHAPE_THICKNESS;

	long left = x1 - radius;
	long top = y1 - radius;
	long right = x1 + radius + 1;
	long bottom = y1 + radius + 1;

	COLORREF color = highlight ? RGB(255, 0, 0) : RGB(0, 0, 0);
	HBRUSH hBrush = CreateSolidBrush(color);

	HPEN hOldPen = (HPEN)SelectObject(hdc, GetStockObject(NULL_PEN));
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

	Rectangle(hdc, left, top, right, bottom);

	SelectObject(hdc, hOldBrush);
	SelectObject(hdc, hOldPen);
	DeleteObject(hBrush);
}

void Line::Show(HDC hdc) {
	HPEN hPen = CreateShapePen();
	HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);

	SelectObject(hdc, hOldPen);
	DeleteObject(hPen);
}

void Rect::Show(HDC hdc) {
	HPEN hPen = CreateShapePen();
	HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

	Rectangle(hdc, x1, y1, x2, y2);

	SelectObject(hdc, hOldPen);
	SelectObject(hdc, hOldBrush);
	DeleteObject(hPen);
}

void Elips::Show(HDC hdc) {
	HPEN hPen = CreateShapePen();
	HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

	Ellipse(hdc, x1, y1, x2, y2);

	SelectObject(hdc, hOldPen);
	SelectObject(hdc, hOldBrush);
	DeleteObject(hPen);
}

void LineOO::Show(HDC hdc)
{
	long xold1 = x1, yold1 = y1;
	long xold2 = x2, yold2 = y2;

	Line::Show(hdc);

	const long r = LINEOO_RADIUS;

	Set(xold1 - r, yold1 - r, xold1 + r, yold1 + r);
	Elips::Show(hdc);

	Set(xold2 - r, yold2 - r, xold2 + r, yold2 + r);
	Elips::Show(hdc);

	Set(xold1, yold1, xold2, yold2);
}

void Cube::Show(HDC hdc)
{
	long xold1 = x1, yold1 = y1;
	long xold2 = x2, yold2 = y2;

	// тут x та y різних кутів r -> right, l -> left і т.д.
	long l = (x1 < x2) ? x1 : x2;
	long r = (x1 < x2) ? x2 : x1;
	long t = (y1 < y2) ? y1 : y2;
	long b = (y1 < y2) ? y2 : y1;

	// тут зсув задньої грані відносно передньої 
	long w = r - l,
		 h = b - t;
	long d = ((w < h) ? w : h) / 3;

	// передня грань
	Set(l, t + d, r - d, b);
	Rect::Show(hdc);

	// задня грань
	Set(l + d, t, r, b - d);
	Rect::Show(hdc);

	// чотири ребра, що з'єднують кути граней
	Set(l, t + d, l + d, t);  Line::Show(hdc);   // лівий верхній
	Set(r - d, t + d, r, t);  Line::Show(hdc);   // правий верхній
	Set(l, b, l + d, b - d);  Line::Show(hdc);   // лівий нижній
	Set(r - d, b, r, b - d);  Line::Show(hdc);   // правий нижній

	Set(xold1, yold1, xold2, yold2);
}