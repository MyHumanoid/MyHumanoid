#include "animorph/BodySettings.h"

#include <string.h>

#include "Logger.h"

namespace Animorph
{

bool BodySettings::load(const std::string & filename)
{
	FileReader file_reader;
	
	log_debug("Open File: {}", filename);
	if(!file_reader.open(filename))
		return false;

	fromStream(file_reader);

	return true;
}

bool BodySettings::save(const std::string & filename)
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

void BodySettings::fromStream(FileReader & in_stream)
{
	float bs_value;

	// empty map before load new settings
	m_targets.clear();

	std::string buffer;
	char bs_name[MAX_LINE_BUFFER];
	while(in_stream.getline(buffer)) {
		if(sscanf(buffer.c_str(), "%[^,],%f\n", bs_name, &bs_value) == 2) {
			if(bs_value != 0.0) {
				// remove "targets/" prefix in MH2.0 bodysettings
				// as side effect a sub directory with name "targets" is forbidden!!
				string bs_name_str = bs_name;
				if(bs_name_str.substr(0, strlen("targets/")) == "targets/") {
					bs_name_str.erase(0, strlen("targets/"));
				}

				float & value_ref = m_targets[bs_name_str];
				value_ref         = bs_value;
			}
		} else {
			if(buffer[0] != '#') {
				std::cerr << "Not allowed line in BodySetting:" << std::endl
				     << buffer << std::endl;
			} else {
				
				uint32_t id;
				int      x, y;
				if(sscanf(buffer.c_str(), "#t,%*c,%u,%i,%i", &id, &x, &y) == 3) {
					
					switch(id) {
					case kAge:
						m_comp.m_kAge = glm::ivec2(x, y);
						break;
					case kMuscleSize:
						m_comp.m_kMuscleSize = glm::ivec2(x, y);
						break;
					case kBreast:
						m_comp.m_kBreast = glm::ivec2(x, y);
						break;
					case kShape:
						m_comp.m_kShape = glm::ivec2(x, y);
						break;
					default:
						log_error("Unexpected value");
					}
				}
				
			}
		}
	}
}

void BodySettings::createStream(std::ostringstream & out_stream)
{
	for(const auto & bodyset_it : m_targets) {
		string name        = bodyset_it.first;
		float  morph_value = bodyset_it.second;
		out_stream << name << "," << morph_value << "\r\n";
	}
	
	glm::ivec2 tmp;
	
	tmp = m_comp.m_kAge;
	if(tmp != InvalidPoint) {
		out_stream << "#t,A," << kAge << ","
		           << tmp.x << "," << tmp.y << "\r\n";
	}
	
	tmp = m_comp.m_kMuscleSize;
	if(tmp != InvalidPoint) {
		out_stream << "#t,M," << kMuscleSize << ","
		           << tmp.x << "," << tmp.y << "\r\n";
	}
	
	tmp = m_comp.m_kBreast;
	if(tmp != InvalidPoint) {
		out_stream << "#t,B," << kBreast << ","
		           << tmp.x << "," << tmp.y << "\r\n";
	}
	
	tmp = m_comp.m_kShape;
	if(tmp != InvalidPoint) {
		out_stream << "#t,S," << kShape << ","
		           << tmp.x << "," << tmp.y;
	}
}

}

