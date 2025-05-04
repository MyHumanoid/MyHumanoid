#include "animorph/Target.h"

#include "Logger.h"

namespace Animorph
{

bool loadTarget(Target &target, const std::string & filename)
{
	target.clear();
	
	FileReader reader;
	if(!reader.open(filename)) {
		return false;
	}
	
	std::string buffer;
	while(reader.getline(buffer)) {
		TargetData td;
		
		auto ret = sscanf(buffer.c_str(), "%d,%f,%f,%f",
		                  &td.vertex_number,
		                  &td.morph_vector.x,
		                  &td.morph_vector.y,
		                  &td.morph_vector.z);
		
		if((ret != 4) && (ret != 0)) {
			log_error("Illegal line while reading target '{}'!", filename);
			target.clear();
			return false;
		}
		
		target.push_back(td);
		target.modVertex.push_back(td.vertex_number);
	}
	log_debug("Loaded {: <8} target vectors from {}", target.size(), filename);
	
	return true;
}

bool saveTarget(Target &t, const std::string & filename)
{
	FileWriter file_writer;

	file_writer.open(filename);

	if(!file_writer)
		return false;

	std::ostringstream out_stream;
	for(Target::iterator target_it = t.begin(); target_it != t.end(); target_it++) {
		TargetData td = (*target_it);
		
		out_stream << td.vertex_number << "," << td.morph_vector.x << ","
		           << td.morph_vector.y << "," << td.morph_vector.z << "," << std::endl;
	}

	file_writer << out_stream.str();

	return true;
}

}
