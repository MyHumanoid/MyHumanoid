#include "animorph/Target.h"

#include "Logger.h"

namespace Animorph
{

void Target::createStream(std::ostringstream & out_stream)
{
	for(Target::iterator target_it = begin(); target_it != end(); target_it++) {
		TargetData td = (*target_it);

		out_stream << td.vertex_number << "," << td.morph_vector.x << ","
		           << td.morph_vector.y << "," << td.morph_vector.z << "," << std::endl;
	}
}

bool Target::load(const std::string & filename)
{
	clear();
	
	Target & target(*this);
	
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
			clear();
			return false;
		}
		
		target.push_back(td);
		modVertex.push_back(td.vertex_number);
	}
	log_debug("Loaded {: <8} target vectors from {}", target.size(), filename);
	
	return true;
}

bool Target::save(const std::string & filename)
{
	FileWriter file_writer;

	file_writer.open(filename);

	if(!file_writer)
		return false;

	std::ostringstream out_stream;
	createStream(out_stream);

	file_writer << out_stream.str();

	return true;
}

}
