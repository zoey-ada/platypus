#pragma once

#include <string>

#include <entities/entity.hpp>

class Command
{
public:
	Command() = default;
	explicit Command(std::string name, std::string command_type, EntityId entity = InvalidEntityId)
		: _name(name), _command_type(command_type), _entity(entity)
	{}
	virtual ~Command() = default;

	std::string name() const { return this->_name.empty() ? this->_command_type : this->_name; }
	std::string command_type() const { return this->_command_type; }
	EntityId entity() const { return this->_entity; }

private:
	std::string _name;
	std::string _command_type;
	EntityId _entity {InvalidEntityId};
};
