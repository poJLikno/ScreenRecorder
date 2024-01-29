#include "Controller.h"

#include <thread>
#include <chrono>

bool video_quality_flag = false;

LRESULT Controller::DefaultMessages(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
}

void Controller::OnResizeMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
}

void Controller::DisableRecording()
{
	// Disable recording (if need)
	if (ui->QueryUIControls()->stop_recording_menu_point->GetState()) {
		model->StopRecording();

		// Change state for ui
		ui->QueryUIControls()->start_recording_menu_point->ChangeState();
		ui->QueryUIControls()->stop_recording_menu_point->ChangeState();
	}
}

LRESULT Controller::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
			if (ui->WasResized(message)) // Window has Been
			{
				OnResizeMessage(hWnd, message, wParam, lParam);
				result = 0;
			}
			else if (message == WM_COMMAND)
			{
				if (ui != NULL) {
					if (ui->QueryUIControls()->video_capture_device_combo_box->WasChanged(&wParam, &lParam))
					{
						OnVideoCaptureChangeMessage();
					}
					else if (ui->QueryUIControls()->video_codec_combo_box->WasChanged(&wParam, &lParam))
					{
						OnVideoCodecChangeMessage();
					}
					else if (ui->QueryUIControls()->audio_capture_device_combo_box->WasChanged(&wParam, &lParam))
					{
						OnAudioCaptureChangeMessage();
					}
					else if (ui->QueryUIControls()->audio_codec_combo_box->WasChanged(&wParam, &lParam))
					{
						OnAudioCodecChangeMessage();
					}
					else if (ui->QueryUIControls()->preview_chekced_menu_point->WasPressed(&wParam))
					{
						OnPreviewPointMessage();
					}
					else if (ui->QueryUIControls()->video_capture_filter_menu_point->WasPressed(&wParam))
					{
						OnVideoFilterSettingsMessage();
					}
					else if (ui->QueryUIControls()->video_capture_pin_menu_point->WasPressed(&wParam))
					{
						OnVideoPinSettingsMessage();
					}
					else if (ui->QueryUIControls()->audio_capture_filter_menu_point->WasPressed(&wParam))
					{
						OnAudioFilterSettingsMessage();
					}
					else if (ui->QueryUIControls()->audio_capture_pin_menu_point->WasPressed(&wParam))
					{
						OnAudioPinSettingsMessage();
					}
					else if (ui->QueryUIControls()->start_recording_menu_point->WasPressed(&wParam))
					{
						OnRecordingStartMessage();
					}
					else if (ui->QueryUIControls()->stop_recording_menu_point->WasPressed(&wParam))
					{
						OnRecordingStopMessage();
					}
				}
				
				result = 0;
			}
			else
			{
				result = DefaultMessages(hWnd, message, wParam, lParam);
			}
			/*else if (message == WM_KEYDOWN)
			{
				if (wParam == VK_ESCAPE) DestroyWindow(hWnd);
				result = 0;
			}*/
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

void Controller::OnVideoCaptureChangeMessage()
{
	// Disable recording (if need)
	DisableRecording();

	// Change device
	model->QueryModelVars()->video_device.ReleaseDevice();
	model->SelectDevice(ui->QueryUIControls()->video_capture_device_combo_box->GetItemId() - 1, &model->QueryModelVars()->video_device);

	// Check filter settings access
	if (model->QueryModelVars()->video_recorder.HasFilterSettings(&model->QueryModelVars()->video_device)
		&& !ui->QueryUIControls()->video_capture_filter_menu_point->GetState())
	{
		// Set state for ui
		ui->QueryUIControls()->video_capture_filter_menu_point->ChangeState();
	}

	// Check pin settings access
	if (model->QueryModelVars()->video_recorder.HasPinSettings(&model->QueryModelVars()->video_device)
		&& !ui->QueryUIControls()->video_capture_pin_menu_point->GetState())
	{
		// Set state for ui
		ui->QueryUIControls()->video_capture_pin_menu_point->ChangeState();
	}
}

void Controller::OnVideoCodecChangeMessage()
{
	// Disable recording (if need)
	DisableRecording();

	model->QueryModelVars()->video_codec.ReleaseDevice();
	model->SelectDevice(ui->QueryUIControls()->video_codec_combo_box->GetItemId() - 1, &model->QueryModelVars()->video_codec);

	double val = model->QueryModelVars()->video_recorder.GetDefaultVideoCodecQuality();
	if (val >= 0.0) {
		if (!video_quality_flag) video_quality_flag = true;
		ui->QueryUIControls()->video_codec_quality_edit->SetText(std::to_wstring((int)round(val * 100.0)).c_str());
		ShowWindow(ui->QueryUIControls()->video_codec_quality_label->GetHwnd(), SW_NORMAL);
		ShowWindow(ui->QueryUIControls()->video_codec_quality_edit->GetHwnd(), SW_NORMAL);
	}
	else {
		if (video_quality_flag) video_quality_flag = false;
		ShowWindow(ui->QueryUIControls()->video_codec_quality_label->GetHwnd(), SW_HIDE);
		ShowWindow(ui->QueryUIControls()->video_codec_quality_edit->GetHwnd(), SW_HIDE);
	}
}

void Controller::OnAudioCaptureChangeMessage()
{
	// Disable recording (if need)
	DisableRecording();

	// Change device
	model->QueryModelVars()->audio_device.ReleaseDevice();
	model->SelectDevice(ui->QueryUIControls()->audio_capture_device_combo_box->GetItemId() - 1, &model->QueryModelVars()->audio_device);

	// Check filter settings access
	if (model->QueryModelVars()->video_recorder.HasFilterSettings(&model->QueryModelVars()->audio_device)
		&& !ui->QueryUIControls()->audio_capture_filter_menu_point->GetState())
	{
		// Set state for ui
		ui->QueryUIControls()->audio_capture_filter_menu_point->ChangeState();
	}

	// Check pin settings access
	if (model->QueryModelVars()->video_recorder.HasPinSettings(&model->QueryModelVars()->audio_device)
		&& !ui->QueryUIControls()->audio_capture_pin_menu_point->GetState())
	{
		// Set state for ui
		ui->QueryUIControls()->audio_capture_pin_menu_point->ChangeState();
	}
}

void Controller::OnAudioCodecChangeMessage()
{
	// Disable recording (if need)
	DisableRecording();

	model->QueryModelVars()->audio_codec.ReleaseDevice();
	model->SelectDevice(ui->QueryUIControls()->audio_codec_combo_box->GetItemId() - 1, &model->QueryModelVars()->audio_codec);
}

void Controller::OnPreviewPointMessage()
{
	// Disable recording (if need)
	DisableRecording();

	ui->QueryUIControls()->preview_chekced_menu_point->ChangeState();

	if (ui->QueryUIControls()->preview_chekced_menu_point->GetState()) {
		model->ShowPreview();
	}
	else {
		model->HidePreview();
	}
}

void Controller::OnVideoFilterSettingsMessage()
{
	// Disable recording (if need)
	DisableRecording();

	model->ShowVideoFilterSettings();
}

void Controller::OnVideoPinSettingsMessage()
{
	// Disable recording (if need)
	DisableRecording();

	model->ShowVideoPinSettings();
}

void Controller::OnAudioFilterSettingsMessage()
{
	// Disable recording (if need)
	DisableRecording();

	model->ShowAudioFilterSettings();
}

void Controller::OnAudioPinSettingsMessage()
{
	// Disable recording (if need)
	DisableRecording();

	model->ShowAudioPinSettings();
}

void Controller::OnRecordingStartMessage()
{
	//recording_flag = !recording_flag;

	wchar_t quality_str[4] = { 0 };
	ui->QueryUIControls()->video_codec_quality_edit->GetText(quality_str, 4/*non sizeof(quality_str)*/);
	//std::cout << (double)atoi(quality_str) / 100.0 << "\r\n";
	model->QueryModelVars()->video_codec.SetCodecQuality((double)_wtoi(quality_str) / 100.0);

	model->StartRecording();

	ui->QueryUIControls()->start_recording_menu_point->ChangeState();
	ui->QueryUIControls()->stop_recording_menu_point->ChangeState();
}

void Controller::OnRecordingStopMessage()
{
	//recording_flag = !recording_flag;

	/*char quality_str[4] = {0};
	video_codec_quality_edit->GetText(quality_str, sizeof(quality_str));
	//std::cout << (double)atoi(quality_str) / 100.0 << "\r\n";
	media_device_capture->SetVideoQuality((double)atoi(quality_str) / 100.0);*/

	model->StopRecording();

	ui->QueryUIControls()->start_recording_menu_point->ChangeState();
	ui->QueryUIControls()->stop_recording_menu_point->ChangeState();
}

void Controller::InitController()
{
	AddDeviceNamesToList(&model->QueryModelVars()->video_device, ui->QueryUIControls()->video_capture_device_combo_box);
	AddDeviceNamesToList(&model->QueryModelVars()->video_codec, ui->QueryUIControls()->video_codec_combo_box);
	AddDeviceNamesToList(&model->QueryModelVars()->audio_device, ui->QueryUIControls()->audio_capture_device_combo_box);
	AddDeviceNamesToList(&model->QueryModelVars()->audio_codec, ui->QueryUIControls()->audio_codec_combo_box);

	model->QueryModelVars()->preview_window.PutOwner(ui->QueryUIControls()->app_wnd->GetHwnd());
	model->QueryModelVars()->preview_window.PutStyle(WS_VISIBLE | WS_CHILD);
	model->QueryModelVars()->preview_window.PutSize(500u, 281u);
	model->QueryModelVars()->preview_window.PutPos(10u, 10u);

	model->QueryModelVars()->video_recorder.Start();
}

void Controller::AsyncKeyListener()
{
	std::chrono::time_point<std::chrono::steady_clock> millis_timer;
	static bool key_tmp_state_flag = true;
	static bool is_recording = false;

	if (GetAsyncKeyState('K') < 0 && GetAsyncKeyState(VK_MENU) < 0 && key_tmp_state_flag)
	{
		if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - millis_timer).count() >= 1000)
		{
			millis_timer = std::chrono::steady_clock::now();
			if (!is_recording) {
				OnRecordingStartMessage();

				is_recording = true;
			}
			else if (is_recording) {
				OnRecordingStopMessage();

				is_recording = false;
			}

			key_tmp_state_flag = false;
		}
	}
	else if (!GetAsyncKeyState('K') && !key_tmp_state_flag)
	{
		key_tmp_state_flag = true;
	}
	else if (GetAsyncKeyState(VK_ESCAPE) != 0 && GetForegroundWindow() == ui->QueryUIControls()->app_wnd->GetHwnd())
	{
		DestroyWindow(ui->QueryUIControls()->app_wnd->GetHwnd());
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(1));
}
