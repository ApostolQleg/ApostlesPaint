#include "pch.h"
#include "framework.h"
#include "shape_editor.h"
#include "editor.h"
#include "shape.h"

Shape* pcshape[MAX_OBJECTS_COUNT] = { nullptr };
int shapesCount = 0;

ShapeObjectsEditor::ShapeObjectsEditor()
{
    pse = nullptr;
}

ShapeObjectsEditor::~ShapeObjectsEditor()
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

void ShapeObjectsEditor::StartPointEditor()
{
    if (pse) delete pse;
    pse = new PointEditor();
}

void ShapeObjectsEditor::StartLineEditor()
{
    if (pse) delete pse;
    pse = new LineEditor();
}

void ShapeObjectsEditor::StartRectEditor()
{
    if (pse) delete pse;
    pse = new RectEditor();
}

void ShapeObjectsEditor::StartEllipseEditor()
{
    if (pse) delete pse;
    pse = new EllipseEditor();
}

void ShapeObjectsEditor::OnLBdown(HWND hWnd)
{
    if (pse) pse->OnLBdown(hWnd);
}

void ShapeObjectsEditor::OnLBup(HWND hWnd)
{
    if (pse) pse->OnLBup(hWnd);
}

void ShapeObjectsEditor::OnMouseMove(HWND hWnd)
{
    if (pse) pse->OnMouseMove(hWnd);
}

void ShapeObjectsEditor::OnInitMenuPopup(HWND hWnd, WPARAM wParam)
{
    if (pse) pse->OnInitMenuPopup(hWnd, wParam);
}

void ShapeObjectsEditor::OnPaint(HWND hWnd)
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