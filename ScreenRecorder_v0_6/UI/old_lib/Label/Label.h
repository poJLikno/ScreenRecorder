#ifndef LABEL_H
#define LABEL_H

#include <iostream>
#include <windows.h>

class Label
{
private:
	HWND hWnd = { 0 };
public:
	Label(HWND hWnd, const wchar_t *text, int x, int y, int width, int height, DWORD style = WS_VISIBLE | WS_CHILD);

	HWND GetHwnd() const;

	void SetText(const wchar_t *text);
	void GetText(wchar_t *buffer, size_t size);
};

#endif