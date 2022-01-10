#pragma once

#include "iResourceStore.hpp"

#include <fstream>
#include <map>

#include <libraries/libzip_include.hpp>

using ZipContentsMap = std::map<std::string, int>;

class ZipResourceStore: public IResourceStore
{
public:
	explicit ZipResourceStore(std::string path) : _path(std::move(path)) {}

	virtual ~ZipResourceStore();

	[[nodiscard]] bool open() override;
	[[nodiscard]] int64_t getResourceSize(const std::string& name) const override;
	[[nodiscard]] uint8_t* getResource(const std::string& name) const override;
	[[nodiscard]] bool getResource(const std::string& name, uint8_t* buffer) const override;
	[[nodiscard]] int64_t getResourceCount() const override;
	[[nodiscard]] int64_t getResourceIndex(const std::string& name) const override;
	[[nodiscard]] std::string getResourceName(const int64_t index) const override;
	[[nodiscard]] std::string getStoreIdentifier() const override;

private:
	zip_t* _zip_archive {nullptr};
	std::string _path {};
};
