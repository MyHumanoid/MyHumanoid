#include "animorph/SmoothVertex.h"

#include "animorph/util.h"
#include "Logger.h"

namespace Animorph
{

bool SmoothVertex::load(const std::string & filename)
{
	FileReader file_reader;
	
	log_debug("Open File: {}", filename);
	if(!file_reader.open(filename))
		return false;

	fromStream(file_reader);

	return true;
}

void SmoothVertex::fromStream(FileReader & in_stream)
{
	clear();

	std::string buffer;
	while(in_stream.getline(buffer)) {
		SmoothData vertexes;

		stringTokeni(buffer, ",", vertexes);
		push_back(vertexes);
	}
}

}
