#include "Window.h"

#include "../../resource.h"

#pragma warning(disable : 4996)

Window::Window(const wchar_t *name, bool icon, int x, int y, int width, int height, LRESULT(*WndProc_ptr)(HWND, UINT, WPARAM, LPARAM))
{
    // Get icon width & height
    /*HBITMAP hIcon = LoadBitmap(NULL, icon);
    BITMAP iconInfo = {};
    GetObject(hIcon, sizeof(BITMAP), &iconInfo);
    DeleteObject(hIcon);*/
	
	//(HICON)LoadImage(HINST_THISCOMPONENT, icon, IMAGE_ICON, iconInfo.bmWidth, iconInfo.bmHeight, LR_LOADFROMFILE);
    
    // Register window class
    _snwprintf(class_name_, 256, L"%s_class", name);

    WNDCLASS wc = {};
    {
        wc.style = CS_OWNDC;// or CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = WndProc_ptr;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = sizeof(LONG_PTR);
        wc.hInstance = HINST_THISCOMPONENT;
        wc.hIcon = icon ? LoadIcon(HINST_THISCOMPONENT, MAKEINTRESOURCE(IDI_ICON1)) : LoadIcon(NULL, IDI_APPLICATION);
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)(6);//NULL
        wc.lpszMenuName = NULL;
        wc.lpszClassName = class_name_;
    }
    if (!RegisterClass(&wc))
        throw std::runtime_error("Can't register class -> Error code: " + std::to_string(GetLastError()));

    // Create window
    this->hWnd = CreateWindow(wc.lpszClassName, name, WS_OVERLAPPEDWINDOW, x, y, width, height, NULL, NULL, NULL, NULL);
    if (!this->hWnd)
        throw std::wstring(L"Can't create window -> Error code: " + std::to_wstring(GetLastError()));

    //ShowWindow(this->hWnd, SW_SHOWNORMAL);
}

Window::~Window()
{
    if (!UnregisterClass(class_name_, HINST_THISCOMPONENT))
        MessageBox(NULL, std::wstring(L"Can't unregister class -> Error code: " + std::to_wstring(GetLastError())).c_str(), L"Error", MB_OK);
    ZeroMemory(class_name_, 256);
}

HWND Window::GetHwnd() const
{
    return hWnd;
}

void Window::SetLabel(const wchar_t *label)
{
    SetWindowText(this->hWnd, label);
}

void Window::GetLabel(wchar_t *buffer, size_t count)
{
    GetWindowText(this->hWnd, buffer, (int)count);
}

void Window::RunMessageLoop(void (*async_proc)())
{
    // Program main loop
    if (async_proc)
    {
        while (true)
        {
            if (PeekMessage(&msg_, NULL, 0, 0, PM_REMOVE))
            {
                if (msg_.message == WM_QUIT) break;
                TranslateMessage(&msg_);
                DispatchMessage(&msg_);
            }
            else
            {
                async_proc();
            }
        }
    }
    else
    {
        while (GetMessage(&msg_, NULL, 0, 0))
        {
            //if (IsDialogMessage(this->hWnd, &msg_)/*..EXPERIMENTAL..*/)
            TranslateMessage(&msg_);
            DispatchMessage(&msg_);
        }
    }
}