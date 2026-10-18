#include "pch.h"
#include "framework.h"
#include "file_dialog.h"

std::wstring FileDialog::ShowOpenDialog(HWND hWndOwner, const WCHAR* filterName, const WCHAR* filterSpec)
{
	std::wstring filePath;

	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	bool coInitialized = SUCCEEDED(hr);

	IFileOpenDialog* pFileOpen = nullptr;
	hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

	if (SUCCEEDED(hr))
	{
		COMDLG_FILTERSPEC fileTypes[] = {
			{ filterName, filterSpec },
			{ L"All Files (*.*)", L"*.*" }
		};
		pFileOpen->SetFileTypes(ARRAYSIZE(fileTypes), fileTypes);

		if (SUCCEEDED(pFileOpen->Show(hWndOwner)))
		{
			IShellItem* pItem = nullptr;
			if (SUCCEEDED(pFileOpen->GetResult(&pItem)))
			{
				PWSTR pszFilePath = nullptr;
				if (SUCCEEDED(pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath)))
				{
					filePath = pszFilePath;
					CoTaskMemFree(pszFilePath);
				}
				pItem->Release();
			}
		}
		pFileOpen->Release();
	}

	if (coInitialized)
	{
		CoUninitialize();
	}

	return filePath;
}

std::wstring FileDialog::ShowSaveDialog(
	HWND hWndOwner, 
	const WCHAR* filterName, 
	const WCHAR* filterSpec, 
	const WCHAR* defaultExt,
	const WCHAR* defaultFileName
){
	std::wstring filePath;

	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	bool coInitialized = SUCCEEDED(hr);

	IFileSaveDialog* pFileSave = nullptr;
	hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL, IID_IFileSaveDialog, reinterpret_cast<void**>(&pFileSave));

	if (SUCCEEDED(hr))
	{
		COMDLG_FILTERSPEC fileTypes[] = {
			{ filterName, filterSpec },
			{ L"All Files (*.*)", L"*.*" }
		};
		pFileSave->SetFileTypes(ARRAYSIZE(fileTypes), fileTypes);
		pFileSave->SetDefaultExtension(defaultExt);

		pFileSave->SetFileName(defaultFileName);

		if (SUCCEEDED(pFileSave->Show(hWndOwner)))
		{
			IShellItem* pItem = nullptr;
			if (SUCCEEDED(pFileSave->GetResult(&pItem)))
			{
				PWSTR pszFilePath = nullptr;
				if (SUCCEEDED(pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath)))
				{
					filePath = pszFilePath;
					CoTaskMemFree(pszFilePath);
				}
				pItem->Release();
			}
		}
		pFileSave->Release();
	}

	if (coInitialized)
	{
		CoUninitialize();
	}

	return filePath;
}