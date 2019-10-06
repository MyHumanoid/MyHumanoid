#include "animorph/FileReader.h"

#include <locale.h>
#include "Logger.h"
#include "Vfs.h"

namespace Animorph
{

FileReader::FileReader()
	: locale(NULL)
{
}

FileReader::~FileReader()
{
	close();
}

bool FileReader::get(std::istringstream::char_type & c)
{
	return (bool) m_stream.get(c);
}

bool FileReader::getline(std::string & buffer)
{
	return (bool) std::getline(m_stream, buffer);
}

bool FileReader::open(const std::string & filename)
{
	
	std::string buffer;
	if(!vfs::loadString(filename, buffer)) {
		log_error("Couldn't open file: {}", filename);
		return false;
	}
	
	// Because float-values should be 0.1 instead of 0,1
	// in non-english if read from file. Later reset the locale
	locale = setlocale(LC_NUMERIC, NULL);
	setlocale(LC_NUMERIC, "C");
	
	m_stream = std::istringstream(buffer);
	
	return true;
}

void FileReader::close()
{
	// reset locale after file was written
	setlocale(LC_NUMERIC, locale);

	//m_stream.close();
}

}

