#include "animorph/FaceGroup.h"

#include <string.h>

namespace Animorph
{

const int MAX_LINE = 350000;

bool FaceGroup::load(const std::string & filename)
{
	FileReader file_reader;

	if(!file_reader.open(filename))
		return false;

	fromStream(file_reader);

	loadVisibilities(filename);

	return true;
}

void FaceGroup::fromStream(FileReader & in_stream)
{
	FGroup data;
	string fgroup_ident;

	m_groups.clear();

	std::string buffer;
	while(in_stream.getline(buffer)) {
		if(isalpha(buffer[0])) // line is a fgroup identifier
		{
			const char * p;
			p = strrchr(buffer.c_str(), ',');
			if(p == NULL)
				p = buffer.c_str();
			else
				p++;

			data.facesIndexes.clear();
			fgroup_ident = p;

			UtilStringDelSurround(fgroup_ident, "\n\r\t:");
		} else // line is a vgroup number
		{
			data.visible = true;
			stringTokeni(buffer, " ", data.facesIndexes);
			m_groups[fgroup_ident] = data;
		}
	}
}

void FaceGroup::toggleVisible(const std::string & name)
{
	if(m_groups.count(name)) {
		m_groups[name].visible = !(m_groups[name].visible);
	}
}

bool FaceGroup::saveVisibilities(const std::string & filename, std::ios_base::openmode mode)
{
	FileWriter file_writer;
	file_writer.open(filename, mode);

	if(!file_writer)
		return false;

	std::ostringstream out_stream;
	createStreamVisibilities(out_stream);

	file_writer << out_stream.str();

	return true;
}

bool FaceGroup::loadVisibilities(const std::string & filename)
{
	FileReader file_reader;

	if(!file_reader.open(filename))
		return false;

	fromStreamVisibilities(file_reader);

	return true;
}

void FaceGroup::createStreamVisibilities(std::ostringstream & out_stream)
{
	for(const auto & [partname, groupValue] : m_groups) {
		bool visible = groupValue.visible;

		if(visible) {
			out_stream << "#v," << partname << std::endl;
		}
	}
}

void FaceGroup::fromStreamVisibilities(FileReader & in_stream)
{
	std::string buffer;
	char line[MAX_LINE];
	while(in_stream.getline(buffer)) {
		if(sscanf(buffer.c_str(), "nv,%s", line) == 1) {
			// ema     if(find(line) != end())
			{
				m_groups[line].visible = false;
			}
		}
	}
}

}

