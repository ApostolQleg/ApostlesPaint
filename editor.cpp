#include "pch.h"
#include "framework.h"
#include "editor.h"

#include "shape.h"
#include "table.h"

static const UINT toolIds[] = { IDM_POINT, IDM_LINE, IDM_RECT, IDM_ELLIPSE, IDM_LINEOO, IDM_CUBE };
static const int toolsCount = sizeof(toolIds) / sizeof(toolIds[0]);

static void GetMousePos(HWND hWnd, long& x, long& y)
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(hWnd, &pt);
	x = pt.x;
	y = pt.y;
}

Editor* Editor::p_instance = nullptr;

Editor* Editor::getInstance()
{
	if (!p_instance)
	{
		p_instance = new Editor();
	}
	return p_instance;
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

	TBBUTTON tbb[toolsCount];
	ZeroMemory(tbb, sizeof(tbb));

	for (int i = 0; i < toolsCount; i++)
	{
		tbb[i].iBitmap = i;
		tbb[i].idCommand = toolIds[i];
		tbb[i].fsState = TBSTATE_ENABLED;
		tbb[i].fsStyle = TBSTYLE_BUTTON;
	}

	hWndToolbar = CreateToolbarEx(
		hWnd,
		WS_CHILD | WS_VISIBLE | WS_BORDER | WS_CLIPSIBLINGS | CCS_TOP | TBSTYLE_TOOLTIPS,
		IDC_SHAPE_TOOLBAR,
		toolsCount,
		hInstance,
		IDB_TOOLBAR,
		tbb,
		toolsCount,
		24, 24,
		24, 24,
		sizeof(TBBUTTON)
	);
}

void Editor::OnSize(HWND hWnd)
{
	if (hWndToolbar)
	{
		RECT rc, rw;
		GetClientRect(hWnd, &rc);
		GetWindowRect(hWndToolbar, &rw);

		MoveWindow(
			hWndToolbar,
			0, 0,
			rc.right - rc.left,
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
		case IDM_LINEOO:  lstrcpyW(lpttt->szText, L"LineOO");    break;
		case IDM_CUBE:    lstrcpyW(lpttt->szText, L"Cube");      break;
		default:          lstrcpyW(lpttt->szText, L"Tool");      break;
		}
	}
}

void Editor::SetToolState(UINT activeId)
{
	if (!hWndToolbar) return;

	UINT ids[] = { IDM_POINT, IDM_LINE, IDM_RECT, IDM_ELLIPSE, IDM_LINEOO, IDM_CUBE };
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

	WCHAR buf[BUFFER_SIZE];
	pcurrent->GetInfo(buf, BUFFER_SIZE);

	Table::getInstance()->Add(buf);

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
		UINT ids[] = { IDM_POINT, IDM_LINE, IDM_RECT, IDM_ELLIPSE, IDM_LINEOO, IDM_CUBE };
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

void Editor::Save() {
	FILE* fout;
	fopen_s(&fout, "file.apostol", "wt");
	if (!fout) return;
	Editor* editor = Editor::getInstance();
	int count = editor->GetShapesCount();

	for (int i = 0; i < count; i++)
	{
		Shape* shape = editor->GetShape(i);
		if (shape)
		{
			WCHAR buf[BUFFER_SIZE];
			shape->GetInfo(buf, BUFFER_SIZE);
			fwprintf(fout, L"%s\n", buf);
		}
	}
	fclose(fout);
}

void Editor::Open(HWND hWnd) {
	FILE* fin;
	fopen_s(&fin, "file.apostol", "rt");
	if (!fin) return;

	for (int i = 0; i < shapesCount; i++)
	{
		delete pshape[i];
	}
	shapesCount = 0;

	Table::getInstance()->Clear();

	WCHAR type[BUFFER_SIZE] = { 0 };
	long x1 = 0, y1 = 0, x2 = 0, y2 = 0;

	while (fwscanf_s(fin, L"%s %ld %ld %ld %ld", type, (unsigned)_countof(type), &x1, &y1, &x2, &y2) == 5) {
		Shape* pshape = nullptr;

		if (wcscmp(type, L"Point") == 0) pshape = new Point();
		else if (wcscmp(type, L"Line") == 0) pshape = new Line();
		else if (wcscmp(type, L"Rectangle") == 0) pshape = new Rect();
		else if (wcscmp(type, L"Ellipse") == 0) pshape = new Elips();
		else if (wcscmp(type, L"LineOO") == 0) pshape = new LineOO();
		else if (wcscmp(type, L"Cube") == 0) pshape = new Cube();

		if (pshape)
		{
			pshape->Set(x1, y1, x2, y2);
			AddShape(pshape);

			WCHAR buf[BUFFER_SIZE];
			pshape->GetInfo(buf, BUFFER_SIZE);
			Table::getInstance()->Add(buf);
		}
	}
	fclose(fin);

	if (hWnd) InvalidateRect(hWnd, NULL, TRUE);
}
