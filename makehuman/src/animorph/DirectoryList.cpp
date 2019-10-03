#include "animorph/DirectoryList.h"
//#include "animorph/util.h"

#include "Vfs.h"

using namespace Animorph;

DirectoryList::DirectoryList()
        : file_list()
        , file_filter()
        , path()
        , recursive_level(-1)
        , recursive_counter(0)
        , file_type(REGULAR_FILE)
{
}

void DirectoryList::setFileFilter(const std::string & file_filter)
{
	this->file_filter = file_filter;
}

bool DirectoryList::hasFileFilterEnding(const std::string & file) const
{
	if(file_filter.length() > file.length())
		return false;

	return (file.find(file_filter, file.length() - file_filter.length()) != std::string::npos);
}

void DirectoryList::setRecursive(int recursive_level)
{
	this->recursive_level = recursive_level;
}

void DirectoryList::setRootPath(const std::string & path)
{
	this->path = path;
}

void DirectoryList::readDirRecursive(const std::string & dir_str)
{
	auto files = vfs::listFlat(dir_str);
	for(const auto & file: files) {
		std::string dname = file;
		std::string dir_file = dir_str + "/" + dname;

		if(dname.at(0) == '.') {
			continue;
		}
		
		auto type = vfs::getType(dir_file);
		if(type == vfs::FileType::Directory) {
			if(file_type == DIRECTORY) {
				file_list.push_back(dir_file);
			}
			
			if((recursive_counter < recursive_level) ||
			   (recursive_level == NO_LIMIT)) {
				recursive_counter++;
				readDirRecursive(dir_file);
				recursive_counter--;
			}
		} else if(type == vfs::FileType::Regular) {
			if(file_type == REGULAR_FILE) {
				if(hasFileFilterEnding(dir_file)) {
					file_list.push_back(dir_file);
				}
			}
		}
	}
}

const StringList & DirectoryList::getDirectoryList()
{
	readDirRecursive(path);
	recursive_counter = 0;

	return file_list;
}

void DirectoryList::setFileType(FileType file_type)
{
	this->file_type = file_type;
}
