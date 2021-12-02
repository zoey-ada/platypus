#pragma once

#include "IFileSystem.hpp"

class StandardFileSystem : public IFileSystem
{
public:
	bool directoryExists(const std::string& path) const override;
	bool directoryExists(const fs::path& path) const override;
	bool createDirectory(const std::string& path) const override;
	bool createDirectory(const fs::path& path) const override;

	bool fileExists(const std::string& path) const override;
	bool createFile(const std::string& path) const override;
};
