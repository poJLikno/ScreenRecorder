#ifndef VIDEO_RECORDER_H
#define VIDEO_RECORDER_H

#include <windows.h>

#include "BaseFilterDevices.h"
#include "PreviewWindow.h"

class VideoRecorder
{
private:
	DSSafePtr<IGraphBuilder> graph;
	DSSafePtr<ICaptureGraphBuilder2> graph_builder;

	VideoDevice *video_device = nullptr;
	VideoCodec *video_codec = nullptr;
	AudioDevice *audio_device = nullptr;
	AudioCodec *audio_codec = nullptr;

	PreviewWindow *preview_window = nullptr;

	bool is_running = false;

	bool video_filter_settings = false;
	bool video_pin_settings = false;
	bool audio_filter_settings = false;
	bool audio_pin_settings = false;
	bool preview_allow = true;
	bool recording_allow = false;
	

	void InitGraph(DSSafePtr<IGraphBuilder> *graph, DSSafePtr<ICaptureGraphBuilder2> *graph_builder);
	void RegisterDevice(DSSafePtr<IBaseFilter> *device, const wchar_t *device_name);
	void SetPreview();
	void SetOutputFile(DSSafePtr<IBaseFilter> *avi_mux_filter);
	void SetStreams(DSSafePtr<IBaseFilter> *avi_mux_filter);
	void StartStream(DSSafePtr<IGraphBuilder> *graph);
	void StopStream(DSSafePtr<IGraphBuilder> *graph);
	void ShowFilterSettings(DSSafePtr<IBaseFilter> *device);
	void ShowPinSettings(DSSafePtr<IBaseFilter> *device);

public:
	~VideoRecorder() {
		video_device = nullptr;
		video_codec = nullptr;
		audio_device = nullptr;
		audio_codec = nullptr;

		preview_window = nullptr;
	}

	void Start();
	void Stop();

	void AttachDevice(Device *device_ptr);
	void AttachPreviewWindow(PreviewWindow *wnd_ptr);

	void ShowVideoFilterSettings(bool state);
	void ShowVideoPinSettings(bool state);
	void ShowAudioFilterSettings(bool state);
	void ShowAudioPinSettings(bool state);

	void AllowPreview(bool state);
	void AllowRecording(bool state);

	bool HasFilterSettings(Device *device);
	bool HasPinSettings(Device *device);

	double GetDefaultVideoCodecQuality();
};

#endif
