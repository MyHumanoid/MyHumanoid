#include "animorph/FileReader.h"

#include <locale.h>

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

bool FileReader::get(std::ifstream::char_type & c)
{
	return (bool) m_stream.get(c);
}

bool FileReader::getline(std::ifstream::char_type *c, size_t s)
{
	return (bool)m_stream.getline(c, s);
}

bool FileReader::open(const std::string & filename)
{
	//std::ifstream & if_stream = (*this);
	
	// Because float-values should be 0.1 instead of 0,1
	// in non-english if read from file. Later reset the locale
	locale = setlocale(LC_NUMERIC, NULL);
	setlocale(LC_NUMERIC, "C");

	m_stream.open(filename.c_str(), std::ios::in);
	if(!m_stream) {
		std::cerr << "Couldn't open file:" << filename << std::endl;
		return false;
	}

	return true;
}

void FileReader::close()
{
	// reset locale after file was written
	setlocale(LC_NUMERIC, locale);

	m_stream.close();
}

}

