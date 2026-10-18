#pragma once

#include "Resource.h"

class Table
{
private:
	WCHAR* pshape_list = {nullptr};
public:
	Table() {};
	~Table() {};
	void Add(const WCHAR* payload);
	void Show(HWND hWnd);
	void Close();
};