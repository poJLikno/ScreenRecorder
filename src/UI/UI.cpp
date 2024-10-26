#include "UI.h"

#include "../Controller/Controller.h"

UI::UI()
{
    Controller::ui = this;

    app_wnd.SetWndText(std::wstring(app_name + (std::wstring)L" " + app_version).c_str());
    app_wnd.EnableControlsDialogMessages(true);
    app_wnd.SetAsyncWndLoopCallback(Controller::AsyncKeyListener);

    // Menus
    {
        // callbacks
        preview_chekced_menu_point.SetOnClickCallback(Controller::OnPreviewPointMessage);
        video_capture_filter_menu_point.SetOnClickCallback(Controller::OnVideoFilterSettingsMessage);
        video_capture_pin_menu_point.SetOnClickCallback(Controller::OnVideoPinSettingsMessage);
        audio_capture_filter_menu_point.SetOnClickCallback(Controller::OnAudioFilterSettingsMessage);
        audio_capture_pin_menu_point.SetOnClickCallback(Controller::OnAudioPinSettingsMessage);
        start_recording_menu_point.SetOnClickCallback(Controller::OnRecordingStartMessage);
    stop_recording_menu_point.SetOnClickCallback(Controller::OnRecordingStopMessage);

        // Attach menu points
        options_menu.AttachMenuPoint(&preview_chekced_menu_point);
        options_menu.AppendSeparator();
        options_menu.AttachMenuPoint(&video_capture_filter_menu_point);
        options_menu.AttachMenuPoint(&video_capture_pin_menu_point);
        options_menu.AppendSeparator();
        options_menu.AttachMenuPoint(&audio_capture_filter_menu_point);
        options_menu.AttachMenuPoint(&audio_capture_pin_menu_point);
        options_menu.AppendSeparator();
        options_menu.AttachMenuPoint(&start_recording_menu_point);
        options_menu.AttachMenuPoint(&stop_recording_menu_point);

        app_wnd_menu.AttachPopupMenu(&options_menu);
    }

    // Labels
    {
        // callbacks
        video_capture_device_label.SetOnResizeCallback([](void *ptr) {
                Label &label = WndCallback::GetCallObject<Label>(ptr);
                Window &wnd = ((WndSizeParams *)*label.SomeDataPtr())->_wnd;
                int tmp_wnd_width = wnd.GetWndSize().first;
                int tmp_wnd_height = wnd.GetWndSize().second;
                int preview_width = (int)round((float)tmp_wnd_width * Controller::ui->PREVIEW_WIDTH_COFF);

                // Video capture device label
                label.SetWndPos(WndPairValue{ preview_width + 20, Controller::ui->CONTROLS_Y });
            });
        video_codec_label.SetOnResizeCallback([](void *ptr) {
                Label &label = WndCallback::GetCallObject<Label>(ptr);
                Window &wnd = ((WndSizeParams *)*label.SomeDataPtr())->_wnd;
                int tmp_wnd_width = wnd.GetWndSize().first;
                int tmp_wnd_height = wnd.GetWndSize().second;
                int preview_width = (int)round((float)tmp_wnd_width * Controller::ui->PREVIEW_WIDTH_COFF);

                // Video codec label
                label.SetWndPos(WndPairValue{ preview_width + 20, Controller::ui->CONTROLS_Y + 55 });
            });
        video_codec_quality_label.SetOnResizeCallback([](void *ptr) {
                Label &label = WndCallback::GetCallObject<Label>(ptr);
                Window &wnd = ((WndSizeParams *)*label.SomeDataPtr())->_wnd;
                int tmp_wnd_width = wnd.GetWndSize().first;
                int tmp_wnd_height = wnd.GetWndSize().second;
                int preview_width = (int)round((float)tmp_wnd_width * Controller::ui->PREVIEW_WIDTH_COFF);

                // Video codec quality label
                label.SetWndPos(WndPairValue{ preview_width + 30, Controller::ui->CONTROLS_Y + 80 });
            });
        video_codec_quality_percent_label.SetOnResizeCallback([](void *ptr) {
                Label &label = WndCallback::GetCallObject<Label>(ptr);
                Window &wnd = ((WndSizeParams *)*label.SomeDataPtr())->_wnd;
                int tmp_wnd_width = wnd.GetWndSize().first;
                int tmp_wnd_height = wnd.GetWndSize().second;
                int preview_width = (int)round((float)tmp_wnd_width * Controller::ui->PREVIEW_WIDTH_COFF);

                // Video codec quality label
                label.SetWndPos(WndPairValue{ preview_width + 115, Controller::ui->CONTROLS_Y + 80 });
            });
        audio_capture_device_label.SetOnResizeCallback([](void *ptr) {
                Label &label = WndCallback::GetCallObject<Label>(ptr);
                Window &wnd = ((WndSizeParams *)*label.SomeDataPtr())->_wnd;
                int tmp_wnd_width = wnd.GetWndSize().first;
                int tmp_wnd_height = wnd.GetWndSize().second;
                int preview_width = (int)round((float)tmp_wnd_width * Controller::ui->PREVIEW_WIDTH_COFF);
                
                // Audio capture device label
                label.SetWndPos(WndPairValue{ preview_width + 20, Controller::ui->CONTROLS_Y + 110 });
            });
        audio_codec_label.SetOnResizeCallback([](void *ptr) {
                Label &label = WndCallback::GetCallObject<Label>(ptr);
                Window &wnd = ((WndSizeParams *)*label.SomeDataPtr())->_wnd;
                int tmp_wnd_width = wnd.GetWndSize().first;
                int tmp_wnd_height = wnd.GetWndSize().second;
                int preview_width = (int)round((float)tmp_wnd_width * Controller::ui->PREVIEW_WIDTH_COFF);
                
                // Audio codec label
                label.SetWndPos(WndPairValue{ preview_width + 20, Controller::ui->CONTROLS_Y + 165 });

                // Preview window
                Controller::model->QueryModelVars()->preview_window.PutSize((long)preview_width, (long)round(9.f / 16.f * (float)preview_width));

                //Window
                if (round(2.498f * (float)tmp_wnd_height) != (float)tmp_wnd_width) {
                    wnd.SetWndSize(WndPairValue{ tmp_wnd_width + 16, (int)round(1.f / 2.498f * (float)tmp_wnd_width) + 59 });
                }
            });
    }

    // Edits
    {
        // callbacks
        video_codec_quality_edit.SetOnResizeCallback([](void *ptr) {
                Edit &edit = WndCallback::GetCallObject<Edit>(ptr);
                Window &wnd = ((WndSizeParams *)*edit.SomeDataPtr())->_wnd;
                int tmp_wnd_width = wnd.GetWndSize().first;
                int tmp_wnd_height = wnd.GetWndSize().second;
                int preview_width = (int)round((float)tmp_wnd_width * Controller::ui->PREVIEW_WIDTH_COFF);

                // Video codec quality edit
                edit.SetWndPos(WndPairValue{ preview_width + 80, Controller::ui->CONTROLS_Y + 80 });
            });
    }

    // ComboBoxes
    {
        video_capture_device_combo_box.SetOnChangeCallback(Controller::OnVideoCaptureChangeMessage);
        video_codec_combo_box.SetOnChangeCallback(Controller::OnVideoCodecChangeMessage);
        audio_capture_device_combo_box.SetOnChangeCallback(Controller::OnAudioCaptureChangeMessage);
        audio_codec_combo_box.SetOnChangeCallback(Controller::OnAudioCodecChangeMessage);
        // callbacks
        video_capture_device_combo_box.SetOnResizeCallback([](void *ptr) {
                ComboBox &combo_box = WndCallback::GetCallObject<ComboBox>(ptr);
                Window &wnd = ((WndSizeParams *)*combo_box.SomeDataPtr())->_wnd;
                int tmp_wnd_width = wnd.GetWndSize().first;
                int tmp_wnd_height = wnd.GetWndSize().second;
                int preview_width = (int)round((float)tmp_wnd_width * Controller::ui->PREVIEW_WIDTH_COFF);

                // Video capture device combo box
                combo_box.SetWndPos(WndPairValue{ preview_width + 20, Controller::ui->CONTROLS_Y + 20 });
                combo_box.SetWndSize(WndPairValue{ tmp_wnd_width - preview_width - 30, 50 });
            });
        video_codec_combo_box.SetOnResizeCallback([](void *ptr) {
                ComboBox &combo_box = WndCallback::GetCallObject<ComboBox>(ptr);
                Window &wnd = ((WndSizeParams *)*combo_box.SomeDataPtr())->_wnd;
                int tmp_wnd_width = wnd.GetWndSize().first;
                int tmp_wnd_height = wnd.GetWndSize().second;
                int preview_width = (int)round((float)tmp_wnd_width * Controller::ui->PREVIEW_WIDTH_COFF);

                // Video codec combo box
                combo_box.SetWndPos(WndPairValue{ preview_width + 110, Controller::ui->CONTROLS_Y + 50 });
                combo_box.SetWndSize(WndPairValue{ tmp_wnd_width - preview_width - 120, 50 });
            });
        audio_capture_device_combo_box.SetOnResizeCallback([](void *ptr) {//
                ComboBox &combo_box = WndCallback::GetCallObject<ComboBox>(ptr);
                Window &wnd = ((WndSizeParams *)*combo_box.SomeDataPtr())->_wnd;
                int tmp_wnd_width = wnd.GetWndSize().first;
                int tmp_wnd_height = wnd.GetWndSize().second;
                int preview_width = (int)round((float)tmp_wnd_width * Controller::ui->PREVIEW_WIDTH_COFF);

                // Audio capture device combo box
                combo_box.SetWndPos(WndPairValue{ preview_width + 20, Controller::ui->CONTROLS_Y + 130 });
                combo_box.SetWndSize(WndPairValue{ tmp_wnd_width - preview_width - 30, 50 });
            });
        audio_codec_combo_box.SetOnResizeCallback([](void *ptr) {
                ComboBox &combo_box = WndCallback::GetCallObject<ComboBox>(ptr);
                Window &wnd = ((WndSizeParams *)*combo_box.SomeDataPtr())->_wnd;
                int tmp_wnd_width = wnd.GetWndSize().first;
                int tmp_wnd_height = wnd.GetWndSize().second;
                int preview_width = (int)round((float)tmp_wnd_width * Controller::ui->PREVIEW_WIDTH_COFF);

                // Audio codec combo box
                combo_box.SetWndPos(WndPairValue{ preview_width + 110, Controller::ui->CONTROLS_Y + 160 });
                combo_box.SetWndSize(WndPairValue{ tmp_wnd_width - preview_width - 120, 50 });
            });
    }

    // Attach controls
    app_wnd.AttachChildControl(&video_capture_device_label);
    app_wnd.AttachChildControl(&video_codec_label);
    app_wnd.AttachChildControl(&video_codec_quality_label);
    app_wnd.AttachChildControl(&video_codec_quality_percent_label);
    app_wnd.AttachChildControl(&audio_capture_device_label);
    app_wnd.AttachChildControl(&audio_codec_label);

    app_wnd.AttachChildControl(&video_capture_device_combo_box);
    app_wnd.AttachChildControl(&video_codec_combo_box);
    app_wnd.AttachChildControl(&video_codec_quality_edit);
    app_wnd.AttachChildControl(&audio_capture_device_combo_box);
    app_wnd.AttachChildControl(&audio_codec_combo_box);

    app_wnd.AttachMenu(&app_wnd_menu);

    // Default settings
    video_capture_device_combo_box.AddItem(L"None");
    video_codec_combo_box.AddItem(L"None");
    audio_capture_device_combo_box.AddItem(L"None");
    audio_codec_combo_box.AddItem(L"None");

    video_capture_device_combo_box.SelectItem(0);
    video_codec_combo_box.SelectItem(0);
    audio_capture_device_combo_box.SelectItem(0);
    audio_codec_combo_box.SelectItem(0);

    //video_codec_quality_edit.SetWndStyle(ES_NUMBER | WS_VISIBLE | WS_CHILD | WS_BORDER/* | ES_AUTOHSCROLL*/ | ES_LEFT);
    video_codec_quality_edit.SetWndText(L"0");

    video_codec_quality_label.ShowWnd(false);
    video_codec_quality_percent_label.ShowWnd(false);
    video_codec_quality_edit.ShowWnd(false);
    
    video_capture_filter_menu_point.SetState(false);
    video_capture_pin_menu_point.SetState(false);
    audio_capture_filter_menu_point.SetState(false);
    audio_capture_pin_menu_point.SetState(false);
    stop_recording_menu_point.SetState(false);
}
