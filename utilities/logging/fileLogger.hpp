#pragma once

#include "ILogger.hpp"

#include <filesystem>
#include <fstream>
#include <map>
#include <mutex>
namespace fs = std::filesystem;

struct ChannelFile
{
	std::ofstream file;
	std::mutex mutex;
};

class FileLogger : public ILogger
{
public:
	explicit FileLogger(std::string_view rootDir, const bool useSingleFile = false);
	virtual ~FileLogger();

	void log(std::string_view message) override;
	void log(std::string_view message, std::string_view channel) override;

	[[nodiscard]] bool supportsColor() const override { return false; }

private:
	std::mutex _mutex;
	fs::path _rootDir;
	std::map<std::string, ChannelFile> _files;
	bool _useSingleFile;
};
