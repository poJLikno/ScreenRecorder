#include "MediaDeviceCapture.h"

#include <thread>

#pragma warning(disable : 4996)

void MediaDeviceCapture::Start()
{
	if (!running_)
	{
		HRESULT hr = S_OK;

		preview_window_.ReleaseInterface();
		graph_.ReleaseInterface();
		graph_builder_.ReleaseInterface();

		IDirectShowConstructor::InitGraph(&graph_, &graph_builder_);

		if (video_capture_filter_.object) graph_.object->AddFilter(video_capture_filter_.object, L"Video Capture Filter");
		if (video_capture_filter_settings_flag_) ShowFilterSettings(&video_capture_filter_);
		if (video_capture_pin_settings_flag_) ShowPinSettings(&video_capture_filter_, &graph_builder_);

		if (audio_capture_filter_.object) graph_.object->AddFilter(audio_capture_filter_.object, L"Audio Capture Filter");
		if (audio_capture_filter_settings_flag_) ShowFilterSettings(&audio_capture_filter_);
		if (audio_capture_pin_settings_flag_) ShowPinSettings(&audio_capture_filter_, &graph_builder_);

		// Set video preview
		if (video_capture_filter_.object && show_preview_)
		{
			hr = graph_.object->QueryInterface(IID_PPV_ARGS(&preview_window_.object));
			if (FAILED(hr)) throw std::runtime_error("Can't get preview window interface");

			hr = graph_builder_.object->RenderStream(&PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video, video_capture_filter_.object, NULL, NULL);
			if (FAILED(hr)) throw std::runtime_error("Can't render video preview stream");

			if (owner_flag_) {
				preview_window_.object->put_Owner((OAHWND)owner_hwnd_);
			}
			if (style_flag_) {
				preview_window_.object->put_WindowStyle(style_);
			}
			if (position_flag_) {
				preview_window_.object->put_Left(x_);
				preview_window_.object->put_Top(y_);
			}
			if (size_flag_) {
				preview_window_.object->put_Width(width_);
				preview_window_.object->put_Height(height_);
			}
		}

		// Set recording
		if (recording_)
		{
			DSInterface<IBaseFilter> avi_mux_filter;

			// Set output file
			if (video_capture_filter_.object || audio_capture_filter_.object)
			{
				size_t thread_id = std::hash<std::thread::id>{}(std::this_thread::get_id());
				SYSTEMTIME record_start_time = { 0 };
				GetLocalTime(&record_start_time);
				std::string a(std::getenv("USERNAME"));
				std::wstring user_name(a.begin(), a.end());

				std::wstring file_name(
					L"C:\\Users\\" + user_name
					+ L"\\Videos\\Recording" + std::to_wstring(thread_id) + L"_"
					+ std::to_wstring(record_start_time.wDay) + L"-"
					+ std::to_wstring(record_start_time.wMonth) + L"-"
					+ std::to_wstring(record_start_time.wYear) + L"_"
					+ std::to_wstring(record_start_time.wHour) + L"-"
					+ std::to_wstring(record_start_time.wMinute) + L"-"
					+ std::to_wstring(record_start_time.wSecond)
					+ L".avi");

				hr = graph_builder_.object->SetOutputFileName(&MEDIASUBTYPE_Avi, file_name.c_str(), &avi_mux_filter.object, NULL);
				if (FAILED(hr)) throw std::runtime_error("Can't set output file name");
			}

			// Set video stream
			if (video_capture_filter_.object)
			{
				if (video_codec_filter_.object) {
					graph_.object->AddFilter(video_codec_filter_.object, L"Video Codec Filter");
					IDirectShowConstructor::SetVideoCodecQuality(quality_, &video_codec_filter_);
					//---
					hr = graph_builder_.object->FindInterface(&PIN_CATEGORY_CAPTURE,
						&MEDIATYPE_Video, video_capture_filter_.object,
						IID_IAMStreamConfig, (void**)&pVSC.object);

					auto delete_media_type = [](AM_MEDIA_TYPE* pmt) {// Start lambda
						// Delete a media type structure that was allocated on the heap.
						if (pmt != NULL)
						{
							if (pmt->cbFormat != 0)
							{
								CoTaskMemFree((PVOID)pmt->pbFormat);
								pmt->cbFormat = 0;
								pmt->pbFormat = NULL;
							}
							if (pmt->pUnk != NULL)
							{
								// pUnk should not be used.
								pmt->pUnk->Release();
								pmt->pUnk = NULL;
							}
							CoTaskMemFree(pmt);
						}
						};// End lambda


					HRESULT hr = S_OK;
					AM_MEDIA_TYPE* media_type = nullptr;

					hr = pVSC.object->GetFormat(&media_type);
					if (FAILED(hr)) {
						delete_media_type(media_type);
						media_type = nullptr;
						throw std::runtime_error("Can't get media type");
					}

					// DV capture does not use a VIDEOINFOHEADER
					if (media_type->formattype == FORMAT_VideoInfo)
					{
						VIDEOINFOHEADER* video_info_header = (VIDEOINFOHEADER*)media_type->pbFormat;
						video_info_header->AvgTimePerFrame = (LONGLONG)(10000000.0 / 60.0/*FPS*/);

						hr = pVSC.object->SetFormat(media_type);
						if (FAILED(hr))
						{
							delete_media_type(media_type);
							media_type = nullptr;

							throw std::runtime_error("Cannot set new frame rate");
						}
						std::cout << "Set fps " << "60\r\n";//fps;
					}
					//DeleteMediaType(pmt);

					delete_media_type(media_type);
					media_type = nullptr;
					//---
				}
				hr = graph_builder_.object->RenderStream(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video, video_capture_filter_.object, video_codec_filter_.object, avi_mux_filter.object);
				if (FAILED(hr)) throw std::runtime_error("Can't render video stream");
			}

			// Set audio stream
			if (audio_capture_filter_.object)
			{
				if (audio_codec_filter_.object) graph_.object->AddFilter(audio_codec_filter_.object, L"Audio Codec Filter");
				hr = graph_builder_.object->RenderStream(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Audio, audio_capture_filter_.object, audio_codec_filter_.object, avi_mux_filter.object);
				if (FAILED(hr)) throw std::runtime_error("Can't render audio stream");
			}

			// Streams settings
			if (video_capture_filter_.object && audio_capture_filter_.object)
			{
				DSInterface<IConfigAviMux> config_avi_mux;
				DSInterface<IConfigInterleaving> config_interleaveing;

				// Set main stream (audio stream prefered)
				hr = avi_mux_filter.object->QueryInterface(IID_PPV_ARGS(&config_avi_mux.object));
				if (SUCCEEDED(hr))
				{
					config_avi_mux.object->SetMasterStream(1);
				}

				// Set interleaving mode
				hr = avi_mux_filter.object->QueryInterface(IID_PPV_ARGS(&config_interleaveing.object));
				if (SUCCEEDED(hr))
				{
					config_interleaveing.object->put_Mode(INTERLEAVE_NONE);//INTERLEAVE_NONE);
				}
			}
		}

		running_ = true;

		StartStream(&graph_);
	}
}

void MediaDeviceCapture::Stop()
{
	if (running_)
	{
		StopStream(&graph_);
		running_ = false;
	}
}
