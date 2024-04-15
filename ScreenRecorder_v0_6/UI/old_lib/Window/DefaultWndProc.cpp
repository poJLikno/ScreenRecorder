#include "Window.h"

LRESULT CALLBACK Window::DefaultWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;

	try
	{
		if (message == WM_CREATE)
		{
			result = 1;
		}
		else
		{
			/*if (message == WM_SIZE)
			{
				// Width LOWORD(lParam);
				// Height HIWORD(lParam);
				result = 0;
			}*/
			/*else if (message == WM_CTLCOLORSTATIC)// !!! For transparent "Label" !!!
			{
				SetBkMode(ps.hdc, TRANSPARENT);
				result = 0;
			}*/
			if (message == WM_KEYDOWN)
			{
				if (wParam == VK_ESCAPE) DestroyWindow(hWnd);
				result = 0;
			}
			else if (message == WM_CLOSE)
			{
				DestroyWindow(hWnd);
				result = 0;
			}
			else if (message == WM_DESTROY)
			{
				PostQuitMessage(0);
				result = 1;
			}
			else result = DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	catch (std::wstring &e)
	{
		MessageBox(NULL, e.c_str(), L"Error", MB_OK);
		DestroyWindow(hWnd);
		result = 0;
	}
    
    return result;
}