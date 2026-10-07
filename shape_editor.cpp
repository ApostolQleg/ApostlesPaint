#include "pch.h"
#include "framework.h"
#include "shape_editor.h"
#include "editor.h"
#include "shape.h"

#define IDC_MY_TOOLBAR 10001

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

void ShapeEditor::OnCreate(HWND hWnd)
{
    HINSTANCE hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);

    TBBUTTON tbb[4]; // масив для кнопок
    ZeroMemory(tbb, sizeof(tbb));

    tbb[0].iBitmap = 0;
    tbb[0].idCommand = IDM_POINT;
    tbb[0].fsState = TBSTATE_ENABLED;
    tbb[0].fsStyle = TBSTYLE_BUTTON;

    tbb[1].iBitmap = 1;
    tbb[1].idCommand = IDM_LINE;
    tbb[1].fsState = TBSTATE_ENABLED;
    tbb[1].fsStyle = TBSTYLE_BUTTON;

    tbb[2].iBitmap = 2;
    tbb[2].idCommand = IDM_RECT;
    tbb[2].fsState = TBSTATE_ENABLED;
    tbb[2].fsStyle = TBSTYLE_BUTTON;

    tbb[3].iBitmap = 3;
    tbb[3].idCommand = IDM_ELLIPSE;
    tbb[3].fsState = TBSTATE_ENABLED;
    tbb[3].fsStyle = TBSTYLE_BUTTON;

    hWndToolbar = CreateToolbarEx(
        hWnd,
        WS_CHILD | WS_VISIBLE | WS_BORDER | WS_CLIPSIBLINGS | CCS_TOP | TBSTYLE_TOOLTIPS,
        IDC_MY_TOOLBAR,
        4, // кількість кнопок у картинці
        hInstance,
        IDB_TOOLBAR,
        tbb,
        4, // кількість кнопок у тулбарі
        24, 24, // висота кнопки
        24, 24, // висота картинки
        sizeof(TBBUTTON)
    );
}

void ShapeEditor::OnSize(HWND hWnd)
{
    if (hWndToolbar)
    {
        RECT rc, rw;
        GetClientRect(hWnd, &rc); //нові розміри головного вікна
        GetWindowRect(hWndToolbar, &rw); //потрібно знати висоту Toolbar

        MoveWindow(
            hWndToolbar,
            0, 0,
            rc.right - rc.left, //ширина Toolbar як у головного вікна
            rw.bottom - rw.top, 
            FALSE);
    }
}

void ShapeEditor::OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    LPNMHDR pnmh = (LPNMHDR)lParam;

    if (pnmh->code == TTN_NEEDTEXT)
    {
        LPTOOLTIPTEXT lpttt = (LPTOOLTIPTEXT)lParam;

        switch (lpttt->hdr.idFrom)
        {
        case IDM_POINT:
            lstrcpy(lpttt->szText, L"Point");
            break;
        case IDM_LINE:
            lstrcpy(lpttt->szText, L"Line");
            break;
        case IDM_RECT:
            lstrcpy(lpttt->szText, L"Rectangle");
            break;
        case IDM_ELLIPSE:
            lstrcpy(lpttt->szText, L"Ellipse");
            break;
        default:
            lstrcpy(lpttt->szText, L"Tool");
            break;
        }
    }
}

void ShapeEditor::SetToolState(UINT activeId)
{
    if (!hWndToolbar) return;

    UINT ids[] = { IDM_POINT, IDM_LINE, IDM_RECT, IDM_ELLIPSE };
    for (UINT id : ids)
    {
        SendMessage(hWndToolbar, TB_PRESSBUTTON, id, (id == activeId) ? TRUE : FALSE);
    }
}

void ShapeEditor::StartPointEditor()
{
    if (pse) delete pse;
    pse = new PointEditor();
    SetToolState(IDM_POINT);
}

void ShapeEditor::StartLineEditor()
{
    if (pse) delete pse;
    pse = new LineEditor();
    SetToolState(IDM_LINE);
}

void ShapeEditor::StartRectEditor()
{
    if (pse) delete pse;
    pse = new RectEditor();
    SetToolState(IDM_RECT);
}

void ShapeEditor::StartEllipseEditor()
{
    if (pse) delete pse;
    pse = new EllipseEditor();
    SetToolState(IDM_ELLIPSE);
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