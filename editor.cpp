#include "pch.h"
#include "framework.h"
#include "editor.h"
#include "shape.h"

static void GetMousePos(HWND hWnd, long& x, long& y)
{
    POINT pt;
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
    x = pt.x;
    y = pt.y;
}


Editor::~Editor()
{
    delete pcurrent;
    pcurrent = nullptr;

    for (int i = 0; i < shapesCount; i++)
        delete pshape[i];

    delete[] pshape;
    pshape = nullptr;
    shapesCount = shapesCapacity = 0;
}

void Editor::AddShape(Shape* p)
{
    if (shapesCount >= shapesCapacity)
    {
        int newCapacity = (shapesCapacity > 0) ? shapesCapacity * 2 : INIT_SHAPES_COUNT;

        Shape** newShapes = new Shape * [newCapacity]();

        for (int i = 0; i < shapesCount && i < newCapacity; i++)
            newShapes[i] = pshape[i];

        delete[] pshape;
        pshape = newShapes;
        shapesCapacity = newCapacity;
    }

    if (pshape && shapesCount >= 0 && shapesCount < shapesCapacity)
        pshape[shapesCount++] = p;
    else
        delete p;
}

void Editor::OnCreate(HWND hWnd)
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
        IDC_SHAPE_TOOLBAR,
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

void Editor::OnSize(HWND hWnd)
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

void Editor::OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    LPNMHDR pnmh = (LPNMHDR)lParam;

    if (pnmh->code == TTN_NEEDTEXT)
    {
        LPTOOLTIPTEXT lpttt = (LPTOOLTIPTEXT)lParam;

        switch (lpttt->hdr.idFrom)
        {
        case IDM_POINT:   lstrcpyW(lpttt->szText, L"Point");     break;
        case IDM_LINE:    lstrcpyW(lpttt->szText, L"Line");      break;
        case IDM_RECT:    lstrcpyW(lpttt->szText, L"Rectangle"); break;
        case IDM_ELLIPSE: lstrcpyW(lpttt->szText, L"Ellipse");   break;
        default:          lstrcpyW(lpttt->szText, L"Tool");      break;
        }
    }
}

void Editor::SetToolState(UINT activeId)
{
    if (!hWndToolbar) return;

    UINT ids[] = { IDM_POINT, IDM_LINE, IDM_RECT, IDM_ELLIPSE };
    for (UINT id : ids)
    {
        SendMessage(hWndToolbar, TB_PRESSBUTTON, id, (id == activeId) ? TRUE : FALSE);
    }
}

void Editor::Start(Shape* p, UINT cmdId)
{
    delete pcurrent;
    pcurrent = p;
    currentId = cmdId;
    isDrawing = false;
    rubberDrawn = false;
    SetToolState(cmdId);
}

void Editor::OnLBdown(HWND hWnd)
{
    if (!pcurrent) return;

    isDrawing = true;
    rubberDrawn = false;

    GetMousePos(hWnd, xstart, ystart);

    pcurrent->Set(xstart, ystart, xstart, ystart);

    SetCapture(hWnd);
}

void Editor::OnMouseMove(HWND hWnd)
{
    if (!isDrawing || !pcurrent) return;

    HDC hdc = GetDC(hWnd);
    SetROP2(hdc, R2_NOTXORPEN);
    SetBkMode(hdc, TRANSPARENT);

    pcurrent->SetRubber(true);

    if (rubberDrawn)
        pcurrent->Show(hdc);

    long x, y;
    GetMousePos(hWnd, x, y);

    pcurrent->Set(xstart, ystart, x, y);
    pcurrent->Show(hdc);

    rubberDrawn = true;
    pcurrent->SetRubber(false);

    ReleaseDC(hWnd, hdc);
}


void Editor::OnLBup(HWND hWnd)
{
    if (!isDrawing || !pcurrent) return;

    isDrawing = false;
    rubberDrawn = false;
    ReleaseCapture();

    long x, y;
    GetMousePos(hWnd, x, y);
    pcurrent->Set(xstart, ystart, x, y);
    pcurrent->SetRubber(false);

    Shape* next = pcurrent->Clone();
    AddShape(pcurrent);
    pcurrent = next;

    InvalidateRect(hWnd, NULL, TRUE);
}


void Editor::OnInitMenuPopup(HWND hWnd, WPARAM wParam)
{
    HMENU hMenu = GetMenu(hWnd);
    HMENU hSubMenu = GetSubMenu(hMenu, 1);

    if ((HMENU)wParam == hSubMenu)
    {
        UINT ids[] = { IDM_POINT, IDM_LINE, IDM_RECT, IDM_ELLIPSE };
        for (UINT id : ids)
            CheckMenuItem(hSubMenu, id, (id == currentId) ? MF_CHECKED : MF_UNCHECKED);
    }
}

void Editor::OnPaint(HWND hWnd)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);

    for (int i = 0; i < shapesCount; i++)
        pshape[i]->Show(hdc);

    EndPaint(hWnd, &ps);
}