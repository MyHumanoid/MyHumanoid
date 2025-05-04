#include "animorph/SmoothVertex.h"

#include "animorph/FileReader.h"
#include "animorph/util.h"
#include "Logger.h"

namespace Animorph
{

bool loadSmoothVertex(SmoothVertex &sv, const std::string & filename)
{
	FileReader file_reader;
	
	log_debug("Open File: {}", filename);
	if(!file_reader.open(filename))
		return false;

	sv.clear();

	std::string buffer;
	while(file_reader.getline(buffer)) {
		SmoothData vertexes;

		stringTokeni(buffer, ",", vertexes);
		sv.push_back(vertexes);
	}
	return true;
}

}
