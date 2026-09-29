#include "pch.h"
#include "framework.h"
#include "editor.h"
#include "shape.h"

extern Shape* pcshape[];
extern int shapesCount;

void ShapeEditor::OnLBdown(HWND hWnd)
{
    isDrawing = true;

    POINT pt;
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);

    xstart = xend = pt.x;
    ystart = yend = pt.y;
}

void ShapeEditor::OnLBup(HWND hWnd) {}
void ShapeEditor::OnMouseMove(HWND hWnd) {}
void ShapeEditor::OnPaint(HWND hWnd) {}
void ShapeEditor::OnInitMenuPopup(HWND hWnd, WPARAM wParam) {}

void PointEditor::OnLBup(HWND hWnd) {
    if (!isDrawing) return;
    isDrawing = false;

    POINT pt;
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
    xend = pt.x;
    yend = pt.y;

    if (shapesCount < MAX_LOADSTRING)
    {
        PointShape* pPoint = new PointShape();
        pPoint->Set(xstart, ystart, xend, yend);

        pcshape[shapesCount] = pPoint;
        shapesCount++;
    }

    InvalidateRect(hWnd, NULL, TRUE);
}

void PointEditor::OnInitMenuPopup(HWND hWnd, WPARAM wParam)
{
    HMENU hMenu = GetMenu(hWnd);
    HMENU hSubMenu = GetSubMenu(hMenu, 1);

    if ((HMENU)wParam == hSubMenu)
    {
        CheckMenuItem(hSubMenu, IDM_POINT, MF_CHECKED);
        CheckMenuItem(hSubMenu, IDM_LINE, MF_UNCHECKED);
        CheckMenuItem(hSubMenu, IDM_RECT, MF_UNCHECKED);
        CheckMenuItem(hSubMenu, IDM_ELLIPSE, MF_UNCHECKED);
    }
}

void LineEditor::OnMouseMove(HWND hWnd)
{
    if (!isDrawing) return;

    HPEN hPen, hOldPen;
    HDC hdc = GetDC(hWnd);

    SetROP2(hdc, R2_NOTXORPEN);
    hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    hOldPen = (HPEN)SelectObject(hdc, hPen);

    MoveToEx(hdc, xstart, ystart, NULL);
    LineTo(hdc, xend, yend);

    POINT pt;
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
    xend = pt.x;
    yend = pt.y;

    MoveToEx(hdc, xstart, ystart, NULL);
    LineTo(hdc, xend, yend);

    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);
    ReleaseDC(hWnd, hdc);
}

void LineEditor::OnLBup(HWND hWnd)
{
    if (!isDrawing) return;
    isDrawing = false;

    POINT pt;
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
    xend = pt.x;
    yend = pt.y;

    if (shapesCount < MAX_LOADSTRING)
    {
        LineShape* pLine = new LineShape();
        pLine->Set(xstart, ystart, xend, yend);

        pcshape[shapesCount] = pLine;
        shapesCount++;
    }

    InvalidateRect(hWnd, NULL, TRUE);
}

void LineEditor::OnInitMenuPopup(HWND hWnd, WPARAM wParam)
{
    HMENU hMenu = GetMenu(hWnd);
    HMENU hSubMenu = GetSubMenu(hMenu, 1);

    if ((HMENU)wParam == hSubMenu)
    {
        CheckMenuItem(hSubMenu, IDM_POINT, MF_UNCHECKED);
        CheckMenuItem(hSubMenu, IDM_LINE, MF_CHECKED);
        CheckMenuItem(hSubMenu, IDM_RECT, MF_UNCHECKED);
        CheckMenuItem(hSubMenu, IDM_ELLIPSE, MF_UNCHECKED);
    }
}

void RectEditor::OnMouseMove(HWND hWnd)
{
    if (!isDrawing) return;

    HDC hdc = GetDC(hWnd);
    SetROP2(hdc, R2_NOTXORPEN);

    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

    Rectangle(hdc, xstart, ystart, xend, yend);

    POINT pt;
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
    xend = pt.x;
    yend = pt.y;

    Rectangle(hdc, xstart, ystart, xend, yend);

    SelectObject(hdc, hOldBrush);
    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);
    ReleaseDC(hWnd, hdc);
}

void RectEditor::OnLBup(HWND hWnd)
{
    if (!isDrawing) return;
    isDrawing = false;

    POINT pt;
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
    xend = pt.x;
    yend = pt.y;

    if (shapesCount < MAX_LOADSTRING)
    {
        RectShape* pRect = new RectShape();
        pRect->Set(xstart, ystart, xend, yend);

        pcshape[shapesCount] = pRect;
        shapesCount++;
    }

    InvalidateRect(hWnd, NULL, TRUE);
}

void RectEditor::OnInitMenuPopup(HWND hWnd, WPARAM wParam)
{
    HMENU hMenu = GetMenu(hWnd);
    HMENU hSubMenu = GetSubMenu(hMenu, 1);

    if ((HMENU)wParam == hSubMenu)
    {
        CheckMenuItem(hSubMenu, IDM_POINT, MF_UNCHECKED);
        CheckMenuItem(hSubMenu, IDM_LINE, MF_UNCHECKED);
        CheckMenuItem(hSubMenu, IDM_RECT, MF_CHECKED);
        CheckMenuItem(hSubMenu, IDM_ELLIPSE, MF_UNCHECKED);
    }
}

void EllipseEditor::OnMouseMove(HWND hWnd)
{
    if (!isDrawing) return;

    HDC hdc = GetDC(hWnd);
    SetROP2(hdc, R2_NOTXORPEN);

    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

    Ellipse(hdc, xstart, ystart, xend, yend);

    POINT pt;
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
    xend = pt.x;
    yend = pt.y;

    Ellipse(hdc, xstart, ystart, xend, yend);

    SelectObject(hdc, hOldBrush);
    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);
    ReleaseDC(hWnd, hdc);
}

void EllipseEditor::OnLBup(HWND hWnd)
{
    if (!isDrawing) return;
    isDrawing = false;

    POINT pt;
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
    xend = pt.x;
    yend = pt.y;

    if (shapesCount < MAX_LOADSTRING)
    {
        EllipseShape* pEllipse = new EllipseShape();
        pEllipse->Set(xstart, ystart, xend, yend);

        pcshape[shapesCount] = pEllipse;
        shapesCount++;
    }

    InvalidateRect(hWnd, NULL, TRUE);
}

void EllipseEditor::OnInitMenuPopup(HWND hWnd, WPARAM wParam)
{
    HMENU hMenu = GetMenu(hWnd);
    HMENU hSubMenu = GetSubMenu(hMenu, 1);

    if ((HMENU)wParam == hSubMenu)
    {
        CheckMenuItem(hSubMenu, IDM_POINT, MF_UNCHECKED);
        CheckMenuItem(hSubMenu, IDM_LINE, MF_UNCHECKED);
        CheckMenuItem(hSubMenu, IDM_RECT, MF_UNCHECKED);
        CheckMenuItem(hSubMenu, IDM_ELLIPSE, MF_CHECKED);
    }
}