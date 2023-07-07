#pragma once

#include "../resources/stringResourceData.hpp"
#include "iResourceLoader.hpp"

class IResourceCache;

namespace platypus
{

template<class T>
class StringLoader: public IResourceLoader
{
public:
	StringLoader(std::shared_ptr<IResourceCache> cache, std::shared_ptr<ILoggingSystem> logging,
		ResourceType resource_type)
		: _cache(std::move(cache)), _logging(std::move(logging)), _resource_type(resource_type)
	{}
	virtual ~StringLoader() = default;

	[[nodiscard]] inline ResourceType getType() override { return _resource_type; }
	[[nodiscard]] inline std::string getPattern() override { return std::string(".*\\.(json)"); }

	[[nodiscard]] std::shared_ptr<Resource> load(const char* resource_id, const char* store_id,
		std::byte* resource_data, const uint64_t data_size) override
	{
		if (_cache == nullptr || resource_data == nullptr || strlen(resource_id) == 0)
		{
			this->_logging->warning("cache",
				"received invalid data for " + std::string(resource_id));
			return nullptr;
		}

		std::string content(reinterpret_cast<char*>(resource_data), data_size);

		StringData string_data {};
		string_data.resource_id = resource_id;
		string_data.store_id = store_id;
		string_data.size = sizeof(std::string) + data_size;
		string_data.string_data = std::move(content);
		return std::make_shared<T>(&string_data);
	}

protected:
	uint8_t* allocate(unsigned int size) override { return this->_cache->allocate(size); }

private:
	std::shared_ptr<IResourceCache> _cache;
	std::shared_ptr<ILoggingSystem> _logging;
	ResourceType _resource_type {ResourceType::Unknown};
};

};
