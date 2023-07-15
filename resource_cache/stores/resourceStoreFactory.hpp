#pragma once

#include <memory>

#include <platypus_proto/settings.hpp>

#include "iResourceStore.hpp"

namespace platypus
{

class ResourceStoreFactory
{
public:
	static std::shared_ptr<IResourceStore> createResourceStore(
		const ResourceStoreSettings& settings);
};

};
