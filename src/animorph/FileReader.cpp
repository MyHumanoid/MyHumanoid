#include "animorph/FileReader.h"

#include <locale.h>

namespace Animorph
{

int FileReader::open(const std::string & filename)
{
	std::ifstream & if_stream = (*this);

	// Because float-values should be 0.1 instead of 0,1
	// in non-english if read from file. Later reset the locale
	locale = setlocale(LC_NUMERIC, NULL);
	setlocale(LC_NUMERIC, "C");

	if_stream.open(filename.c_str(), std::ios::in);
	if(!if_stream) {
		std::cerr << "Couldn't open file:" << filename << std::endl;
		return -1;
	}

	return 0;
}

void FileReader::close()
{
	std::ifstream & if_stream = (*this);

	// reset locale after file was written
	setlocale(LC_NUMERIC, locale);

	if_stream.close();
}

}

