#ifndef MEDIA_DEVICE_CAPTURE_H
#define MEDIA_DEVICE_CAPTURE_H

#include "IDirectShowConstructor/IDirectShowConstructor.h"

class MediaDeviceCapture : private IDirectShowConstructor
{
private:
	DSInterface<IGraphBuilder> graph_;
	DSInterface<ICaptureGraphBuilder2> graph_builder_;
	DSInterface<IBaseFilter> video_capture_filter_;
	DSInterface<IBaseFilter> video_codec_filter_;
	DSInterface<IBaseFilter> audio_capture_filter_;
	DSInterface<IBaseFilter> audio_codec_filter_;
	DSInterface<IVideoWindow> preview_window_;

	DSInterface<IAMStreamConfig> pVSC;

	bool running_;
	bool owner_flag_, style_flag_, position_flag_, size_flag_;
	bool show_preview_, recording_;
	bool video_capture_filter_settings_flag_, video_capture_pin_settings_flag_;
	bool audio_capture_filter_settings_flag_, audio_capture_pin_settings_flag_;

	HWND owner_hwnd_;
	DWORD style_;
	long x_, y_, width_, height_;
	double quality_;

public:
	using IDirectShowConstructor::GetVideoCaptureDeviceNames;
	using IDirectShowConstructor::GetVideoCodecNames;
	using IDirectShowConstructor::GetAudioCaptureDeviceNames;
	using IDirectShowConstructor::GetAudioCodecNames;

	MediaDeviceCapture()
		: owner_hwnd_(NULL),
		style_(0ul),
		x_(0l), y_(0l),
		width_(0l), height_(0l),
		running_(false),
		owner_flag_(false), style_flag_(false),
		position_flag_(false), size_flag_(false),
		show_preview_(false), recording_(false),
		video_capture_filter_settings_flag_(false), video_capture_pin_settings_flag_(false),
		audio_capture_filter_settings_flag_(false), audio_capture_pin_settings_flag_(false),
		quality_(-1.0) {}
	~MediaDeviceCapture() { Stop(); }

	void SelectVideoCaptureDevice(int video_capture_device_index)
	{
		HRESULT hr = S_OK;

		video_capture_filter_.ReleaseInterface();
		if (video_capture_device_index >= 0)
			hr = IDirectShowConstructor::SelectVideoCaptureDevice(video_capture_device_index, &video_capture_filter_, &graph_);
		if (hr == VFW_E_NOT_FOUND) MessageBox(NULL, L"Can't find devices in video input device category", L"Attention", MB_OK);
	}
	void SelectVideoCodec(int video_codec_index)
	{
		HRESULT hr = S_OK;

		video_codec_filter_.ReleaseInterface();
		if (video_codec_index >= 0)
			hr = IDirectShowConstructor::SelectVideoCodec(video_codec_index, &video_codec_filter_, &graph_);
		if (hr == VFW_E_NOT_FOUND) MessageBox(NULL, L"Can't find devices in video compressor category", L"Attention", MB_OK);
	}
	void SelectAudioCaptureDevice(int audio_capture_device_index)
	{
		HRESULT hr = S_OK;

		audio_capture_filter_.ReleaseInterface();
		if (audio_capture_device_index >= 0)
			hr = IDirectShowConstructor::SelectAudioCaptureDevice(audio_capture_device_index, &audio_capture_filter_, &graph_);
		if (hr == VFW_E_NOT_FOUND) MessageBox(NULL, L"Can't find devices in audio input device category", L"Attention", MB_OK);
	}
	void SelectAudioCodec(int audio_codec_index)
	{
		HRESULT hr = S_OK;

		audio_codec_filter_.ReleaseInterface();
		if (audio_codec_index >= 0)
			hr = IDirectShowConstructor::SelectAudioCodec(audio_codec_index, &audio_codec_filter_, &graph_);
		if (hr == VFW_E_NOT_FOUND) MessageBox(NULL, L"Can't find devices in audio compressor category", L"Attention", MB_OK);
	}

	bool HasVideoCaptureFilterSettings() { return IDirectShowConstructor::HasFilterSettings(&video_capture_filter_); }
	bool HasVideoCapturePinSettings() { return IDirectShowConstructor::HasPinSettings(&video_capture_filter_, &graph_builder_); }
	bool HasAudioCaptureFilterSettings() { return IDirectShowConstructor::HasFilterSettings(&audio_capture_filter_); }
	bool HasAudioCapturePinSettings() { return IDirectShowConstructor::HasPinSettings(&audio_capture_filter_, &graph_builder_); }

	void VideoCaptureFilterSettingsOn(bool on_off) { video_capture_filter_settings_flag_ = on_off; }
	void VideoCapturePinSettingsOn(bool on_off) { video_capture_pin_settings_flag_ = on_off; }
	void AudioCaptureFilterSettingsOn(bool on_off) { audio_capture_filter_settings_flag_ = on_off; }
	void AudioCapturePinSettingsOn(bool on_off) { audio_capture_pin_settings_flag_ = on_off; }

	double GetDefaultVideoQuality() {
		return IDirectShowConstructor::GetDefaultVideoCodecQuality(&video_codec_filter_);
	}

	void SetVideoQuality(double quality) {
		quality_ = quality;
	}

	void Start();
	void Stop();

	void RecordingOn(bool on_off) { recording_ = on_off; }

	void PreviewOn(bool on_off) { show_preview_ = on_off; }
	void SetOwner(HWND *hWnd)
	{
		owner_hwnd_ = *hWnd;
		owner_flag_ = true;
	}
	void SetStyle(DWORD style)
	{
		style_ = style;
		style_flag_ = true;
	}
	void SetPosition(long x, long y)
	{
		x_ = x; 
		y_ = y;
		position_flag_ = true;
	}
	void SetSize(long width, long height)
	{
		width_ = width;
		height_ = height;
		size_flag_ = true;
	}

	void ChangePreviewSizeOnRuntime(long width, long height)
	{
		width_ = width;
		height_ = height;
		size_flag_ = true;

		if (preview_window_.object)
		{
			preview_window_.object->put_Width(width_);
			preview_window_.object->put_Height(height_);
		}
	}
};

#endif
