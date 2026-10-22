#pragma once

#include "Resource.h"

class TableListener
{
public:
	virtual ~TableListener() {}

	virtual void OnTableOpened() = 0;
	virtual void OnTableSelect(int index) = 0;
	virtual void OnTableDelete(int index) = 0;
	virtual void OnTableClosed() = 0;
};

class Table
{
private:
	static Table* p_instance;
	WCHAR* pshape_list = {nullptr};

	TableListener* listener = nullptr;

	Table() {};
	~Table() {};

	Table(const Table&) = delete;
	Table& operator=(const Table&) = delete;
public:
	static Table* getInstance();

	void SetListener(TableListener* l) { listener = l; }

	void Add(const WCHAR* payload);
	void Show(HWND hWnd);
	void Clear();
	void Close();

	void NotifyOpened()			 { if (listener) listener->OnTableOpened(); }
	void NotifySelect(int index) { if (listener) listener->OnTableSelect(index); }
	void NotifyDelete(int index) { if (listener) listener->OnTableDelete(index); }
	void NotifyClosed()			 { if (listener) listener->OnTableClosed(); }
};