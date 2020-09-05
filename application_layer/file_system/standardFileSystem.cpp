#include "standardFileSystem.h"

#include <filesystem>
namespace fs = std::filesystem;

bool StandardFileSystem::directoryExists(const std::string& path) const
{
	return this->directoryExists(fs::path(path));
}

bool StandardFileSystem::createDirectory(const std::string& path) const
{
	return this->createDirectory(fs::path(path));
}

bool StandardFileSystem::directoryExists(const fs::path& path) const
{
	return fs::exists(path) && fs::is_directory(path);
}

bool StandardFileSystem::createDirectory(const fs::path& path) const
{
	return fs::create_directories(path);
}

bool StandardFileSystem::fileExists(const std::string& path) const
{
	fs::path p(path);
	return fs::exists(p) && fs::is_regular_file(p);
}

bool StandardFileSystem::createFile(const std::string& /*path*/) const
{
	_ASSERT("NOT IMPLIMENTED");
	return false;
}
