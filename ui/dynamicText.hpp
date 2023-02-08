#pragma once

#include "staticText.hpp"

class DynamicText: public StaticText
{
public:
	DynamicText(const char* data_field_name, const Vec2& position,
		EntityId entity_id = InvalidEntityId);
	virtual ~DynamicText() = default;

	void registerEventSinks() override;

	void onDataChanged(std::shared_ptr<IEvent> event);
	virtual std::string parseNewValue(std::shared_ptr<IEvent> event) = 0;

private:
	const char* _data_field_name {""};
};

class DynamicIntegerText: public DynamicText
{
public:
	DynamicIntegerText(const char* data_field_name, const Vec2& position,
		EntityId entity_id = InvalidEntityId);
	virtual ~DynamicIntegerText() = default;

	std::string parseNewValue(std::shared_ptr<IEvent> event) override;
};
