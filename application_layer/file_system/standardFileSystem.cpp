#include "standardFileSystem.h"

#include <filesystem>
namespace fs = std::filesystem;

bool StandardFileSystem::directoryExists(const std::string& path) const
{
	fs::path p(path);
	return fs::exists(p) && fs::is_directory(p);
}

bool StandardFileSystem::createDirectory(const std::string& path) const
{
	return fs::create_directories(fs::path(path));
}

bool StandardFileSystem::fileExists(const std::string& path) const
{
	fs::path p(path);
	return fs::exists(p) && fs::is_regular_file(p);
}

bool StandardFileSystem::createFile(const std::string& path) const
{
	_ASSERT("NOT IMPLIMENTED");
}
