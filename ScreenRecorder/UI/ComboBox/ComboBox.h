#ifndef BOMBO_BOX_H
#define BOMBO_BOX_H

#include <iostream>
#include <windows.h>

class ComboBox
{
private:
	HWND hWnd = { 0 };

public:
	ComboBox(HWND hWnd, int x, int y, int width, int height, DWORD style = WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST);

	HWND GetHwnd() const;

	void AddItem(const wchar_t *item);

	void SelectItem(int id);

	int GetItemId();

	void GetItem(wchar_t *buffer);

	void GetOldItem(wchar_t *buffer, int buffer_size); // Use when catch "CBN_SELCHANGE" message

	bool WasChanged(WPARAM *wParam, LPARAM *lParam); // Check "CBN_SELCHANGE" message
};

#endif