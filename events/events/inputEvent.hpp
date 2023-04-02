#pragma once

#include <memory>
#include <optional>

#include <entities/entity.hpp>

#include "iEvent.hpp"

enum class InputActionType
{
	Start,
	Update,
	End,
	Invalid,
};

class InputEvent: public IEvent
{
public:
	InputEvent(std::string command, InputActionType action, EntityId entity_id = InvalidEntityId)
		: _command_name(std::move(command)),
		  _action(std::move(action)),
		  _entity_id(std::move(entity_id))
	{}

	InputEvent(std::string command, InputActionType action, float value,
		EntityId entity_id = InvalidEntityId)
		: _command_name(std::move(command)),
		  _action(std::move(action)),
		  _value(std::move(value)),
		  _entity_id(std::move(entity_id))
	{}

	virtual ~InputEvent() = default;

	const char* getType() override { return "input-event"; }

	std::string _command_name;
	InputActionType _action {InputActionType::Invalid};
	std::optional<float> _value;
	EntityId _entity_id {InvalidEntityId};
};
