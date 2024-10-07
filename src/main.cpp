#include <windows.h>

#include "Model/Model.h"
#include "UI/UI.h"
#include "Controller/Controller.h"

int wmain(int argc, const wchar_t **argv)
{
	int result = 0;

	try
	{
		setlocale(LC_ALL, "");
		_wsetlocale(LC_ALL, L"");
		SetConsoleCP(1251);
		SetConsoleOutputCP(1251);

		// COM init
		HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED | COINIT_SPEED_OVER_MEMORY);
		if (FAILED(hr)) throw std::runtime_error("Can't initialize COM");

		Model app_model;
		UI app_ui;

		Controller::model = &app_model;
		Controller::InitController();

		app_ui.app_wnd.ShowWnd(true);

		result = app_ui.app_wnd.Run();

		// COM release
		CoUninitialize();
	}
	catch (std::wstring& e)
	{
		MessageBox(NULL, e.c_str(), L"Error", MB_OK);
	}

	return result;
}

// Set new style
		//SetWindowLongPtr(window.hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_THICKFRAME));
		//SetWindowPos(window.hWnd, NULL, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);

		/*HWND hw = FindWindowEx(window.hWnd, NULL, NULL, "ActiveMovie Window");// Find child window
		SetWindowLongPtr(hw, GWL_STYLE, WS_VISIBLE | WS_CHILD);
		SetWindowPos(hw, NULL, 10, 10, 500, 281, SWP_SHOWWINDOW | SWP_NOZORDER);*/

		//SetWindowPos(window.hWnd, NULL, 0, 0, 1000, 1000, SWP_SHOWWINDOW | SWP_NOZORDER | SWP_NOMOVE);
