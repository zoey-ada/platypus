#pragma once

#include <filesystem>
#include <string>
namespace fs = std::filesystem;

class IFileSystem
{
public:
	virtual bool directoryExists(const std::string& path) const = 0;
	virtual bool directoryExists(const fs::path& path) const = 0;
	virtual bool createDirectory(const std::string& path) const = 0;
	virtual bool createDirectory(const fs::path& path) const = 0;

	virtual bool fileExists(const std::string& path) const = 0;
	virtual bool createFile(const std::string& path) const = 0;
};
