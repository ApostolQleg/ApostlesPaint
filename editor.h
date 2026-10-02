#pragma once

#include "Resource.h"

class Editor
{
protected:
    bool isDrawing = false;
    long xstart = 0, ystart = 0;
    long xend = 0, yend = 0;

    virtual UINT GetMenuItemId() const = 0;

public:
    Editor() {}
    virtual ~Editor() {}

    virtual void OnLBdown(HWND hWnd);
    virtual void OnLBup(HWND hWnd);
    virtual void OnMouseMove(HWND hWnd);

    void OnInitMenuPopup(HWND hWnd, WPARAM wParam);
};

class PointEditor : public Editor
{
protected:
    UINT GetMenuItemId() const override { return IDM_POINT; }
public:
    void OnMouseMove(HWND hWnd) override;
    void OnLBup(HWND hWnd) override;
};

class LineEditor : public Editor
{
protected:
    UINT GetMenuItemId() const override { return IDM_LINE; }
public:
    void OnMouseMove(HWND hWnd) override;
    void OnLBup(HWND hWnd) override;
};

class RectEditor : public Editor
{
protected:
    UINT GetMenuItemId() const override { return IDM_RECT; }
public:
    void OnMouseMove(HWND hWnd) override;
    void OnLBup(HWND hWnd) override;
};

class EllipseEditor : public Editor
{
protected:
    UINT GetMenuItemId() const override { return IDM_ELLIPSE; }
public:
    void OnMouseMove(HWND hWnd) override;
    void OnLBup(HWND hWnd) override;
};