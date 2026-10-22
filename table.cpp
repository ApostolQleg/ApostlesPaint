#include "pch.h"
#include "framework.h"
#include "table.h"

HWND hWndDlg = NULL;
INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
void PopulateTable(HWND hDlg);

Table *Table::p_instance = nullptr;

Table *Table::getInstance()
{
    if (!p_instance)
    {
        p_instance = new Table();
    }
    return p_instance;
}

void Table::Show(HWND hWnd)
{
    if (hWndDlg) // якщо це вікно діалогу вже є, то закриваємо його
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

void Table::Add(const WCHAR *payload)
{
    if (hWndDlg && payload)
    {
        SendDlgItemMessage(hWndDlg, IDC_SHAPES_LIST, LB_ADDSTRING, 0, (LPARAM)payload);
    }
}

void Table::Clear()
{
    if (hWndDlg)
    {
        SendDlgItemMessage(hWndDlg, IDC_SHAPES_LIST, LB_RESETCONTENT, 0, 0);
    }
}

void Table::Close()
{
    if (hWndDlg)
    {
        DestroyWindow(hWndDlg);
        hWndDlg = NULL;
        return;
    }
}

INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    HWND hList = GetDlgItem(hDlg, IDC_SHAPES_LIST);
    int index = (int)SendMessage(hList, LB_GETCURSEL, 0, 0);

    switch (message)
    {
    case WM_INITDIALOG:
        hWndDlg = hDlg;
        SendMessage(hList, LB_RESETCONTENT, 0, 0);
        Table::getInstance()->NotifyOpened();
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
            if (index != LB_ERR)
            {
                SendMessage(hList, LB_DELETESTRING, index, 0);
                Table::getInstance()->NotifyDelete(index);
                InvalidateRect(GetParent(hDlg), NULL, TRUE);
            }
            return (INT_PTR)TRUE;
        }
        if (LOWORD(wParam) == IDCANCEL)
        {
            DestroyWindow(hDlg);
            return (INT_PTR)TRUE;
        }
        if (LOWORD(wParam) == IDC_SHAPES_LIST && HIWORD(wParam) == LBN_SELCHANGE)
        {
            Table::getInstance()->NotifySelect(index);
            InvalidateRect(GetParent(hDlg), NULL, TRUE);
        }
        break;

    case WM_DESTROY:
        Table::getInstance()->NotifyClosed();
        InvalidateRect(GetParent(hDlg), NULL, TRUE);
        hWndDlg = NULL;
        return (INT_PTR)TRUE;
    }

    return (INT_PTR)FALSE;
}