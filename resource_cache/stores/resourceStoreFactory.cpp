#include "resourceStoreFactory.hpp"

#include "zipResourceStore.hpp"

std::shared_ptr<IResourceStore> ResourceStoreFactory::createResourceStore(
	const platypus::ResourceStoreSettings& settings)
{
	if (settings.type() == "zip_file")
	{
		return std::make_shared<ZipResourceStore>(settings.identifier(),
			settings.zip_file_store_settings().filepath());
	}

	return nullptr;
}
