#include "pch.h"
#include "framework.h"
#include "table.h"

HWND hWndDlg = NULL;
INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

void Table::Show(HWND hWnd)
{
	if (hWndDlg) //якщо це вікно діалогу вже є, то закриваємо його
	{
		PostMessage(hWndDlg, WM_SYSCOMMAND, SC_CLOSE, 0);
        hWndDlg = NULL;
		return;
	}
	
    hWndDlg = CreateDialog((HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), MAKEINTRESOURCE(IDD_TABLE), hWnd, DlgProc);
	
    if (hWndDlg)
	{
		ShowWindow(hWndDlg, SW_SHOW);
		UpdateWindow(hWndDlg);
	}
}

void Table::Add(const WCHAR* payload)
{
    if (hWndDlg && payload)
    {
        SendDlgItemMessage(hWndDlg, IDC_SHAPES_LIST, LB_ADDSTRING, 0, (LPARAM)payload);
    }
}

void Table::Close() {
    if (hWndDlg)
    {
        DestroyWindow(hWndDlg);
        return;
    }
}

INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_SYSCOMMAND:
        if (wParam == SC_CLOSE)
        {
            DestroyWindow(hDlg);
            return (INT_PTR)TRUE;
        }
        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK)
        {
            //щось робимо
            return (INT_PTR)TRUE;
        }
        if (LOWORD(wParam) == IDCANCEL)
        {
            DestroyWindow(hDlg);
            return (INT_PTR)TRUE;
        }
        break;

    case WM_DESTROY:
        hWndDlg = NULL;
        return (INT_PTR)TRUE;
    }

    return (INT_PTR)FALSE;
}