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
	static void DisableRecording();
	static void AddDeviceNamesToList(Device *device, ComboBox *combo_box);	

public:
	static Model *model;
	static UI *ui;

	static void OnVideoCaptureChangeMessage(void *ptr);
	static void OnVideoCodecChangeMessage(void *ptr);
	static void OnAudioCaptureChangeMessage(void *ptr);
	static void OnAudioCodecChangeMessage(void *ptr);
	static void OnPreviewPointMessage(void *ptr);
	static void OnVideoFilterSettingsMessage(void *ptr);
	static void OnVideoPinSettingsMessage(void *ptr);
	static void OnAudioFilterSettingsMessage(void *ptr);
	static void OnAudioPinSettingsMessage(void *ptr);
	static void OnRecordingStartMessage(void *ptr);
	static void OnRecordingStopMessage(void *ptr);

	static void InitController();
	static void AsyncKeyListener(void *ptr);
};

#endif
