#pragma once

#include <memory>

#include <utilities/time/utils.hpp>

#include "iRawInputDevice.hpp"

class RawInputKeyboardDevice: public IRawInputDevice,
							  public std::enable_shared_from_this<RawInputKeyboardDevice>
{
public:
	explicit RawInputKeyboardDevice(HWND hwnd);
	virtual ~RawInputKeyboardDevice() = default;

	bool initialize() override;
	void deinitialize() override;

	void registerInputs(std::list<InputType> input_types) override;
	std::map<InputType, Input>& readState() override;
	void updateState() override;

	void onRawInputEvent(RAWINPUT* input);

private:
	HWND _hwnd {nullptr};
	Milliseconds _last_read_time {0};
	std::map<InputType, Input> _input_state;
};
