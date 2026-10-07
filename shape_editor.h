#pragma once

#include "Resource.h"

class Editor;

class ShapeEditor
{
private:
    Editor *pse = nullptr;

public:
    ShapeEditor();
    ~ShapeEditor();

    void StartPointEditor();
    void StartLineEditor();
    void StartRectEditor();
    void StartEllipseEditor();

    void OnLBdown(HWND);
    void OnLBup(HWND);
    void OnMouseMove(HWND);
    void OnPaint(HWND);
    void OnInitMenuPopup(HWND, WPARAM);
};