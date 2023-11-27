#include "cgdigrab.h"

#include <string.h>

static HWND wnd_hwnd;
static HDC wnd_hdc;

// Window metrics
static uint32_t x;
static uint32_t y;
static uint32_t cx;
static uint32_t cy;
static uint32_t wnd_width;
static uint32_t wnd_height;

// Frame metrics
static uint32_t frame_width;
static uint32_t frame_height;
static BITMAPINFOHEADER bmp_info_head;

// Error message box
static void ShowError(void) {
	char str[16] = { 0 };
	snprintf(str, sizeof(str), "%d", GetLastError());
	MessageBox(NULL, str, (const char *)"Error", MB_OK);
}

void cgdigrab_select_window(const char *title) {
	// Get window
	if (strcmp(title, "desktop") == 0) {
		wnd_hwnd = GetDesktopWindow();
	}
	else if (title != NULL) {
		wnd_hwnd = FindWindow(NULL, title);
	}

	if (wnd_hwnd == NULL) {// Check hWnd
		ShowError();
		return;
	}
	wnd_hdc = GetDC(wnd_hwnd);// Get hDC

	RECT wnd_rect = { 0 };// Get window metrics
	{
		GetClientRect(wnd_hwnd, &wnd_rect);
		x = wnd_rect.left;
		y = wnd_rect.top;
		cx = wnd_rect.right;
		cy = wnd_rect.bottom;
		wnd_width = wnd_rect.right - wnd_rect.left;
		wnd_height = wnd_rect.bottom - wnd_rect.top;
	}

	// Set default farme metrics if no values
	if (frame_width == 0 || frame_height == 0) {
		frame_width = wnd_width;
		frame_height = wnd_height;
	}

	// bmp_info_header
	{
		bmp_info_head.biSize = sizeof(BITMAPINFOHEADER);
		bmp_info_head.biWidth = frame_width;
		bmp_info_head.biHeight = frame_height;
		bmp_info_head.biSizeImage = frame_width * frame_height * 3;
		bmp_info_head.biCompression = BI_RGB;
		bmp_info_head.biBitCount = 24;
		bmp_info_head.biPlanes = 1;
	}
}

void cgdigrab_set_frame_resolution(uint32_t width, uint32_t height)
{
	// Set default farme metrics if no values
	frame_width = width;
	frame_height = height;

	// bmp_info_header
	{
		bmp_info_head.biWidth = frame_width;
		bmp_info_head.biHeight = frame_height;
		bmp_info_head.biSizeImage = frame_width * frame_height * 3;
	}
}
