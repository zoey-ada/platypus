#pragma once



class IDataManager
{
public:
	virtual ~IDataManager() = default;

	void registerDataField(const char* field_name, EntityId = InvalidEntityId);
};
