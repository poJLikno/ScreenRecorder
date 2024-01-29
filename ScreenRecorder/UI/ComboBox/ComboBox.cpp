#include "ComboBox.h"

ComboBox::ComboBox(HWND hWnd, int x, int y, int width, int height, DWORD style)
{
	this->hWnd = CreateWindow(L"combobox", L"", style, x, y, width, height, hWnd, NULL, NULL, NULL);
	if (!this->hWnd)
		throw std::runtime_error("Can't create combo box");
}

HWND ComboBox::GetHwnd() const
{
	return this->hWnd;
}

void ComboBox::AddItem(const wchar_t *item)
{
	SendMessage(this->hWnd, CB_ADDSTRING, 0, (LPARAM)item);
}

void ComboBox::SelectItem(int id)
{
	SendMessage(this->hWnd, CB_SETCURSEL, (WPARAM)id, 0);
}

int ComboBox::GetItemId()
{
	return (int)SendMessage(this->hWnd, CB_GETCURSEL, 0, 0);
}

void ComboBox::GetItem(wchar_t *buffer)
{
	SendMessage(this->hWnd, CB_GETLBTEXT, (WPARAM)GetItemId(), (LPARAM)buffer);
}

void ComboBox::GetOldItem(wchar_t *buffer, int buffer_size)
{
	GetWindowText(this->hWnd, buffer, buffer_size);
	//SendMessage(this->hWnd, CB_GETLBTEXT, (WPARAM)GetItemId(), (LPARAM)buffer);
}

bool ComboBox::WasChanged(WPARAM *wParam, LPARAM *lParam)
{
	bool result = false;
	if (*lParam == (LPARAM)this->hWnd && HIWORD(*wParam) == CBN_SELCHANGE)
		result = true;
	return result;
}
