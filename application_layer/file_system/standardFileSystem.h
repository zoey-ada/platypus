#pragma once

#include "IFileSystem.h"

class StandardFileSystem : public IFileSystem
{
public:
	bool directoryExists(const std::string& path) const override;
	bool createDirectory(const std::string& path) const override;

	bool fileExists(const std::string& path) const override;
	bool createFile(const std::string& path) const override;
};
