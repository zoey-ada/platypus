#pragma once

#include <memory>

#include <entities/entity.hpp>

#include "iEvent.hpp"

template<class FieldType>
class DataChangeEvent: public IEvent
{
public:
	explicit DataChangeEvent(const char* field_name, FieldType new_value,
		EntityId entity_id = InvalidEntityId)
		: _field_name(field_name), _new_value(new_value), _entity_id(entity_id)
	{
		this->_event_type = this->_field_name;
		this->_event_type.append("-data-change-event");
	}
	virtual ~DataChangeEvent() = default;

	const char* getType() override { return this->_event_type.c_str(); }

	std::string _event_type;
	const char* _field_name;
	FieldType _new_value;
	EntityId _entity_id {InvalidEntityId};
};
