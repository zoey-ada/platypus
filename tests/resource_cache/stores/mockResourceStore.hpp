#pragma once

#include <resource_cache/stores/iResourceStore.hpp>

using namespace platypus;

class MockResourceStore: public IResourceStore
{
public:
	virtual ~MockResourceStore() = default;

	std::string identifier() const override { return this->_identifier; }

	bool open() override { return this->_can_open; }

	uint64_t getResourceSize(const std::string& name) const override
	{
		return this->_resource_size;
	}

	uint8_t* getResource(const std::string& name) override { return this->_resource_data; }

	bool getResource(const std::string& name, uint8_t* buffer) override
	{
		if (this->_can_get_resource)
			buffer = this->_resource_data;

		return this->_can_get_resource;
	}

	uint64_t getResourceCount() const override { return this->_resource_count; }

	int64_t getResourceIndex(const std::string& name) const override
	{
		return this->_resource_index;
	}

	std::string getResourceName(const int64_t index) const override { return this->_resource_name; }

	std::string _identifier {"test"};
	bool _can_open {true};
	uint64_t _resource_size {1};
	uint8_t* _resource_data {nullptr};
	bool _can_get_resource {true};
	uint64_t _resource_count {1};
	int64_t _resource_index {0};
	std::string _resource_name {"test_resource.txt"};
};
