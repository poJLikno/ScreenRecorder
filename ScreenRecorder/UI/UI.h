#ifndef UI_H
#define UI_H

#include "Window/Window.h"
#include "Label/Label.h"
#include "Edit/Edit.h"
#include "ComboBox/ComboBox.h"
#include "NormalFont/NormalFont.h"
#include "Menu/Menu.h"

template <class T>
void SafeRelease(T **ppT)
{
	if (*ppT)
	{
		delete *ppT;
		*ppT = nullptr;;
	}
}

class UIControls
{
public:
	Window *app_wnd = nullptr;

	Label *video_capture_device_label = nullptr;
	Label *video_codec_label = nullptr;
	Label *audio_capture_device_label = nullptr;
	Label *audio_codec_label = nullptr;
	Label *video_codec_quality_label = nullptr;

	Edit *video_codec_quality_edit = nullptr;

	ComboBox *video_capture_device_combo_box = nullptr;
	ComboBox *video_codec_combo_box = nullptr;
	ComboBox *audio_capture_device_combo_box = nullptr;
	ComboBox *audio_codec_combo_box = nullptr;

	Menu *app_wnd_menu = nullptr;
	ChildMenu *options_menu = nullptr;

	MenuPoint *preview_chekced_menu_point = nullptr;
	MenuPoint *video_capture_filter_menu_point = nullptr;
	MenuPoint *video_capture_pin_menu_point = nullptr;
	MenuPoint *audio_capture_filter_menu_point = nullptr;
	MenuPoint *audio_capture_pin_menu_point = nullptr;
	MenuPoint *start_recording_menu_point = nullptr;
	MenuPoint *stop_recording_menu_point = nullptr;

	~UIControls()
	{
		SafeRelease(&app_wnd);

		SafeRelease(&video_capture_device_label);
		SafeRelease(&video_codec_label);
		SafeRelease(&audio_capture_device_label);
		SafeRelease(&audio_codec_label);
		SafeRelease(&video_codec_quality_label);

		SafeRelease(&video_codec_quality_edit);

		SafeRelease(&video_capture_device_combo_box);
		SafeRelease(&video_codec_combo_box);
		SafeRelease(&audio_capture_device_combo_box);
		SafeRelease(&audio_codec_combo_box);

		SafeRelease(&app_wnd_menu);
		SafeRelease(&options_menu);

		SafeRelease(&preview_chekced_menu_point);
		SafeRelease(&video_capture_filter_menu_point);
		SafeRelease(&video_capture_pin_menu_point);
		SafeRelease(&audio_capture_filter_menu_point);
		SafeRelease(&audio_capture_pin_menu_point);
		SafeRelease(&start_recording_menu_point);
		SafeRelease(&stop_recording_menu_point);
	}
};

class UI
{
private:
	const wchar_t *app_name = L"ScreenRecorder";
	const wchar_t *app_version = L"v0.4 Release";

	UIControls ui_controls = { 0 };

public:
	const float PREVIEW_WIDTH_COFF = 0.6648f;// 500 / 752
	const int PREVIEW_WIDTH = 500;
	const int CONTROLS_X = PREVIEW_WIDTH + 20;
	const int CONTROLS_Y = 10;
	const int CONTROLS_WIDTH = 222;

	UI();
	//~UI();

	UIControls *QueryUIControls();

	bool WasResized(UINT message);
};

#endif
