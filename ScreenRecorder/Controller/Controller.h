#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <iostream>
#include <string>
#include <windows.h>

#include "../Model/Model.h"
#include "../UI/UI.h"

class Controller
{
protected:
	static LRESULT DefaultMessages(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	static void DisableRecording();
	static void AddDeviceNamesToList(Device *device, ComboBox *combo_box);

	static void OnResizeMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static void OnVideoCaptureChangeMessage();
	static void OnVideoCodecChangeMessage();
	static void OnAudioCaptureChangeMessage();
	static void OnAudioCodecChangeMessage();
	static void OnPreviewPointMessage();
	static void OnVideoFilterSettingsMessage();
	static void OnVideoPinSettingsMessage();
	static void OnAudioFilterSettingsMessage();
	static void OnAudioPinSettingsMessage();
	static void OnRecordingStartMessage();
	static void OnRecordingStopMessage();

public:
	static Model *model;
	static UI *ui;

	static void InitController();
	static void AsyncKeyListener();
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

#endif
