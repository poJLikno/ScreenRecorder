#include "UI.h"

#include "../Controller/Controller.h"

UI::UI()
{
	//Window
	ui_controls.app_wnd = new Window(
			app_name, true,
			CW_USEDEFAULT, CW_USEDEFAULT, 768/*752*/, 301 + 59/*482/*423*/,
			Controller::WndProc);//230 + 59
	ui_controls.app_wnd->SetLabel(std::wstring(app_name + (std::wstring)L" " + app_version).c_str());

	HWND app_wnd_hwnd = ui_controls.app_wnd->GetHwnd();

	// Menus
	{
		ui_controls.app_wnd_menu = new Menu();
		ui_controls.options_menu = new ChildMenu(L"Options", ui_controls.app_wnd_menu);
		{
			ui_controls.preview_chekced_menu_point = new MenuPoint(L"Preview", ui_controls.options_menu, Checked);
			MenuPoint::AddSeparator(ui_controls.options_menu);//
			ui_controls.video_capture_filter_menu_point = new MenuPoint(L"Video capture filter settings", ui_controls.options_menu);
			ui_controls.video_capture_pin_menu_point = new MenuPoint(L"Video capture pin settings", ui_controls.options_menu);
			MenuPoint::AddSeparator(ui_controls.options_menu);//
			ui_controls.audio_capture_filter_menu_point = new MenuPoint(L"Audio capture filter settings", ui_controls.options_menu);
			ui_controls.audio_capture_pin_menu_point = new MenuPoint(L"Audio capture pin settings", ui_controls.options_menu);
			MenuPoint::AddSeparator(ui_controls.options_menu);//
			ui_controls.start_recording_menu_point = new MenuPoint(L"Start recording (key: Alt + K)", ui_controls.options_menu);
			ui_controls.stop_recording_menu_point = new MenuPoint(L"Stop recording (key: Alt + K)", ui_controls.options_menu);
		}
		ui_controls.app_wnd_menu->AttachToWindow(app_wnd_hwnd);
	}

	// Labels
	{
		// Video
		ui_controls.video_capture_device_label = new Label(
				app_wnd_hwnd, L"Video capture device: ",
				CONTROLS_X, CONTROLS_Y, 130, 17);

		ui_controls.video_codec_label = new Label(
				app_wnd_hwnd, L"Video codec: ",
				CONTROLS_X, CONTROLS_Y + 55, 80, 17);


		ui_controls.video_codec_quality_label = new Label(
				app_wnd_hwnd, L"Quality: ",
				CONTROLS_X + 10, CONTROLS_Y + 80, 50, 17);

		// Audio
		ui_controls.audio_capture_device_label = new Label(
				app_wnd_hwnd, L"Audio capture device: ",
				CONTROLS_X, CONTROLS_Y + 110, 130, 17);

		ui_controls.audio_codec_label = new Label(
				app_wnd_hwnd, L"Audio codec: ",
				CONTROLS_X, CONTROLS_Y + 165, 80, 17);
	}

	// Edits
	{
		ui_controls.video_codec_quality_edit = new Edit(
				app_wnd_hwnd,
				CONTROLS_X + 60, CONTROLS_Y + 80, 30, 17,
				ES_NUMBER | WS_VISIBLE | WS_CHILD | WS_BORDER/* | ES_AUTOHSCROLL*/ | ES_LEFT);
	}

	// ComboBoxes
	{
		// Video
		ui_controls.video_capture_device_combo_box = new ComboBox(app_wnd_hwnd, CONTROLS_X, CONTROLS_Y + 20, CONTROLS_WIDTH, 50);
		ui_controls.video_capture_device_combo_box->AddItem(L"None");

		ui_controls.video_codec_combo_box = new ComboBox(app_wnd_hwnd, CONTROLS_X + 90, CONTROLS_Y + 50, CONTROLS_WIDTH - 90, 50);
		ui_controls.video_codec_combo_box->AddItem(L"None");

		// Audio
		ui_controls.audio_capture_device_combo_box = new ComboBox(app_wnd_hwnd, CONTROLS_X, CONTROLS_Y + 130, CONTROLS_WIDTH, 50);
		ui_controls.audio_capture_device_combo_box->AddItem(L"None");

		ui_controls.audio_codec_combo_box = new ComboBox(app_wnd_hwnd, CONTROLS_X + 90, CONTROLS_Y + 160, CONTROLS_WIDTH - 90, 50);
		ui_controls.audio_codec_combo_box->AddItem(L"None");
	}

	// Fonts
	NormalFont normal_font;
	{
		normal_font.SetFont(ui_controls.video_capture_device_label->GetHwnd());
		normal_font.SetFont(ui_controls.video_codec_label->GetHwnd());
		normal_font.SetFont(ui_controls.video_codec_quality_label->GetHwnd());
		normal_font.SetFont(ui_controls.video_capture_device_combo_box->GetHwnd());
		normal_font.SetFont(ui_controls.video_codec_combo_box->GetHwnd());

		normal_font.SetFont(ui_controls.video_codec_quality_edit->GetHwnd());

		normal_font.SetFont(ui_controls.audio_capture_device_label->GetHwnd());
		normal_font.SetFont(ui_controls.audio_codec_label->GetHwnd());
		normal_font.SetFont(ui_controls.audio_capture_device_combo_box->GetHwnd());
		normal_font.SetFont(ui_controls.audio_codec_combo_box->GetHwnd());
	}

	// Default settings
	// Video codec quality controls
	//ui_controls.video_codec_quality_edit->SetText(L"0");
	//ShowWindow(ui_controls.video_codec_quality_label->GetHwnd(), SW_HIDE);
	//ShowWindow(ui_controls.video_codec_quality_edit->GetHwnd(), SW_HIDE);

	// Video
	ui_controls.video_capture_device_combo_box->SelectItem(0);
	ui_controls.video_codec_combo_box->SelectItem(0);

	// Audio
	ui_controls.audio_capture_device_combo_box->SelectItem(0);
	ui_controls.audio_codec_combo_box->SelectItem(0);

	// Device settings menu points
	ui_controls.video_capture_filter_menu_point->ChangeState();
	ui_controls.video_capture_pin_menu_point->ChangeState();
	ui_controls.audio_capture_filter_menu_point->ChangeState();
	ui_controls.audio_capture_pin_menu_point->ChangeState();

	// Stop recording menu point
	ui_controls.stop_recording_menu_point->ChangeState();
}

UIControls *UI::QueryUIControls()
{
	return &ui_controls;
}

bool UI::WasResized(UINT message)
{
	if (message == WM_SIZE) return true;
	return false;
}
