#pragma once

#include <memory>

#include <platypus_proto/settings.hpp>

#include "iResourceStore.hpp"

class ResourceStoreFactory
{
public:
	static std::shared_ptr<IResourceStore> createResourceStore(
		const platypus::ResourceStoreSettings& settings);
};
