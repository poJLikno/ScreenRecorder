#include "PreviewWindow.h"

DSSafePtr<IVideoWindow> *PreviewWindow::GetPtr()
{
    return &preview_window;
}

void PreviewWindow::AcceptProperties()
{
    if (preview_window.object) {
        preview_window.object->put_Owner((OAHWND)wnd_metrics.owner_hwnd);
        preview_window.object->put_Width(wnd_metrics.wnd_width);
        preview_window.object->put_Height(wnd_metrics.wnd_height);
        preview_window.object->put_Left(wnd_metrics.wnd_x);
        preview_window.object->put_Top(wnd_metrics.wnd_y);
        preview_window.object->put_WindowStyle(wnd_metrics.wnd_style);
    }
}

void PreviewWindow::PutOwner(HWND hWnd)
{
    if (preview_window.object) {
        preview_window.object->put_Owner((OAHWND)hWnd);
    }

    wnd_metrics.owner_hwnd = hWnd;
}

void PreviewWindow::PutSize(unsigned int width, unsigned int height)
{
    if (preview_window.object) {
        preview_window.object->put_Width(width);
        preview_window.object->put_Height(height);
    }

    wnd_metrics.wnd_width = width;
    wnd_metrics.wnd_height = height;
}

void PreviewWindow::PutPos(unsigned int x, unsigned int y)
{
    if (preview_window.object) {
        preview_window.object->put_Left(x);
        preview_window.object->put_Top(y);
    }

    wnd_metrics.wnd_x = x;
    wnd_metrics.wnd_y = y;
}

void PreviewWindow::PutStyle(DWORD style)
{
    if (preview_window.object) {
        preview_window.object->put_WindowStyle(style);
    }

    wnd_metrics.wnd_style = style;
}

void PreviewWindow::QueryMetrics(PreviewWindowMetrics **metrics_ptr)
{
    *metrics_ptr = &wnd_metrics;
}
