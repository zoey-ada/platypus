#include "fileLogger.h"

#include "../../application_layer/file_system/standardFileSystem.h"
#include "../time.h"

#include <fstream>

FileLogger::FileLogger(std::string_view rootDir, const bool useSingleFile)
	: _mutex(), _rootDir(rootDir), _files(), _useSingleFile(useSingleFile)
{ }

FileLogger::~FileLogger()
{
	std::lock_guard(this->_mutex);

	for (auto& f : this->_files)
	{
		std::lock_guard(f.second.mutex);
		f.second.file.close();
	}

	this->_files.clear();
}

void FileLogger::log(std::string_view message)
{
	this->log(message, "");
}

void FileLogger::log(std::string_view message, std::string_view channel)
{
	this->_mutex.lock();

	std::string entry(channel);
	if (this->_useSingleFile || entry.empty())
		entry.assign("main");

	auto& cFile = this->_files[entry];

	cFile.mutex.lock();
	this->_mutex.unlock();

	if (!cFile.file.is_open())
	{
		StandardFileSystem sfs;
		if (!sfs.directoryExists(this->_rootDir))
			sfs.createDirectory(this->_rootDir);

		cFile.file.open(this->_rootDir / (entry + ".log"), std::ios_base::app);
	}

	cFile.file << "[" << getCurrentTimestamp() << "] ";

	if (this->_useSingleFile && !channel.empty())
		cFile.file << "<" << channel << "> ";

	cFile.file << message << std::endl;

	cFile.mutex.unlock();
}