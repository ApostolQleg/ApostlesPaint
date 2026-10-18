#pragma once

#include "Resource.h"

class Table
{
private:
	static Table* p_instance;
	WCHAR* pshape_list = {nullptr};

	Table() {};
	~Table() {};

	Table(const Table&) = delete;
	Table& operator=(const Table&) = delete;
public:
	static Table* getInstance();

	void Add(const WCHAR* payload);
	void Show(HWND hWnd);
	void Highlight();
	void Clear();
	void Close();
};