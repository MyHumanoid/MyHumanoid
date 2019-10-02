#pragma once

#include <vector>
#include <string>

namespace vfs
{

bool init();

void deinit();

std::vector<std::string> list(const std::string & path, bool listDirs = false);

bool loadString(const std::string & fileName, std::string & value);

bool writeString(const char * fileName, const std::string & value);

} // namespace vfs
