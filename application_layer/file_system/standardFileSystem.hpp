#pragma once

#include "IFileSystem.hpp"

class StandardFileSystem : public IFileSystem
{
public:
	[[nodiscard]] bool directoryExists(const std::string& path) const override;
	[[nodiscard]] bool directoryExists(const fs::path& path) const override;
	[[nodiscard]] bool createDirectory(const std::string& path) const override;
	[[nodiscard]] bool createDirectory(const fs::path& path) const override;

	[[nodiscard]] bool fileExists(const std::string& path) const override;
	[[nodiscard]] bool createFile(const std::string& path) const override;
};
