#ifndef UI_H
#define UI_H

#include "../resource.h"

#include "WindowLib/Window.h"
#include "WindowLib/NormalFont.h"

class UI
{
private:
	const wchar_t *app_name = L"ScreenRecorder";
	const wchar_t *app_version = L"v0.6 Release";

public:
	const float PREVIEW_WIDTH_COFF = 0.6648f;// 500 / 752
	const int PREVIEW_WIDTH = 500;
	const int CONTROLS_X = PREVIEW_WIDTH + 20;
	const int CONTROLS_Y = 10;
	const int CONTROLS_WIDTH = 222;

	// Main window
	Window app_wnd = Window(app_name,
		WndPairValue{ CW_USEDEFAULT, CW_USEDEFAULT },
		WndPairValue{ 768/*752*/, 301 + 59/*482/*423*/ },
		IDI_ICON1);

	// Labels
	Label video_capture_device_label = Label(L"Video capture device: ",
		WndPairValue{ CONTROLS_X, CONTROLS_Y }, WndPairValue{ 130, 17 });

	Label video_codec_label = Label(L"Video codec: ",
		WndPairValue{ CONTROLS_X, CONTROLS_Y + 55 }, WndPairValue{ 80, 17 });

	Label video_codec_quality_label = Label(L"Quality: ",
		WndPairValue{ CONTROLS_X + 10, CONTROLS_Y + 80 }, WndPairValue{ 42, 17 });

	Label video_codec_quality_percent_label = Label(L"%",
		WndPairValue{ CONTROLS_X + 95, CONTROLS_Y + 80 }, WndPairValue{ 12, 17 });

	Label audio_capture_device_label = Label(L"Audio capture device: ",
		WndPairValue{ CONTROLS_X, CONTROLS_Y + 110 }, WndPairValue{ 130, 17 });

	Label audio_codec_label = Label(L"Audio codec: ",
		WndPairValue{ CONTROLS_X, CONTROLS_Y + 165 }, WndPairValue{ 80, 17 });

	// Edit
	Edit video_codec_quality_edit = Edit(
		WndPairValue{ CONTROLS_X + 60, CONTROLS_Y + 80 }, WndPairValue{ 30, 17 });

	// Combo boxes
	ComboBox video_capture_device_combo_box = ComboBox(
		WndPairValue{ CONTROLS_X, CONTROLS_Y + 20 }, WndPairValue{ CONTROLS_WIDTH, 50 });

	ComboBox video_codec_combo_box = ComboBox(
		WndPairValue{ CONTROLS_X + 90, CONTROLS_Y + 50 }, WndPairValue{ CONTROLS_WIDTH - 90, 50 });

	ComboBox audio_capture_device_combo_box = ComboBox(
		WndPairValue{ CONTROLS_X, CONTROLS_Y + 130 }, WndPairValue{ CONTROLS_WIDTH, 50 });

	ComboBox audio_codec_combo_box = ComboBox(
		WndPairValue{ CONTROLS_X + 90, CONTROLS_Y + 160 }, WndPairValue{ CONTROLS_WIDTH - 90, 50 });

	// Menus
	MenuPoint preview_chekced_menu_point = MenuPoint(L"Preview", Checked);
	MenuPoint video_capture_filter_menu_point = MenuPoint(L"Video capture filter settings");
	MenuPoint video_capture_pin_menu_point = MenuPoint(L"Video capture pin settings");
	MenuPoint audio_capture_filter_menu_point = MenuPoint(L"Audio capture filter settings");
	MenuPoint audio_capture_pin_menu_point = MenuPoint(L"Audio capture pin settings");
	MenuPoint start_recording_menu_point = MenuPoint(L"Start recording (key: Alt + K)");
	MenuPoint stop_recording_menu_point = MenuPoint(L"Stop recording (key: Alt + K)");

	PopupMenu options_menu = PopupMenu(L"Options");
	Menu app_wnd_menu;

	// Font
	NormalFont normal_font;

	UI();
	~UI() = default;
};

#endif
