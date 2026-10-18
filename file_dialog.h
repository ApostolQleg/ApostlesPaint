#pragma once

class FileDialog
{
public:
    static std::wstring ShowOpenDialog(
        HWND hWndOwner,
        const WCHAR* filterName = L"Apostle's Paint Files (*.apostol)",
        const WCHAR* filterSpec = L"*.apostol"
    );

    static std::wstring ShowSaveDialog(
        HWND hWndOwner,
        const WCHAR* filterName = L"Apostle's Paint Files (*.apostol)",
        const WCHAR* filterSpec = L"*.apostol",
        const WCHAR* defaultExt = L"apostol",
        const WCHAR* defaultFileName = L"file.apostol"
    );
};