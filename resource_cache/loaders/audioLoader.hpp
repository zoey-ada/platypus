#pragma once

#include <memory>

#include "iResourceLoader.hpp"

class ILoggingSystem;
class IResourceCache;

namespace platypus
{

class AudioLoader: public IResourceLoader
{
public:
	explicit AudioLoader(std::shared_ptr<IResourceCache> cache,
		std::shared_ptr<ILoggingSystem> logging);
	virtual ~AudioLoader() = default;

	[[nodiscard]] inline ResourceType getType() override { return ResourceType::Audio; }
	[[nodiscard]] inline std::string getPattern() override { return std::string(".*\\.(wav)"); }

	[[nodiscard]] std::shared_ptr<Resource> load(const char* resource_id, const char* store_id,
		std::byte* resource_data, const uint64_t data_size) override;

protected:
	uint8_t* allocate(unsigned int size) override;

private:
	std::shared_ptr<IResourceCache> _cache;
	std::shared_ptr<ILoggingSystem> _logging;
};

};
