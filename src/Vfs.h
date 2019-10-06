#pragma once

#include <vector>
#include <string>

// TODO remove
#include "util.h"


namespace vfs
{

bool init();

void deinit();

bool exists(const std::string & path);

enum class FileType
{
	Error,
	Regular,
	Directory,
	Symlink,
	Other
};

FileType getType(const std::string & path);

std::vector<std::string> listFlat(const std::string & path);

std::vector<std::string> list(const std::string & path, bool listDirs = false);

bool loadString(const std::string & fileName, std::string & value);

bool writeString(const char * fileName, const std::string & value);

void copyToFilesystem(const std::string & inPath, const std::string outPath);

} // namespace vfs
