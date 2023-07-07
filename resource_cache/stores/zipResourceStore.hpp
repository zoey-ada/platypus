#pragma once

#include <fstream>
#include <map>

#include <libraries/libzip_include.hpp>

#include "iResourceStore.hpp"

using ZipContentsMap = std::map<std::string, int>;

namespace platypus
{

class ZipResourceStore: public IResourceStore
{
public:
	explicit ZipResourceStore(std::string identifier, std::string path)
		: _identifier(std::move(identifier)), _path(std::move(path))
	{}
	explicit ZipResourceStore(std::string path)
		: _identifier(std::move(path)), _path(this->_identifier)
	{}

	virtual ~ZipResourceStore();

	[[nodiscard]] std::string identifier() const override { return this->_identifier; };
	[[nodiscard]] bool open() override;
	[[nodiscard]] uint64_t getResourceSize(const std::string& name) const override;
	[[nodiscard]] uint8_t* getResource(const std::string& name) override;
	[[nodiscard]] bool getResource(const std::string& name, uint8_t* buffer) override;
	[[nodiscard]] uint64_t getResourceCount() const override;
	[[nodiscard]] int64_t getResourceIndex(const std::string& name) const override;
	[[nodiscard]] std::string getResourceName(const int64_t index) const override;

private:
	std::string _identifier {};
	std::string _path {};
	zip_t* _zip_archive {nullptr};
};

};
