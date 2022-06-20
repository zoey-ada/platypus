#pragma once

#include <filesystem>
#include <fstream>
#include <map>
#include <mutex>

#include <platypus_proto/settings.hpp>

#include "iLogger.hpp"
namespace fs = std::filesystem;

class IClock;

struct ChannelFile
{
	std::ofstream file;
	std::mutex mutex;
};

class FileLogger: public ILogger
{
public:
	explicit FileLogger(const platypus::FileLoggerSettings& settings, std::shared_ptr<IClock> clock)
		: FileLogger(clock, settings.root_dir(), settings.use_single_file())
	{}

	explicit FileLogger(std::shared_ptr<IClock> clock, std::string_view rootDir,
		const bool useSingleFile = false);

	virtual ~FileLogger();

	void log(std::string_view message) override;
	void log(std::string_view message, std::string_view channel) override;

	[[nodiscard]] bool supportsColor() const override { return false; }

private:
	std::mutex _mutex;
	fs::path _rootDir;
	std::map<std::string, ChannelFile> _files;
	bool _useSingleFile;
	std::shared_ptr<IClock> _clock;
};
