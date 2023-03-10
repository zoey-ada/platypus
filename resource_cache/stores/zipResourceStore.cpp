#include "zipResourceStore.hpp"

#include <utilities/logging/logger.hpp>

ZipResourceStore::~ZipResourceStore()
{
	if (this->_zip_archive != nullptr)
		zip_discard(this->_zip_archive);
}

bool ZipResourceStore::open()
{
	if (this->_zip_archive != nullptr)
		return true;

	int error = ZIP_ER_OK;
	int flags = ZIP_RDONLY;
	this->_zip_archive = zip_open(this->_path.c_str(), flags, &error);

	if (this->_zip_archive == nullptr || error != ZIP_ER_OK)
	{
		if (error == ZIP_ER_MEMORY)
			logError("unable to allocate memory for zip file: " + this->_path, "resource_cache");
		else if (error == ZIP_ER_INVAL || error == ZIP_ER_NOENT || error == ZIP_ER_NOZIP)
			logError("unable to find zip file: " + this->_path, "resource_cache");
		else if (error == ZIP_ER_OPEN || error == ZIP_ER_READ || error == ZIP_ER_SEEK)
			logError("unable to use zip file: " + this->_path, "resource_cache");

		return false;
	}

	return true;
}

uint64_t ZipResourceStore::getResourceSize(const std::string& name) const
{
	auto index = this->getResourceIndex(name);
	if (index == -1)
		return 0;

	auto* stats = new (std::nothrow) zip_stat_t;
	if (stats == nullptr)
	{
		// error
		return 0;
	}

	zip_stat_index(this->_zip_archive, index, ZIP_FL_UNCHANGED, stats);
	if ((stats->valid & ZIP_STAT_SIZE) == 0)
	{
		// error
		return 0;
	}

	auto size = stats->size;
	delete stats;
	stats = nullptr;
	return size;
}

uint8_t* ZipResourceStore::getResource(const std::string& name)
{
	auto buffer = new (std::nothrow) uint8_t[this->getResourceSize(name)];
	if (!this->getResource(name, buffer))
	{
		delete[] buffer;
		buffer = nullptr;
	}

	return buffer;
}

bool ZipResourceStore::getResource(const std::string& name, uint8_t* buffer)
{
	auto index = this->getResourceIndex(name);
	if (index == -1)
	{
		// error
		return false;
	}

	auto size = this->getResourceSize(name);
	if (size == (uint64_t)-1)
	{
		// error
		return false;
	}

	auto* file = zip_fopen_index(this->_zip_archive, index, ZIP_FL_UNCHANGED);
	if (file == nullptr)
	{
		// error
		return false;
	}

	auto read_size = zip_fread(file, buffer, size);
	zip_fclose(file);

	if (read_size < 1 || static_cast<uint64_t>(read_size) != size)
	{
		// error
		return false;
	}

	return true;
}

uint64_t ZipResourceStore::getResourceCount() const
{
	if (this->_zip_archive == nullptr)
	{
		logWarning("tried to use store " + this->identifier() + " without opening it",
			"resource_cache");
		return 0;
	}

	return zip_get_num_entries(this->_zip_archive, ZIP_FL_UNCHANGED);
}

int64_t ZipResourceStore::getResourceIndex(const std::string& name) const
{
	if (this->_zip_archive == nullptr)
	{
		logWarning("tried to use store " + this->identifier() + " without opening it",
			"resource_cache");
		return -1;
	}

	int flags = 0;
	auto index = zip_name_locate(this->_zip_archive, name.c_str(), flags);

	if (index == -1)
	{
		int error = zip_error_code_zip(zip_get_error(this->_zip_archive));
		if (error == ZIP_ER_NOENT)
			logError(this->identifier() + " does not contain file " + name, "resource_cache");
		else if (error == ZIP_ER_INVAL || error == ZIP_ER_MEMORY)
			logError("failed to get " + name + " from " + this->identifier(), "resource_cache");

		return -1;
	}

	return index;
}

std::string ZipResourceStore::getResourceName(const int64_t index) const
{
	auto* stats = new (std::nothrow) zip_stat_t;
	if (stats == nullptr)
	{
		// error
		return std::string();
	}

	zip_stat_index(this->_zip_archive, index, ZIP_FL_UNCHANGED, stats);
	if ((stats->valid & ZIP_STAT_NAME) == 0)
	{
		// error
		return std::string();
	}

	std::string name(stats->name);
	delete stats;
	stats = nullptr;
	return name;
}
