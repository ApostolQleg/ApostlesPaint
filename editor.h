#pragma once

#include "Resource.h"
#include "shape.h"

class Editor
{
private:
    static Editor* p_instance;

    Editor() {};
    ~Editor();

    Editor(const Editor&) = delete;
    Editor& operator=(const Editor&) = delete;

    Shape** pshape = nullptr;
    int shapesCount = 0;
    int shapesCapacity = 0;

    Shape* pcurrent = nullptr;
    UINT currentId = 0;
    bool isDrawing = false;
    bool rubberDrawn = false;

    long xstart = 0, ystart = 0;

    HWND hWndToolbar = nullptr;

    void AddShape(Shape* p);
public:
    static Editor* getInstance();

    int GetShapesCount() const { return shapesCount; }
    Shape* GetShape(int index) const {
        if (index >= 0 && index < shapesCount)
            return pshape[index];
        return nullptr;
    }

    void OnCreate(HWND hWnd);
    void OnSize(HWND hWnd);
    void OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam);
    void SetToolState(UINT activeId);

    void Start(Shape* p, UINT cmdId);

    void OnLBdown(HWND);
    void OnLBup(HWND);
    void OnMouseMove(HWND);
    void OnPaint(HWND);
    void OnInitMenuPopup(HWND, WPARAM);

    void Save();
    void Open(HWND hWnd);
};