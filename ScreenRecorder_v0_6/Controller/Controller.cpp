#include "Controller.h"

#include <thread>
#include <chrono>

//bool video_quality_flag = false;

Model *Controller::model = nullptr;
UI *Controller::ui = nullptr;

/*LRESULT Controller::DefaultMessages(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	if (message == WM_CTLCOLORSTATIC)// !!! For transparent "Label" !!!
	{
		HDC label_hdc = (HDC)wParam;
		SetBkMode(label_hdc, TRANSPARENT);

		result = 0;
	}
	else if (message == WM_GETMINMAXINFO)
	{
		LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
		lpMMI->ptMinTrackSize.x = 752 + 16;// 16 * 47 +...
		lpMMI->ptMinTrackSize.y = 301 + 59;//423 + 59;// 9 * 47 +...
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

	return result;
}*/

/*void Controller::OnResizeMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static unsigned int msg_wmsize_counter = 0;
	if (msg_wmsize_counter > 1)
	{
		int tmp_wnd_width = LOWORD(lParam);
		int tmp_wnd_height = HIWORD(lParam);

		if (round(2.498f * (float)tmp_wnd_height) != (float)tmp_wnd_width) {
			SetWindowPos(hWnd, NULL, 0, 0, tmp_wnd_width + 16, (int)round(1.f / 2.498f * (float)tmp_wnd_width) + 59, SWP_SHOWWINDOW | SWP_NOZORDER | SWP_NOMOVE);
		}
		int preview_width = (int)round((float)tmp_wnd_width * ui->PREVIEW_WIDTH_COFF);

		// Preview window
		model->QueryModelVars()->preview_window.PutSize((long)preview_width, (long)round(9.f / 16.f * (float)preview_width));

		UIControls *ui_controls = ui->QueryUIControls();
		// Video capture device label
		SetWindowPos(ui_controls->video_capture_device_label->GetHwnd(), NULL,
				20 + preview_width, ui->CONTROLS_Y, 0, 0,
				SWP_SHOWWINDOW | SWP_NOZORDER | SWP_NOSIZE);

		// Video capture device combo box
		SetWindowPos(ui_controls->video_capture_device_combo_box->GetHwnd(), NULL,
				preview_width + 20, ui->CONTROLS_Y + 20, tmp_wnd_width - preview_width - 30, 50,
				SWP_SHOWWINDOW | SWP_NOZORDER);

		// Video codec label
		SetWindowPos(ui_controls->video_codec_label->GetHwnd(), NULL,
				preview_width + 20, ui->CONTROLS_Y + 55, 0, 0,
				SWP_SHOWWINDOW | SWP_NOZORDER | SWP_NOSIZE);

		// Video codec combo box
		SetWindowPos(ui_controls->video_codec_combo_box->GetHwnd(), NULL,
				preview_width + 110, ui->CONTROLS_Y + 50, tmp_wnd_width - preview_width - 120, 50,
				SWP_SHOWWINDOW | SWP_NOZORDER);

		if (video_quality_flag) {
			// Video codec quality label
			SetWindowPos(ui_controls->video_codec_quality_label->GetHwnd(), NULL,
					preview_width + 30, ui->CONTROLS_Y + 80, 0, 0,
					SWP_SHOWWINDOW | SWP_NOZORDER | SWP_NOSIZE);

			// Video codec quality edit
			SetWindowPos(ui_controls->video_codec_quality_edit->GetHwnd(), NULL,
					preview_width + 80, ui->CONTROLS_Y + 80, 0, 0,
					SWP_SHOWWINDOW | SWP_NOZORDER | SWP_NOSIZE);
		}
		else if (!video_quality_flag) {
			// Video codec quality label
			SetWindowPos(ui_controls->video_codec_quality_label->GetHwnd(), NULL,
				preview_width + 30, ui->CONTROLS_Y + 80, 0, 0,
				SWP_NOZORDER | SWP_NOSIZE);

			// Video codec quality edit
			SetWindowPos(ui_controls->video_codec_quality_edit->GetHwnd(), NULL,
				preview_width + 80, ui->CONTROLS_Y + 80, 0, 0,
				SWP_NOZORDER | SWP_NOSIZE);
		}

		// Video capture device label
		SetWindowPos(ui_controls->audio_capture_device_label->GetHwnd(), NULL,
				preview_width + 20, ui->CONTROLS_Y + 110, 0, 0,
				SWP_SHOWWINDOW | SWP_NOZORDER | SWP_NOSIZE);

		// Video capture device combo box
		SetWindowPos(ui_controls->audio_capture_device_combo_box->GetHwnd(), NULL,
				preview_width + 20, ui->CONTROLS_Y + 130, tmp_wnd_width - preview_width - 30, 50,
				SWP_SHOWWINDOW | SWP_NOZORDER);

		// Video codec label
		SetWindowPos(ui_controls->audio_codec_label->GetHwnd(), NULL,
				preview_width + 20, ui->CONTROLS_Y + 165, 0, 0,
				SWP_SHOWWINDOW | SWP_NOZORDER | SWP_NOSIZE);

		// Video codec combo box
		SetWindowPos(ui_controls->audio_codec_combo_box->GetHwnd(), NULL,
				preview_width + 110, ui->CONTROLS_Y + 160, tmp_wnd_width - preview_width - 120, 50,
				SWP_SHOWWINDOW | SWP_NOZORDER);
	}
	else ++msg_wmsize_counter;
}*/

void Controller::DisableRecording()
{
	// Disable recording (if need)
	if (ui->stop_recording_menu_point.GetState()) {
		model->StopRecording();

		// Change state for ui
		ui->start_recording_menu_point.SetState(true);
		ui->stop_recording_menu_point.SetState(false);
	}
}

void Controller::AddDeviceNamesToList(Device *device, ComboBox *combo_box)
{
	wchar_t **device_names = nullptr;
	unsigned int names_num = 0u;
	device->GetDeviceList(&device_names, &names_num);
	if (device_names && names_num) {
		for (unsigned int i = 0u; i < names_num; ++i) {
			combo_box->AddItem((const wchar_t *)device_names[i]);
			free(device_names[i]);
		}
		free(device_names);
		device_names = nullptr;
	}
}

void Controller::OnVideoCaptureChangeMessage(void *ptr) {
	ComboBox &combo_box = WndCallback::GetCallObject<ComboBox>(ptr);

	// Disable recording (if need)
	DisableRecording();

	// Change device
	model->QueryModelVars()->video_device.ReleaseDevice();
	model->SelectDevice(combo_box.GetItemId() - 1, &model->QueryModelVars()->video_device);

	// Check filter settings access
	if (model->QueryModelVars()->video_recorder.HasFilterSettings(&model->QueryModelVars()->video_device)
		&& !ui->video_capture_filter_menu_point.GetState())
	{
		// Set state for ui
		ui->video_capture_filter_menu_point.SetState(true);
	}

	// Check pin settings access
	if (model->QueryModelVars()->video_recorder.HasPinSettings(&model->QueryModelVars()->video_device)
		&& !ui->video_capture_pin_menu_point.GetState())
	{
		// Set state for ui
		ui->video_capture_pin_menu_point.SetState(true);
	}
}

void Controller::OnVideoCodecChangeMessage(void *ptr) {
	ComboBox &combo_box = WndCallback::GetCallObject<ComboBox>(ptr);

	// Disable recording (if need)
	DisableRecording();

	model->QueryModelVars()->video_codec.ReleaseDevice();
	model->SelectDevice(combo_box.GetItemId() - 1, &model->QueryModelVars()->video_codec);

	double val = model->QueryModelVars()->video_recorder.GetDefaultVideoCodecQuality();
	if (val >= 0.0) {
		//if (!video_quality_flag) video_quality_flag = true;
		ui->video_codec_quality_edit.SetWndText(std::to_wstring((int)round(val * 100.0)).c_str());
		ui->video_codec_quality_label.ShowWnd(true);
		ui->video_codec_quality_percent_label.ShowWnd(true);
		ui->video_codec_quality_edit.ShowWnd(true);
	}
	else {
		//if (video_quality_flag) video_quality_flag = false;
		ui->video_codec_quality_label.ShowWnd(false);
		ui->video_codec_quality_percent_label.ShowWnd(false);
		ui->video_codec_quality_edit.ShowWnd(false);
	}
}

void Controller::OnAudioCaptureChangeMessage(void *ptr) {
	ComboBox &combo_box = WndCallback::GetCallObject<ComboBox>(ptr);

	// Disable recording (if need)
	DisableRecording();

	// Change device
	model->QueryModelVars()->audio_device.ReleaseDevice();
	model->SelectDevice(combo_box.GetItemId() - 1, &model->QueryModelVars()->audio_device);

	// Check filter settings access
	if (model->QueryModelVars()->video_recorder.HasFilterSettings(&model->QueryModelVars()->audio_device)
		&& !ui->audio_capture_filter_menu_point.GetState()) {
		// Set state for ui
		ui->audio_capture_filter_menu_point.SetState(true);
	}

	// Check pin settings access
	if (model->QueryModelVars()->video_recorder.HasPinSettings(&model->QueryModelVars()->audio_device)
		&& !ui->audio_capture_pin_menu_point.GetState()) {
		// Set state for ui
		ui->audio_capture_pin_menu_point.SetState(true);
	}
}

void Controller::OnAudioCodecChangeMessage(void *ptr) {
	ComboBox &combo_box = WndCallback::GetCallObject<ComboBox>(ptr);

	// Disable recording (if need)
	DisableRecording();

	model->QueryModelVars()->audio_codec.ReleaseDevice();
	model->SelectDevice(combo_box.GetItemId() - 1, &model->QueryModelVars()->audio_codec);
}

void Controller::OnPreviewPointMessage(void *ptr) {
	MenuPoint &menu_point = WndCallback::GetCallObject<MenuPoint>(ptr);

	// Disable recording (if need)
	DisableRecording();

	menu_point.SetState(!menu_point.GetState());

	if (menu_point.GetState()) {
		model->ShowPreview();
	}
	else {
		model->HidePreview();
	}
}

void Controller::OnVideoFilterSettingsMessage(void *ptr) {
	// Disable recording (if need)
	DisableRecording();

	model->ShowVideoFilterSettings();
}

void Controller::OnVideoPinSettingsMessage(void *ptr) {
	// Disable recording (if need)
	DisableRecording();

	model->ShowVideoPinSettings();
}

void Controller::OnAudioFilterSettingsMessage(void *ptr) {
	// Disable recording (if need)
	DisableRecording();

	model->ShowAudioFilterSettings();
}

void Controller::OnAudioPinSettingsMessage(void *ptr) {
	// Disable recording (if need)
	DisableRecording();

	model->ShowAudioPinSettings();
}

void Controller::OnRecordingStartMessage(void *ptr) {
	try {
		MenuPoint &menu_point = WndCallback::GetCallObject<MenuPoint>(ptr);

		//recording_flag = !recording_flag;

		wchar_t quality_str[4] = { 0 };
		ui->video_codec_quality_edit.GetWndText(quality_str, 4/*non sizeof(quality_str)*/);
		//std::cout << (double)atoi(quality_str) / 100.0 << "\r\n";
		model->QueryModelVars()->video_codec.SetCodecQuality((double)_wtoi(quality_str) / 100.0);

		model->StartRecording();

		menu_point.SetState(false);
		ui->stop_recording_menu_point.SetState(true);
	}
	catch (std::wstring &message) {
		MessageBox(NULL, message.c_str(), L"Attention", MB_OK);
	}
}

void Controller::OnRecordingStopMessage(void *ptr) {
	MenuPoint &menu_point = WndCallback::GetCallObject<MenuPoint>(ptr);

	//recording_flag = !recording_flag;

	/*char quality_str[4] = {0};
	video_codec_quality_edit->GetText(quality_str, sizeof(quality_str));
	//std::cout << (double)atoi(quality_str) / 100.0 << "\r\n";
	media_device_capture->SetVideoQuality((double)atoi(quality_str) / 100.0);*/

	model->StopRecording();

	ui->start_recording_menu_point.SetState(true);
	menu_point.SetState(false);
}

void Controller::InitController()
{
	AddDeviceNamesToList(&model->QueryModelVars()->video_device, &ui->video_capture_device_combo_box);
	AddDeviceNamesToList(&model->QueryModelVars()->video_codec, &ui->video_codec_combo_box);
	AddDeviceNamesToList(&model->QueryModelVars()->audio_device, &ui->audio_capture_device_combo_box);
	AddDeviceNamesToList(&model->QueryModelVars()->audio_codec, &ui->audio_codec_combo_box);

	// Fonts
	{
		ui->normal_font.SetFont(&ui->video_capture_device_label);
		ui->normal_font.SetFont(&ui->video_codec_label);
		ui->normal_font.SetFont(&ui->video_codec_quality_label);
		ui->normal_font.SetFont(&ui->video_codec_quality_percent_label);
		ui->normal_font.SetFont(&ui->video_capture_device_combo_box);
		ui->normal_font.SetFont(&ui->video_codec_combo_box);

		ui->normal_font.SetFont(&ui->video_codec_quality_edit);

		ui->normal_font.SetFont(&ui->audio_capture_device_label);
		ui->normal_font.SetFont(&ui->audio_codec_label);
		ui->normal_font.SetFont(&ui->audio_capture_device_combo_box);
		ui->normal_font.SetFont(&ui->audio_codec_combo_box);
	}

	model->QueryModelVars()->preview_window.PutOwner(ui->app_wnd.GetHwnd());
	model->QueryModelVars()->preview_window.PutStyle(WS_VISIBLE | WS_CHILD);
	model->QueryModelVars()->preview_window.PutSize(500u, 281u);
	model->QueryModelVars()->preview_window.PutPos(10u, 10u);

	model->QueryModelVars()->video_recorder.Start();
}

void Controller::AsyncKeyListener(void *ptr) {
	Window &wnd = WndCallback::GetCallObject<Window>(ptr);

	std::chrono::time_point<std::chrono::steady_clock> millis_timer;
	static bool key_tmp_state_flag = true;
	static bool is_recording = false;

	if (GetAsyncKeyState('K') < 0 && GetAsyncKeyState(VK_MENU) < 0 && key_tmp_state_flag)
	{
		if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - millis_timer).count() >= 1000)
		{
			millis_timer = std::chrono::steady_clock::now();
			if (!is_recording) {
				OnRecordingStartMessage(&ui->start_recording_menu_point);

				is_recording = true;
			}
			else if (is_recording) {
				OnRecordingStopMessage(&ui->stop_recording_menu_point);

				is_recording = false;
			}

			key_tmp_state_flag = false;
		}
	}
	else if (!GetAsyncKeyState('K') && !key_tmp_state_flag)
	{
		key_tmp_state_flag = true;
	}
	else if (GetAsyncKeyState(VK_ESCAPE) != 0 && GetForegroundWindow() == wnd.GetHwnd())
	{
		DestroyWindow(wnd.GetHwnd());
	}

	//std::this_thread::sleep_for(std::chrono::milliseconds(1));
}
