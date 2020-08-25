#pragma once

#include <string>

class IFileSystem
{
public:
	virtual bool directoryExists(const std::string& path) const = 0;
	virtual bool createDirectory(const std::string& path) const = 0;

	virtual bool fileExists(const std::string& path) const = 0;
	virtual bool createFile(const std::string& path) const = 0;
};
