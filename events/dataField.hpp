#pragma once

#include <entities/entity.hpp>
#include <events/events/dataChangeEvent.hpp>
#include <events/iEventManager.hpp>
#include <serviceProvider.hpp>

template<class FieldType>
class DataField
{
public:
	DataField() = delete;
	DataField(const DataField<FieldType>& copy)
		: _field_name(copy.field_name), _field(copy._field), _entity_id(copy._entity_id)
	{}

	DataField(const char* field_name, EntityId entity_id)
		: _field_name(std::move(field_name)), _entity_id(std::move(entity_id))
	{}
	DataField(const char* field_name, FieldType field, EntityId entity_id = InvalidEntityId)
		: _field_name(std::move(field_name)),
		  _field(std::move(field)),
		  _entity_id(std::move(entity_id))
	{}

	virtual ~DataField() = default;

	inline DataField& operator=(const FieldType& b)
	{
		if (b != this->_field)
		{
			this->_field = b;
			this->triggerDataChange();
		}
		return *this;
	}
	[[nodiscard]] inline bool operator==(const FieldType& b) const { return this->_field == b; }
	[[nodiscard]] inline bool operator!=(const FieldType& b) const { return this->_field != b; }

	[[nodiscard]] inline operator FieldType() const { return this->_field; }

	void triggerDataChange()
	{
		auto e = std::make_shared<DataChangeEvent<FieldType>>(this->_field_name, this->_field,
			this->_entity_id);
		ServiceProvider::getEventManager()->triggerEvent(e);
	}

private:
	FieldType _field;
	const char* _field_name;
	EntityId _entity_id {InvalidEntityId};
};

// class DataManager
// {
// public:
// 	virtual ~DataManager() = default;

// 	void registerDataField<FieldType>(const char* field_name, const char* field,
// 		EntityId entity_id = InvalidEntityId)
// 	{}
// };
