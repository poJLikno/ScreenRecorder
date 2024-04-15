#include "Edit.h"

Edit::Edit(HWND hWnd, int x, int y, int width, int height, DWORD style)
{
	this->hWnd = CreateWindow(L"edit", L"", style, x, y, width, height, hWnd, NULL, NULL, NULL);
	if (!this->hWnd)
		throw std::runtime_error("Can't create edit");
}

HWND Edit::GetHwnd() const
{
	return this->hWnd;
}

void Edit::SetText(const wchar_t *text)
{
	SetWindowText(this->hWnd, text);
}

void Edit::GetText(wchar_t *buffer, size_t count)
{
	GetWindowText(this->hWnd, buffer, (int)count);
}