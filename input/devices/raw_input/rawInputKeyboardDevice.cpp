#include "rawInputKeyboardDevice.hpp"

#include <array>

#include "Windows.h"
#include "windowsHidMappings.hpp"
#include <application_layer/window/windowsWindow.hpp>
#include <hidusage.h>

#include <serviceProvider.hpp>
#include <utilities/logging/logger.hpp>

#include "hidsdi.h"

RawInputKeyboardDevice::RawInputKeyboardDevice(HWND hwnd): _hwnd(hwnd)
{}

bool RawInputKeyboardDevice::initialize()
{
	// std::vector<RAWINPUTDEVICELIST> input_devices;
	// UINT num_input_devices = 0;

	// GetRawInputDeviceList(nullptr, &num_input_devices, sizeof(RAWINPUTDEVICELIST));
	// input_devices.resize(num_input_devices);

	// GetRawInputDeviceList(input_devices.data(), &num_input_devices, sizeof(RAWINPUTDEVICELIST));

	// std::vector<RID_DEVICE_INFO> devices;
	// std::vector<std::string> device_names;
	// std::vector<PHIDP_PREPARSED_DATA*> parsed_device_data;
	// for (auto& input_device : input_devices)
	// {
	// 	RID_DEVICE_INFO device {};
	// 	UINT size = sizeof(RID_DEVICE_INFO);
	// 	GetRawInputDeviceInfo(input_device.hDevice, RIDI_DEVICEINFO, &device, &size);
	// 	devices.push_back(device);

	// 	std::string name(150, ' ');
	// 	size = 150;
	// 	GetRawInputDeviceInfo(input_device.hDevice, RIDI_DEVICENAME, name.data(), &size);
	// 	device_names.push_back(name);

	// 	GetRawInputDeviceInfo(input_device.hDevice, RIDI_PREPARSEDDATA, nullptr, &size);
	// 	PHIDP_PREPARSED_DATA* parsed_data = reinterpret_cast<PHIDP_PREPARSED_DATA*>(new byte[size]);
	// 	size = sizeof(PHIDP_PREPARSED_DATA);
	// 	GetRawInputDeviceInfo(input_device.hDevice, RIDI_PREPARSEDDATA, parsed_data, &size);
	// 	parsed_device_data.push_back(parsed_data);
	// }

	RAWINPUTDEVICE keyboard {};
	keyboard.usUsagePage = HID_USAGE_PAGE_GENERIC;
	keyboard.usUsage = HID_USAGE_GENERIC_KEYBOARD;
	keyboard.dwFlags = RIDEV_NOLEGACY;
	keyboard.hwndTarget = this->_hwnd;

	if (TRUE != RegisterRawInputDevices(&keyboard, 1, sizeof(keyboard)))
	{
		// error
		return false;
	}

	auto window = dynamic_pointer_cast<WindowsWindow>(ServiceProvider::getWindow());
	if (window == nullptr)
	{
		// error
		return false;
	}

	window->addRawInputDevice(this->shared_from_this());
	return true;
}

void RawInputKeyboardDevice::deinitialize()
{
	RAWINPUTDEVICE keyboard {};
	keyboard.usUsagePage = HID_USAGE_PAGE_GENERIC;
	keyboard.usUsage = HID_USAGE_GENERIC_KEYBOARD;
	keyboard.dwFlags = RIDEV_REMOVE;
	keyboard.hwndTarget = this->_hwnd;

	if (TRUE != RegisterRawInputDevices(&keyboard, 1, sizeof(keyboard)))
	{
		// error
	}
}

void RawInputKeyboardDevice::registerInputs(std::list<InputType> input_types)
{
	this->_input_state.clear();

	for (const auto input_type : input_types)
	{
		Input input {input_type, hid_mappings.at(input_type).is_analog};
		this->_input_state[input_type] = input;
	}
}

bool isPressed(byte key_flags)
{
	return !(key_flags & RI_KEY_BREAK);
}

std::map<InputType, Input>& RawInputKeyboardDevice::readState()
{
	// std::array<BYTE, 256> keyboard_state;
	// BOOL hr = GetKeyboardState(keyboard_state.data());

	// for (const auto entry : this->_input_state)
	// {
	// 	auto& key = entry.first;
	// 	auto& virtual_key = hid_mappings.at(key).hid_vkey;
	// 	this->_input_state[key].previous_state = this->_input_state[key].current_state;
	// 	this->_input_state[key].current_state.pressed = isPressed(keyboard_state[virtual_key]);
	// }

	return this->_input_state;
}

void RawInputKeyboardDevice::updateState()
{
	for (auto& entry : this->_input_state)
	{
		entry.second.previous_state = entry.second.current_state;
	}
}

void RawInputKeyboardDevice::onRawInputEvent(RAWINPUT* input)
{
	if (input == nullptr || input->header.dwType != RIM_TYPEKEYBOARD)
	{
		return;
	}

	const byte virtual_key = static_cast<byte>(input->data.keyboard.VKey);

	if (input_type_mappings.find(virtual_key) == input_type_mappings.end())
	{
		logWarning("encountered unknown virtual key:" + std::to_string(virtual_key));
		return;
	}
	const InputType input_type = input_type_mappings.at(virtual_key);

	if (this->_input_state.find(input_type) != this->_input_state.end())
	{
		this->_input_state[input_type].current_state.pressed =
			isPressed(static_cast<byte>(input->data.keyboard.Flags));
	}

	// UINT size = 0;
	// GetRawInputData(*raw_input, RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER));
	// RAWINPUT* input = (RAWINPUT*)malloc(size);
	// if (size == GetRawInputData(*raw_input, RID_INPUT, input, &size, sizeof(RAWINPUTHEADER)))
	// {
	// 	GetRawInputDeviceInfo(input->header.hDevice, RIDI_PREPARSEDDATA, nullptr, &size);
	// 	_HIDP_PREPARSED_DATA* data = (_HIDP_PREPARSED_DATA*)malloc(size);
	// 	if (size == GetRawInputDeviceInfo(input->header.hDevice, RIDI_PREPARSEDDATA, data, &size))
	// 	{
	// 		HIDP_CAPS caps;
	// 		HidP_GetCaps(data, &caps);

	// 		HIDP_VALUE_CAPS* value_caps =
	// 			(HIDP_VALUE_CAPS*)malloc(caps.NumberInputValueCaps * sizeof(HIDP_VALUE_CAPS));
	// 		HidP_GetValueCaps(HidP_Input, value_caps, &caps.NumberInputValueCaps, data);

	// 		for (USHORT i = 0; i < caps.NumberInputValueCaps; ++i)
	// 		{
	// 			ULONG value;
	// 			HidP_GetUsageValue(HidP_Input, value_caps[i].UsagePage, 0,
	// 				value_caps[i].Range.UsageMin, &value, data, (PCHAR)input->data.hid.bRawData,
	// 				input->data.hid.dwSizeHid);
	// 			printf("%d:%5d ", i, value);
	// 		}
	// 		free(value_caps);

	// 		HIDP_BUTTON_CAPS* button_caps =
	// 			(HIDP_BUTTON_CAPS*)malloc(caps.NumberInputButtonCaps * sizeof(HIDP_BUTTON_CAPS));
	// 		HidP_GetButtonCaps(HidP_Input, button_caps, &caps.NumberInputButtonCaps, data);

	// 		for (USHORT i = 0; i < caps.NumberInputButtonCaps; ++i)
	// 		{
	// 			ULONG usage_count = button_caps->Range.UsageMax - button_caps->Range.UsageMin + 1;
	// 			USAGE* usages = (USAGE*)malloc(sizeof(USAGE) * usage_count);
	// 			HidP_GetUsages(HidP_Input, button_caps[i].UsagePage, 0, usages, &usage_count, data,
	// 				(PCHAR)input->data.hid.bRawData, input->data.hid.dwSizeHid);
	// 			for (ULONG usage_index = 0; usage_index < usage_count; ++usage_index)
	// 			{
	// 				printf("%d ", usages[usage_index]);
	// 			}
	// 			free(usages);
	// 		}
	// 		free(button_caps);
	// 	}

	// 	free(data);
	// }

	// free(input);

	return;
}
