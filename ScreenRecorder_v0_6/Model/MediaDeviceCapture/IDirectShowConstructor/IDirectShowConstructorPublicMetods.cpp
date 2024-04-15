#include "IDirectShowConstructor.h"

void IDirectShowConstructor::InitGraph(DSInterface<IGraphBuilder> *graph, DSInterface<ICaptureGraphBuilder2> *graph_builder)
{
	HRESULT hr = S_OK;

	// Create the Filter Graph Manager.
	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **)&graph->object);
	if (FAILED(hr)) throw std::runtime_error("Can't create Graph");

	// Create the Capture Graph Builder.
	hr = CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC_SERVER, IID_ICaptureGraphBuilder2, (void **)&graph_builder->object);
	if (FAILED(hr)) throw std::runtime_error("Can't create GraphBuilder");

	graph_builder->object->SetFiltergraph(graph->object);
}

// Video capture devices
void IDirectShowConstructor::PrintVideoCaptureDevicesInfo()
{
	HRESULT hr = S_OK;
	DSInterface<IEnumMoniker> video_capture_device_list;

	// Show video devices info
	hr = GetElementList(CLSID_VideoInputDeviceCategory, &video_capture_device_list);
	if (SUCCEEDED(hr))
	{
		std::cout << "---------- Video capture devices ----------\r\n";
		PrintElementsInfo(&video_capture_device_list);
	}
	else if (hr == VFW_E_NOT_FOUND) MessageBox(NULL, L"Can't find devices in video input device category", L"Attention", MB_OK);
}

void IDirectShowConstructor::PrintVideoCodecsInfo()
{
	HRESULT hr = S_OK;
	DSInterface<IEnumMoniker> video_codec_list;

	// Show video devices info
	hr = GetElementList(CLSID_VideoCompressorCategory, &video_codec_list);
	if (SUCCEEDED(hr))
	{
		std::cout << "---------- Video codecs ----------\r\n";
		PrintElementsInfo(&video_codec_list);
	}
	else if (hr == VFW_E_NOT_FOUND) MessageBox(NULL, L"Can't find devices in video compressor category", L"Attention", MB_OK);
}

void IDirectShowConstructor::PrintAudioCaptureDevicesInfo()
{
	HRESULT hr = S_OK;
	DSInterface<IEnumMoniker> audio_capture_device_list;

	// Show video devices info
	hr = GetElementList(CLSID_AudioInputDeviceCategory, &audio_capture_device_list);
	if (SUCCEEDED(hr))
	{
		std::cout << "---------- Audio capture devices ----------\r\n";
		PrintElementsInfo(&audio_capture_device_list);
	}
	else if (hr == VFW_E_NOT_FOUND) MessageBox(NULL, L"Can't find devices in audio input device category", L"Attention", MB_OK);
}

void IDirectShowConstructor::PrintAudioCodecsInfo()
{
	HRESULT hr = S_OK;
	DSInterface<IEnumMoniker> audio_codec_list;

	// Show audio devices info
	hr = GetElementList(CLSID_AudioCompressorCategory, &audio_codec_list);
	if (SUCCEEDED(hr))
	{
		std::cout << "---------- Audio codecs ----------\r\n";
		PrintElementsInfo(&audio_codec_list);
	}
	else if (hr == VFW_E_NOT_FOUND) MessageBox(NULL, L"Can't find devices in audio compressor category", L"Attention", MB_OK);
}

void IDirectShowConstructor::GetVideoCaptureDeviceNames(void(*func)(VARIANT *var))
{
	HRESULT hr = S_OK;
	DSInterface<IEnumMoniker> video_capture_device_list;

	// Show audio devices info
	hr = GetElementList(CLSID_VideoInputDeviceCategory, &video_capture_device_list);
	if (SUCCEEDED(hr))
	{
		DSInterface<IMoniker> device_handler;

		while (video_capture_device_list.object->Next(1, &device_handler.object, NULL) == S_OK)
		{
			DSInterface<IPropertyBag> device_properties;

			hr = device_handler.object->BindToStorage(NULL, NULL, IID_PPV_ARGS(&device_properties.object));
			if (FAILED(hr)) continue;

			VARIANT var = { 0 };
			VariantInit(&var);

			// Device friendly name.
			hr = device_properties.object->Read(L"FriendlyName", &var, 0);
			if (SUCCEEDED(hr))
			{
				func(&var);
				VariantClear(&var);
			}

			// Clean up
			device_handler.ReleaseInterface();
		}
	}
	else if (hr == VFW_E_NOT_FOUND) MessageBox(NULL, L"Can't find devices in video input device category", L"Attention", MB_OK);
}

void IDirectShowConstructor::GetVideoCodecNames(void(*func)(VARIANT *var))
{
	HRESULT hr = S_OK;
	DSInterface<IEnumMoniker> video_codec_list;

	// Show audio devices info
	hr = GetElementList(CLSID_VideoCompressorCategory, &video_codec_list);
	if (SUCCEEDED(hr))
	{
		DSInterface<IMoniker> device_handler;

		while (video_codec_list.object->Next(1, &device_handler.object, NULL) == S_OK)
		{
			DSInterface<IPropertyBag> device_properties;

			hr = device_handler.object->BindToStorage(NULL, NULL, IID_PPV_ARGS(&device_properties.object));
			if (FAILED(hr)) continue;

			VARIANT var = { 0 };
			VariantInit(&var);

			// Device friendly name.
			hr = device_properties.object->Read(L"FriendlyName", &var, 0);
			if (SUCCEEDED(hr))
			{
				func(&var);
				VariantClear(&var);
			}

			// Clean up
			device_handler.ReleaseInterface();
		}
	}
	else if (hr == VFW_E_NOT_FOUND) MessageBox(NULL, L"Can't find devices in video compressor category", L"Attention", MB_OK);
}

void IDirectShowConstructor::GetAudioCaptureDeviceNames(void(*func)(VARIANT *var))
{
	HRESULT hr = S_OK;
	DSInterface<IEnumMoniker> audio_capture_device_list;

	// Show audio devices info
	hr = GetElementList(CLSID_AudioInputDeviceCategory, &audio_capture_device_list);
	if (SUCCEEDED(hr))
	{
		DSInterface<IMoniker> device_handler;

		while (audio_capture_device_list.object->Next(1, &device_handler.object, NULL) == S_OK)
		{
			DSInterface<IPropertyBag> device_properties;

			hr = device_handler.object->BindToStorage(NULL, NULL, IID_PPV_ARGS(&device_properties.object));
			if (FAILED(hr)) continue;

			VARIANT var = { 0 };
			VariantInit(&var);

			// Device friendly name.
			hr = device_properties.object->Read(L"FriendlyName", &var, 0);
			if (SUCCEEDED(hr))
			{
				func(&var);
				VariantClear(&var);
			}

			// Clean up
			device_handler.ReleaseInterface();
		}
	}
	else if (hr == VFW_E_NOT_FOUND) MessageBox(NULL, L"Can't find devices in audio input device category", L"Attention", MB_OK);
}

void IDirectShowConstructor::GetAudioCodecNames(void(*func)(VARIANT *var))
{
	HRESULT hr = S_OK;
	DSInterface<IEnumMoniker> audio_codec_list;

	// Show audio devices info
	hr = GetElementList(CLSID_AudioCompressorCategory, &audio_codec_list);
	if (SUCCEEDED(hr))
	{
		DSInterface<IMoniker> device_handler;

		while (audio_codec_list.object->Next(1, &device_handler.object, NULL) == S_OK)
		{
			DSInterface<IPropertyBag> device_properties;

			hr = device_handler.object->BindToStorage(NULL, NULL, IID_PPV_ARGS(&device_properties.object));
			if (FAILED(hr)) continue;

			VARIANT var = { 0 };
			VariantInit(&var);

			// Device friendly name.
			hr = device_properties.object->Read(L"FriendlyName", &var, 0);
			if (SUCCEEDED(hr))
			{
				func(&var);
				VariantClear(&var);
			}

			// Clean up
			device_handler.ReleaseInterface();
		}
	}
	else if (hr == VFW_E_NOT_FOUND) MessageBox(NULL, L"Can't find devices in audio compressor category", L"Attention", MB_OK);
}

HRESULT IDirectShowConstructor::SelectVideoCaptureDevice(uint32_t video_capture_device_index, DSInterface<IBaseFilter> *video_capture_filter, DSInterface<IGraphBuilder> *graph)
{
	HRESULT hr = S_OK;
	DSInterface<IEnumMoniker> video_capture_device_list;

	// if reselect device release old object
	video_capture_filter->ReleaseInterface();

	// Select video device
	hr = GetElementList(CLSID_VideoInputDeviceCategory, &video_capture_device_list);
	if (SUCCEEDED(hr))
	{
		hr = SelectElement(&video_capture_device_list, &video_capture_device_index, video_capture_filter);
		if (SUCCEEDED(hr))
			graph->object->AddFilter(video_capture_filter->object, L"Video Capture Filter");
	}
	//else if (hr == VFW_E_NOT_FOUND) throw std::runtime_error("Can't find devices in video input device category");

	return hr;
}

HRESULT IDirectShowConstructor::SelectVideoCodec(uint32_t video_codec_index, DSInterface<IBaseFilter> *video_codec_filter, DSInterface<IGraphBuilder> *graph)
{
	HRESULT hr = S_OK;
	DSInterface<IEnumMoniker> video_codec_list;

	// if reselect device release old object
	video_codec_filter->ReleaseInterface();

	// Select video device
	hr = GetElementList(CLSID_VideoCompressorCategory, &video_codec_list);
	if (SUCCEEDED(hr))
	{
		hr = SelectElement(&video_codec_list, &video_codec_index, video_codec_filter);
		if (SUCCEEDED(hr))
			graph->object->AddFilter(video_codec_filter->object, L"Video Codec Filter");
	}
	//else if (hr == VFW_E_NOT_FOUND) throw std::runtime_error("Can't find devices in video compressor category");

	return hr;
}

HRESULT IDirectShowConstructor::SelectAudioCaptureDevice(uint32_t audio_capture_device_index, DSInterface<IBaseFilter> *audio_capture_filter, DSInterface<IGraphBuilder> *graph)
{
	HRESULT hr = S_OK;
	DSInterface<IEnumMoniker> audio_capture_device_list;

	// if reselect device release old object
	audio_capture_filter->ReleaseInterface();

	// Select video device
	hr = GetElementList(CLSID_AudioInputDeviceCategory, &audio_capture_device_list);
	if (SUCCEEDED(hr))
	{
		hr = SelectElement(&audio_capture_device_list, &audio_capture_device_index, audio_capture_filter);
		if (SUCCEEDED(hr))
			graph->object->AddFilter(audio_capture_filter->object, L"Audio Capture Filter");
	}
	//else if (hr == VFW_E_NOT_FOUND) throw std::runtime_error("Can't find devices in audio input device category");

	return hr;
}

HRESULT IDirectShowConstructor::SelectAudioCodec(uint32_t audio_codec_index, DSInterface<IBaseFilter> *audio_codec_filter, DSInterface<IGraphBuilder> *graph)
{
	HRESULT hr = S_OK;
	DSInterface<IEnumMoniker> audio_codec_list;

	// if reselect device release old object
	audio_codec_filter->ReleaseInterface();

	// Select audio device
	hr = GetElementList(CLSID_AudioCompressorCategory, &audio_codec_list);
	if (SUCCEEDED(hr))
	{
		hr = SelectElement(&audio_codec_list, &audio_codec_index, audio_codec_filter);
		if (SUCCEEDED(hr))
			graph->object->AddFilter(audio_codec_filter->object, L"Audio Codec Filter");
	}
	//else if (hr == VFW_E_NOT_FOUND) throw std::runtime_error("Can't find devices in audio compressor category");

	return hr;
}

bool IDirectShowConstructor::HasFilterSettings(DSInterface<IBaseFilter> *filter)
{
	bool result = false;

	if (filter->object)
	{
		HRESULT hr = S_OK;
		DSInterface<ISpecifyPropertyPages> specify_property_pages;
		CAUUID cauuid = { 0 };

		hr = filter->object->QueryInterface(IID_PPV_ARGS(&specify_property_pages.object));
		if (SUCCEEDED(hr))
		{
			hr = specify_property_pages.object->GetPages(&cauuid);
			if (SUCCEEDED(hr) && cauuid.cElems > 0) result = true;
		}
	}

	return result;
}

bool IDirectShowConstructor::HasPinSettings(DSInterface<IBaseFilter> *filter, DSInterface<ICaptureGraphBuilder2> *graph_builder)
{
	bool result = false;

	if (filter->object && graph_builder->object)
	{
		HRESULT hr = S_OK;
		DSInterface<IAMStreamConfig> video_stream_config;
		DSInterface<ISpecifyPropertyPages> specify_property_pages;
		CAUUID cauuid = { 0 };

		hr = graph_builder->object->FindInterface(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Interleaved, filter->object, IID_PPV_ARGS(&video_stream_config.object));
		if (FAILED(hr))
		{
			hr = graph_builder->object->FindInterface(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video, filter->object, IID_PPV_ARGS(&video_stream_config.object));
		}

		if (SUCCEEDED(hr))
		{
			hr = video_stream_config.object->QueryInterface(IID_PPV_ARGS(&specify_property_pages.object));
			if (SUCCEEDED(hr))
			{
				hr = specify_property_pages.object->GetPages(&cauuid);
				if (SUCCEEDED(hr) && cauuid.cElems > 0) result = true;
			}
		}
	}

	return result;
}

void IDirectShowConstructor::ShowFilterSettings(DSInterface<IBaseFilter> *filter)
{

	if (filter->object)
	{
		HRESULT hr = S_OK;
		DSInterface<ISpecifyPropertyPages> specify_property_pages;
		CAUUID cauuid = { 0 };

		hr = filter->object->QueryInterface(IID_PPV_ARGS(&specify_property_pages.object));
		if (SUCCEEDED(hr))
		{
			hr = specify_property_pages.object->GetPages(&cauuid);
			if (SUCCEEDED(hr) && cauuid.cElems > 0)
			{
				OleCreatePropertyFrame(NULL, 30, 30, NULL, 1, (IUnknown **)&filter->object, cauuid.cElems, (GUID *)cauuid.pElems, 0, 0, NULL);

				CoTaskMemFree(cauuid.pElems);
			}
		}
	}
}

void IDirectShowConstructor::ShowPinSettings(DSInterface<IBaseFilter> *filter, DSInterface<ICaptureGraphBuilder2> *graph_builder)
{
	if (filter->object && graph_builder->object)
	{
		HRESULT hr = S_OK;
		DSInterface<IAMStreamConfig> video_stream_config;
		DSInterface<ISpecifyPropertyPages> specify_property_pages;
		CAUUID cauuid = { 0 };

		hr = graph_builder->object->FindInterface(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Interleaved, filter->object, IID_PPV_ARGS(&video_stream_config.object));
		if (FAILED(hr))
		{
			hr = graph_builder->object->FindInterface(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video, filter->object, IID_PPV_ARGS(&video_stream_config.object));
		}

		if (SUCCEEDED(hr))
		{
			hr = video_stream_config.object->QueryInterface(IID_PPV_ARGS(&specify_property_pages.object));
			if (SUCCEEDED(hr))
			{
				hr = specify_property_pages.object->GetPages(&cauuid);
				if (SUCCEEDED(hr) && cauuid.cElems > 0)
				{
					OleCreatePropertyFrame(NULL, 30, 30, NULL, 1, (IUnknown **)&video_stream_config.object, cauuid.cElems, (GUID *)cauuid.pElems, 0, 0, NULL);

					CoTaskMemFree(cauuid.pElems);
				}
			}
		}
	}
}

void IDirectShowConstructor::StartStream(DSInterface<IGraphBuilder> *graph)
{
	DSInterface<IMediaControl> media_control;

	graph->object->QueryInterface(IID_PPV_ARGS(&media_control.object));
	media_control.object->Run();
}

void IDirectShowConstructor::StopStream(DSInterface<IGraphBuilder> *graph)
{
	DSInterface<IMediaControl> media_control;

	graph->object->QueryInterface(IID_PPV_ARGS(&media_control.object));
	media_control.object->Stop();
}

double IDirectShowConstructor::GetDefaultVideoCodecQuality(DSInterface<IBaseFilter>* video_codec_filter)
{
	double default_quality = -1.0;

	long compression_params_support = 0l;

	HRESULT hr = E_FAIL;

	DSInterface<IEnumPins> enum_pins;
	DSInterface<IPin> pin;
	DSInterface<IAMVideoCompression> video_compression;

	// Find the pin that supports IAMVideoCompression (if any)
	if (video_codec_filter->object) {
		video_codec_filter->object->EnumPins(&enum_pins.object);
		while (S_OK == enum_pins.object->Next(1, &pin.object, NULL))
		{
			hr = pin.object->QueryInterface(IID_PPV_ARGS(&video_compression.object));
			pin.ReleaseInterface();
			if (SUCCEEDED(hr)) // Found the interface.
			{
				break;
			}
		}

		if (SUCCEEDED(hr)) {
			// Get default values and capabilities.
			hr = video_compression.object->GetInfo(
				0, 0, 0, 0,
				0, 0, &default_quality, &compression_params_support);

			/*if (compression_params_support & CompressionCaps_CanQuality) {
				std::cout << "Default Quality: " << default_quality << "\r\n";
			}
			else std::cout << "Unsupported\r\n";*/
		}
	}

	return default_quality;
}

void IDirectShowConstructor::SetVideoCodecQuality(double video_codec_quality, DSInterface<IBaseFilter>* video_codec_filter)
{
	HRESULT hr = E_FAIL;

	DSInterface<IEnumPins> enum_pins;
	DSInterface<IPin> pin;
	DSInterface<IAMVideoCompression> video_compression;

	// Find the pin that supports IAMVideoCompression (if any)
	if (video_codec_filter->object) {
		video_codec_filter->object->EnumPins(&enum_pins.object);
		while (S_OK == enum_pins.object->Next(1, &pin.object, NULL))
		{
			hr = pin.object->QueryInterface(IID_PPV_ARGS(&video_compression.object));
			pin.ReleaseInterface();
			if (SUCCEEDED(hr)) { // Found the interface.
				break;
			}
		}

		if (SUCCEEDED(hr))
		{
			long compression_params_support = 0l;
			double quality = 0.0, default_quality = 0.0;

			// Get default values and capabilities.
			hr = video_compression.object->GetInfo(
				0, 0, 0, 0,
				0, 0, &default_quality, &compression_params_support);

			if (SUCCEEDED(hr))
			{
				// Get actual values where possible.
				if (compression_params_support & CompressionCaps_CanQuality)
				{
					video_compression.object->put_Quality(video_codec_quality);
					hr = video_compression.object->get_Quality(&quality);
					if (FAILED(hr) || quality < 0) {
						quality = default_quality;
					}
					//std::cout << "Quality: " << quality << "\r\n";
				}
			}
		}
	}
}
