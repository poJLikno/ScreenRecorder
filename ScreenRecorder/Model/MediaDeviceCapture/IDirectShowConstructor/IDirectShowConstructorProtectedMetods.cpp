#include "IDirectShowConstructor.h"

HRESULT IDirectShowConstructor::GetElementList(REFGUID element_category, DSInterface<IEnumMoniker> *element_list)
{
	HRESULT hr = S_OK;
	DSInterface <ICreateDevEnum> element_parser;

	// Init device parser
	hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&element_parser.object));
	if (FAILED(hr)) throw std::runtime_error("Can't create device parser");

	// Create an enumerator for the category.
	hr = element_parser.object->CreateClassEnumerator(element_category, &element_list->object, 0);
	if (FAILED(hr)) hr = VFW_E_NOT_FOUND;// have no devices in this category

	return hr;
}

void IDirectShowConstructor::PrintElementsInfo(DSInterface<IEnumMoniker> *element_list)
{
	HRESULT hr = S_OK;
	DSInterface<IMoniker> device_handler;

	uint32_t device_index_counter = 0;

	while (element_list->object->Next(1, &device_handler.object, NULL) == S_OK)
	{
		DSInterface<IPropertyBag> device_properties;

		hr = device_handler.object->BindToStorage(NULL, NULL, IID_PPV_ARGS(&device_properties.object));
		if (FAILED(hr)) continue;

		VARIANT var = { 0 };
		VariantInit(&var);

		std::cout << "Device index -> [" << device_index_counter++ << "]\r\n";

		// Device friendly name.
		hr = device_properties.object->Read(L"FriendlyName", &var, 0);
		if (SUCCEEDED(hr))
		{
			std::wcout << L"FriendlyName -> " << var.bstrVal << L"\r\n";
			VariantClear(&var);
		}
		// Device description
		hr = device_properties.object->Read(L"Description", &var, 0);
		if (SUCCEEDED(hr))
		{
			std::wcout << L"Description -> " << var.bstrVal << L"\r\n";
			VariantClear(&var);
		}
		// Device path (video devices only)
		hr = device_properties.object->Read(L"DevicePath", &var, 0);
		if (SUCCEEDED(hr))
		{
			std::wcout << L"DevicePath -> " << var.bstrVal << L"\r\n";
			VariantClear(&var);
		}
		// Device wave id (audio devices only)
		hr = device_properties.object->Read(L"WaveInID", &var, 0);
		if (SUCCEEDED(hr))
		{
			std::wcout << L"WaveInID -> " << var.lVal << L"\r\n";
			VariantClear(&var);
		}

		std::cout << "\r\n";

		// Clean up
		device_handler.ReleaseInterface();
	}
}

HRESULT IDirectShowConstructor::SelectElement(DSInterface<IEnumMoniker> *element_list, uint32_t *element_index, DSInterface<IBaseFilter> *element_filter)
{
	HRESULT hr = S_OK;
	DSInterface<IMoniker> element_handler;

	// Select device handler by index
	for (uint32_t i = 0; i < *(element_index) + 1; i++)
	{
		hr = element_list->object->Next(1, &element_handler.object, NULL);
		if (FAILED(hr)) break;

		if (i == *element_index)
		{
			// Select device
			hr = element_handler.object->BindToObject(NULL, NULL, IID_PPV_ARGS(&element_filter->object));
		}

		// Clean up
		element_handler.ReleaseInterface();
	}

	return hr;
}
