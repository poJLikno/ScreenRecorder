#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
//#include <string.h>
//#include <pthread.h>
#include <windows.h>

//#pragma comment(lib, "gdi32.lib")

#pragma warning(disable: 4996)

typedef struct {
	int width;
	int height;
	HBITMAP hbmp;
} Frame;

int main(int argc, const char **argv) {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	HWND wnd_hwnd = GetDesktopWindow();
	HDC wnd_hdc = GetDC(wnd_hwnd);

	RECT wnd_rect = { 0 };
	GetClientRect(wnd_hwnd, &wnd_rect);

	uint32_t wnd_width = wnd_rect.right - wnd_rect.left;
	uint32_t wnd_height = wnd_rect.bottom - wnd_rect.top;

	BITMAPINFOHEADER bmp_info_head = { 0 };
	{
		bmp_info_head.biSize = sizeof(BITMAPINFOHEADER);
		bmp_info_head.biWidth = wnd_width;
		bmp_info_head.biHeight = wnd_height;
		bmp_info_head.biSizeImage = wnd_width * wnd_height * 3;
		bmp_info_head.biCompression = BI_RGB;//BI_JPEG;
		bmp_info_head.biBitCount = 24;
		bmp_info_head.biPlanes = 1;
	}

	uint8_t *bmp_buffer = NULL;

	HBITMAP bmp = CreateDIBSection(
		wnd_hdc, (const BITMAPINFO *)&bmp_info_head,
		DIB_RGB_COLORS, (void **)&bmp_buffer,
		NULL, NULL);

	HDC bmp_hdc = CreateCompatibleDC(wnd_hdc);

	HGDIOBJ old_obj = SelectObject(bmp_hdc, (HGDIOBJ)bmp);

	BitBlt(
		bmp_hdc,
		wnd_rect.left, wnd_rect.top, wnd_rect.right, wnd_rect.bottom,
		wnd_hdc,
		wnd_rect.left, wnd_rect.top,
		SRCCOPY);

	SelectObject(bmp_hdc, old_obj);

	BITMAPFILEHEADER bmp_file_head = { 0 };
	{
		bmp_file_head.bfType = 'B' + ('M' << 8);
		bmp_file_head.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
		bmp_file_head.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
			+ (unsigned long long)wnd_width * (unsigned long long)wnd_height * 3ull;
	}

	FILE *bmp_file = NULL;
	if ((bmp_file = fopen("test_picture.bmp", "wb")) == NULL) return -1;
	{
		fwrite((const void *)&bmp_file_head, 1ull, sizeof(BITMAPFILEHEADER), bmp_file);
		fwrite((const void *)&bmp_info_head, 1ull, sizeof(BITMAPINFOHEADER), bmp_file);
		fwrite((const void *)bmp_buffer, (size_t)(wnd_width * wnd_height * 3), sizeof(uint8_t), bmp_file);
		fclose(bmp_file);
	}

	// Clean up
	DeleteDC(bmp_hdc);
	DeleteObject((HGDIOBJ)bmp);
	ReleaseDC(wnd_hwnd, wnd_hdc);
	bmp_buffer = NULL;

	return 0;
}

//system(L"ffmpeg -f gdigrab -s 1280x800 -framerate 60 -i desktop out.mp4");

	//Sleep(1000);

	//system(L"del out.mp4");
