#ifndef EDIT_H
#define EDIT_H

#include <iostream>
#include <windows.h>

class Edit
{
private:
	HWND hWnd = { 0 };

public:
	Edit(HWND hWnd, int x, int y, int width, int height, DWORD style = WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL | ES_LEFT);

	HWND GetHwnd() const;

	void SetText(const wchar_t *text);
	void GetText(wchar_t *buffer, size_t count);
};

#endif