#ifndef WINDOW_H
#define WINDOW_H

//#pragma comment(linker, "/subsystem:WINDOWS")
#pragma comment(linker, "/entry:wmainCRTStartup")

#include <iostream>
#include <string>
#include <windows.h>

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

class Window
{
private:
	wchar_t class_name_[256] = { 0 };
	HWND hWnd = { 0 };
	MSG msg_ = { 0 };

	static LRESULT CALLBACK DefaultWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:	
	Window(
			const wchar_t *name, bool icon,
			int x, int y, int width, int height,
			LRESULT(*WndProc_ptr)(HWND, UINT, WPARAM, LPARAM) = DefaultWndProc);

	~Window();

	HWND GetHwnd() const;

	void SetLabel(const wchar_t *label);
	void GetLabel(wchar_t *buffer, size_t count);

	void RunMessageLoop(void (*async_proc)() = nullptr);
};

#endif