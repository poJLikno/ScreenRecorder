#ifndef MODEL_H
#define MODEL_H

#include "VideoRecorder/VideoRecorder.h"

class ModelVars
{
public:
	VideoDevice video_device;
	VideoCodec video_codec;
	AudioDevice audio_device;
	AudioCodec audio_codec;

	PreviewWindow preview_window;

	VideoRecorder video_recorder;
};

class Model
{
private:
	ModelVars model_vars;

public:
	Model();
	//~Model();

	ModelVars *QueryModelVars();

	void StartRecording();
	void StopRecording();
	void SelectDevice(unsigned int index, Device *device);
	void ShowPreview();
	void HidePreview();
	void ShowVideoFilterSettings();
	void ShowVideoPinSettings();
	void ShowAudioFilterSettings();
	void ShowAudioPinSettings();
};

#endif