#pragma once

#include <map>
#include <vector>

#include "command.hpp"
#include "devices/iInputDevice.hpp"
#include "inputType.hpp"

class IEventManager;

using InputMappings = std::map<InputType, Command>;

class InputManager
{
public:
	InputManager();
	virtual ~InputManager() = default;

	bool initialize();
	void deinitialize();

	void readInput(const Milliseconds now);

	void loadConfiguration(const std::string& path);
	// void saveConfiguration(const std::string& path);

	void loadCommands(const std::string& path);

private:
	InputMappings _configured_inputs;
	std::vector<std::shared_ptr<IInputDevice>> _input_devices;
	std::shared_ptr<IEventManager> _event_manager;
	std::vector<Command> _commands;
	std::list<InputType> _inputs_to_register;

	Milliseconds _last_read {0};
	Milliseconds _wait_time {0};

	void readInputFromDevice(const std::shared_ptr<IInputDevice>& device);
	void processBinaryInput(const Input& input);
	void processAnalogInput(const Input& input);
};
