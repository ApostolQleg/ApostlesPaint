#include "pch.h"
#include "framework.h"
#include "shape_editor.h"
#include "editor.h"
#include "shape.h"

Shape *pcshape[MAX_OBJECTS_COUNT] = {nullptr};
int shapesCount = 0;

ShapeEditor::ShapeEditor()
{
    pse = nullptr;
}

ShapeEditor::~ShapeEditor()
{
    if (pse)
    {
        delete pse;
        pse = nullptr;
    }

    for (int i = 0; i < shapesCount; i++)
    {
        if (pcshape[i])
        {
            delete pcshape[i];
            pcshape[i] = nullptr;
        }
    }
    shapesCount = 0;
}

void ShapeEditor::StartPointEditor()
{
    if (pse)
        delete pse;
    pse = new PointEditor();
}

void ShapeEditor::StartLineEditor()
{
    if (pse)
        delete pse;
    pse = new LineEditor();
}

void ShapeEditor::StartRectEditor()
{
    if (pse)
        delete pse;
    pse = new RectEditor();
}

void ShapeEditor::StartEllipseEditor()
{
    if (pse)
        delete pse;
    pse = new EllipseEditor();
}

void ShapeEditor::OnLBdown(HWND hWnd)
{
    if (pse)
        pse->OnLBdown(hWnd);
}

void ShapeEditor::OnLBup(HWND hWnd)
{
    if (pse)
        pse->OnLBup(hWnd);
}

void ShapeEditor::OnMouseMove(HWND hWnd)
{
    if (pse)
        pse->OnMouseMove(hWnd);
}

void ShapeEditor::OnInitMenuPopup(HWND hWnd, WPARAM wParam)
{
    if (pse)
        pse->OnInitMenuPopup(hWnd, wParam);
}

void ShapeEditor::OnPaint(HWND hWnd)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);

    for (int i = 0; i < shapesCount; i++)
    {
        if (pcshape[i])
        {
            pcshape[i]->Show(hdc);
        }
    }

    EndPaint(hWnd, &ps);
}