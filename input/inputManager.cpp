#include "inputManager.hpp"

#include <algorithm>

#include <platypus_proto/input.hpp>
#include <platypus_proto/util.hpp>

#include <events/events/inputEvent.hpp>
#include <events/iEventManager.hpp>
#include <platform/iPlatform.hpp>
#include <serviceProvider.hpp>
#include <utilities/logging/logger.hpp>

#include "devices/iInputDevice.hpp"

InputManager::InputManager()
{}

bool InputManager::initialize()
{
	logVerbose("Input Manager initializing", "input");

	this->loadCommands("./command_list.json");
	this->loadConfiguration("./input_config.json");

	this->_event_manager = ServiceProvider::getEventManager();

	bool success = true;

	this->_input_devices = ServiceProvider::getPlatform()->queryInputDevices();
	for (auto& device : this->_input_devices)
	{
		success &= device->initialize();
		device->registerInputs(this->_inputs_to_register);
	}

	return success;
}

void InputManager::deinitialize()
{
	logVerbose("Input Manager deinitializing", "input");

	for (auto& device : this->_input_devices)
	{
		device->deinitialize();
	}
}

void InputManager::readInput(const Milliseconds now)
{
	if (now - this->_last_read >= this->_wait_time)
	{
		logDebug("reading input", "input");
		for (auto& device : this->_input_devices)
		{
			this->readInputFromDevice(device);
		}

		this->_last_read = now;
	}
};

void InputManager::readInputFromDevice(const std::shared_ptr<IInputDevice>& device)
{
	auto inputs = device->readState();
	device->updateState();

	for (auto input : inputs)
	{
		if (input.second.is_analog)
		{
			this->processAnalogInput(input.second);
		}
		else
		{
			this->processBinaryInput(input.second);
		}
	}
}

void InputManager::processBinaryInput(const Input& input)
{
	if (input.current_state.pressed != input.previous_state.pressed)
	{
		auto iter = this->_configured_inputs.find(input.type);
		if (iter != this->_configured_inputs.end())
		{
			const auto command = iter->second;
			auto action_type =
				input.current_state.pressed ? InputActionType::Start : InputActionType::End;

			auto event =
				std::make_shared<InputEvent>(command.command_type(), action_type, command.entity());
			this->_event_manager->triggerEvent(event);
		}
	}
}

void InputManager::processAnalogInput(const Input& input)
{
	// todo: this needs to be a threshold, not just a straight compare
	if (input.current_state.analog_value != input.previous_state.analog_value)
	{
		auto iter = this->_configured_inputs.find(input.type);
		if (iter != this->_configured_inputs.end())
		{
			// todo: need to determine if this is the beginning, ending, or updating of the input

			const auto command = iter->second;
			auto event = std::make_shared<InputEvent>(command.command_type(),
				InputActionType::Invalid, input.current_state.analog_value, command.entity());
			this->_event_manager->triggerEvent(event);
		}
	}
}

void InputManager::loadConfiguration(const std::string& path)
{
	logVerbose("Loading input configuration from: " + path, "input");
	try
	{
		auto input_config = platypus::readProtoFile<platypus::InputConfiguration>(path);
		this->_wait_time = frametimeFromFrameRate(input_config.refresh_rate());

		for (const auto& mapping : input_config.button_mappings())
		{
			auto command = std::find_if(this->_commands.begin(), this->_commands.end(),
				[&mapping](Command c) { return c.command_type().compare(mapping.command()) == 0; });

			InputType input_type = (InputType)mapping.input();
			this->_configured_inputs[input_type] = (*command);
			this->_inputs_to_register.push_back(input_type);
		}
	}
	catch (const platypus::ProtoFileParsingError& e)
	{
		logWarning("Unable to load command list. Falling back to defaults.", "input");
		logVerbose("Error loading '" + e._filename + "' >> " + e.what(), "input");
		// todo: load defaults
		assert(false);
	}
}

void InputManager::loadCommands(const std::string& path)
{
	logVerbose("Loading command list from: " + path, "input");
	try
	{
		auto command_list = platypus::readProtoFile<platypus::CommandList>(path);
		for (const auto& command : command_list.commands())
		{
			this->_commands.push_back(Command(command.friendly_name(), command.name()));
		}
	}
	catch (const platypus::ProtoFileParsingError& e)
	{
		logWarning("Unable to load command list. Inputs disabled.", "input");
		logVerbose("Error loading '" + e._filename + "' >> " + e.what(), "input");
	}
}

// void InputManager::saveConfiguration(const std::string& path)
// {}
