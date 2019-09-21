#include "../include/animorph/FileWriter.h"

#include <locale.h>

using namespace std;
using namespace Animorph;

int FileWriter::open(const std::string &filename, std::ios_base::openmode mode)
{
	ofstream &of_stream = (*this);

	// Because float-values should be 0.1 instead of 0,1
	// in non-english if read from file. Later reset the locale
	locale = setlocale(LC_NUMERIC, NULL);
	setlocale(LC_NUMERIC, "C");

	of_stream.open(filename.c_str(), mode);
	if (!of_stream) {
		cerr << "Couldn't open file:" << filename << endl;
		return -1;
	}

	return 0;
}

void FileWriter::close()
{
	ofstream &of_stream = (*this);

	// reset locale after file was written
	setlocale(LC_NUMERIC, locale);

	of_stream.close();
}
