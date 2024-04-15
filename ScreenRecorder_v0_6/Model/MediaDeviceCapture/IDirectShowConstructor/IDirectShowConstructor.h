#ifndef IDIRECTSHOW_CONSTRUCTOR
#define IDIRECTSHOW_CONSTRUCTOR

#include <iostream>
#include <string>
#include <windows.h>
#include <dshow.h>
#include <mmsystem.h>
#include <aviriff.h>

#pragma comment(lib, "strmiids.lib")

template<class T>
class DSInterface
{
public:
	T *object;

	DSInterface() : object(nullptr) { }
	~DSInterface()
	{
		DSInterface::ReleaseInterface();
	}

	void ReleaseInterface()
	{
		if (object)
		{
			object->Release();
			object = nullptr;
		}
	}
};

class IDirectShowConstructor
{
protected:
	HRESULT GetElementList(REFGUID element_category, DSInterface<IEnumMoniker> *element_list);
	void PrintElementsInfo(DSInterface<IEnumMoniker> *element_list);

	HRESULT SelectElement(DSInterface<IEnumMoniker> *element_list, uint32_t *element_index, DSInterface<IBaseFilter> *element_filter);

public:
	void InitGraph(DSInterface<IGraphBuilder> *graph, DSInterface<ICaptureGraphBuilder2> *graph_builder);

	void PrintVideoCaptureDevicesInfo();
	void PrintVideoCodecsInfo();
	void PrintAudioCaptureDevicesInfo();
	void PrintAudioCodecsInfo();

	void GetVideoCaptureDeviceNames(void(*func)(VARIANT *var));
	void GetVideoCodecNames(void(*func)(VARIANT *var));
	void GetAudioCaptureDeviceNames(void(*func)(VARIANT *var));
	void GetAudioCodecNames(void(*func)(VARIANT *var));

	HRESULT SelectVideoCaptureDevice(uint32_t video_capture_device_index, DSInterface<IBaseFilter> *video_capture_filter, DSInterface<IGraphBuilder> *graph);
	HRESULT SelectVideoCodec(uint32_t video_codec_index, DSInterface<IBaseFilter> *video_codec_filter, DSInterface<IGraphBuilder> *graph);
	HRESULT SelectAudioCaptureDevice(uint32_t audio_capture_device_index, DSInterface<IBaseFilter> *audio_capture_filter, DSInterface<IGraphBuilder> *graph);
	HRESULT SelectAudioCodec(uint32_t audio_codec_index, DSInterface<IBaseFilter> *audio_codec_filter, DSInterface<IGraphBuilder> *graph);

	bool HasFilterSettings(DSInterface<IBaseFilter> *filter);
	bool HasPinSettings(DSInterface<IBaseFilter> *filter, DSInterface<ICaptureGraphBuilder2> *graph_builder);

	void ShowFilterSettings(DSInterface<IBaseFilter> *filter);
	void ShowPinSettings(DSInterface<IBaseFilter> *filter, DSInterface<ICaptureGraphBuilder2> *graph_builder);

	void StartStream(DSInterface<IGraphBuilder> *graph);
	void StopStream(DSInterface<IGraphBuilder> *graph);

	double GetDefaultVideoCodecQuality(DSInterface<IBaseFilter>* video_codec_filter);
	void SetVideoCodecQuality(double video_codec_quality, DSInterface<IBaseFilter>* video_codec_filter);
};

#endif
