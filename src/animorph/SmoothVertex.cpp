#include "animorph/SmoothVertex.h"
#include "animorph/util.h"

namespace Animorph
{

bool SmoothVertex::load(const std::string & filename)
{
	FileReader file_reader;

	file_reader.open(filename);

	if(!file_reader)
		return false;

	fromStream(file_reader);

	return true;
}

void SmoothVertex::fromStream(std::ifstream & in_stream)
{
	clear();

	char buffer[MAX_LINE];
	while(in_stream.getline(buffer, MAX_LINE)) {
		SmoothData vertexes;

		stringTokeni(buffer, ",", vertexes);
		push_back(vertexes);
	}
}

}
