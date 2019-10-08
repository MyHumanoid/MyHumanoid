#include "Vfs.h"

#include <array>
#include <cstddef>
#include <vector>
#include <stack>
#include <optional>
#include <iostream>
#include <fstream>

#include <physfs.h>

#include "Logger.h"

namespace vfs
{

bool init() {
	PHYSFS_permitSymbolicLinks(1);
	PHYSFS_mount("./data", "/data", 1);
	//PHYSFS_mount("./", "/", 1);
	
	//auto ok = PHYSFS_setWriteDir("./");
	return true;//ok != 0;
}

void deinit() {
	PHYSFS_deinit();
}

bool exists(const std::string & path) {
	return PHYSFS_exists(path.c_str());
}

FileType getType(const std::string & path)
{
	PHYSFS_Stat stat;
	if(PHYSFS_stat(path.c_str(), &stat)) {
		switch (stat.filetype) {
		case PHYSFS_FILETYPE_REGULAR:
			return FileType::Regular;
		case PHYSFS_FILETYPE_DIRECTORY:
			return FileType::Directory;
		case PHYSFS_FILETYPE_SYMLINK:
			return FileType::Symlink;
		case PHYSFS_FILETYPE_OTHER:
			return FileType::Other;
		}
	} else {
		return FileType::Error;
	}
}

std::vector<std::string> listFlat(const std::string & path)
{
	std::vector<std::string> paths;
	auto files = PHYSFS_enumerateFiles(path.c_str());
	for(char ** i = files; *i != NULL; i++) {
		paths.push_back(std::string(*i));
	}
	PHYSFS_freeList(files);
	return paths;
}


std::vector<std::string> list(const std::string &path, bool listDirs)
{
	std::vector<std::string> paths;
	auto files = PHYSFS_enumerateFiles(path.c_str());
	for(char ** i = files; *i != NULL; i++) {
		std::string s = std::string(path) + "/" + std::string(*i);
		PHYSFS_Stat stat;
		if(PHYSFS_stat(s.c_str(), &stat)) {
			if(!listDirs && stat.filetype == PHYSFS_FILETYPE_REGULAR) {
				paths.push_back(s);
			} else if(stat.filetype == PHYSFS_FILETYPE_DIRECTORY) {
				paths.push_back(s);
			}
		}
	}
	PHYSFS_freeList(files);
	return paths;
}

bool loadString(const std::string &fileName, std::string & value) {
	auto file = PHYSFS_openRead(fileName.c_str());
	if(file == nullptr) {
		auto errVal = PHYSFS_getLastErrorCode();
		auto errStr = PHYSFS_getErrorByCode(errVal);
		log_error("Failed to open file: {}, {}", fileName, errStr);
		return false;
	}
	int64_t fileSize = PHYSFS_fileLength(file);
	if(fileSize >= 0) {
		value.resize(fileSize, 0);
		PHYSFS_sint64 readData = PHYSFS_readBytes(file, value.data(), fileSize);
		if(fileSize == readData) {
			return true;
		}
	}
	if(!PHYSFS_close(file)) {
		log_error("Failed to close file: {}", fileName);
	}
	return true;
}

bool writeString(const char *fileName, const std::string &value) {
	auto file = PHYSFS_openWrite(fileName);
	if(file == nullptr) {
		auto errVal = PHYSFS_getLastErrorCode();
		auto errStr = PHYSFS_getErrorByCode(errVal);
		log_error("Failed to open file: {}, {}", fileName, errStr);
		return false;
	}
	
	PHYSFS_writeBytes(file, value.data(), value.size());
	
	if(!PHYSFS_close(file)) {
		log_error("Failed to close file: {}", fileName);
	}
	return true;
}

void copyToFilesystem(const std::string & inPath, const std::string outPath) {
	
	auto file = PHYSFS_openRead(inPath.c_str());
	if(!file) {
		log_error("File does not exist");
		return;
	}
	
	std::ofstream out(outPath, std::ios::out | std::ios::binary);
	if(!out) {
		log_error("Failed to open file: {}", outPath);
		return;
	}
	
	std::array<char, 1024 * 4> buffer;
	
	PHYSFS_sint64 readBytes = 0;
	while((readBytes = PHYSFS_readBytes(file, buffer.data(), buffer.size())) > 0) {
		out.write(buffer.data(), readBytes);
	}
	if(!PHYSFS_eof(file)) {
		log_error("Got a short read ");
	}
	out.close();
	PHYSFS_close(file);
}

} // namespace vfs

std::optional<std::string> fs::loadString(const std::string & filePath)
{
	std::ifstream in(filePath, std::ios::in | std::ios::binary);
	if(!in) {
		return std::nullopt;
	}
	
	std::string value;
	in.seekg(0, std::ios::end);
	value.resize(in.tellg());
	in.seekg(0, std::ios::beg);
	in.read(&value[0], value.size());
	in.close();
	return value;
}

bool fs::saveString(const std::string & filePath, const std::string & value)
{
	std::ofstream out(filePath, std::ios::out | std::ios::binary);
	if(!out) {
		return false;
	}
	
	out << value;
	return true;
}
