#pragma once

#include <filesystem>
#include <string>
namespace fs = std::filesystem;

class IFileSystem
{
public:
	[[nodiscard]] virtual bool directoryExists(const std::string& path) const = 0;
	[[nodiscard]] virtual bool directoryExists(const fs::path& path) const = 0;
	[[nodiscard]] virtual bool createDirectory(const std::string& path) const = 0;
	[[nodiscard]] virtual bool createDirectory(const fs::path& path) const = 0;

	[[nodiscard]] virtual bool fileExists(const std::string& path) const = 0;
	[[nodiscard]] virtual bool createFile(const std::string& path) const = 0;
};
