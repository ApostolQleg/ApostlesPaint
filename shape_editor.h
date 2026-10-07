#pragma once

#include "Resource.h"

class Editor;

class ShapeEditor
{
private:
    Editor *pse = nullptr;
    HWND hWndToolbar = nullptr;

public:
    ShapeEditor();
    ~ShapeEditor();

    void OnCreate(HWND hWnd);
    void OnSize(HWND hWnd);
    void OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam);
    void SetToolState(UINT activeId);

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