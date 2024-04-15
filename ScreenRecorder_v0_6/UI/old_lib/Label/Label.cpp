#include "Label.h"

Label::Label(HWND hWnd, const wchar_t *text, int x, int y, int width, int height, DWORD style)
{
	this->hWnd = CreateWindow(L"static", text, style, x, y, width, height, hWnd, NULL, NULL, NULL);
	if (!this->hWnd)
		throw std::runtime_error("Can't create label");
}

HWND Label::GetHwnd() const
{
	return this->hWnd;
}

void Label::SetText(const wchar_t *text)
{
	SetWindowText(this->hWnd, text);
}

void Label::GetText(wchar_t *buffer, size_t size)
{
	GetWindowText(this->hWnd, buffer, (int)size);
}
