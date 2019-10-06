#include "animorph/VertexGroup.h"
#include "animorph/util.h"

#include "Logger.h"

using std::string;

namespace Animorph
{

bool VertexGroup::load(const std::string & filename)
{
	FileReader file_reader;

	if(!file_reader.open(filename))
		return false;

	fromStream(file_reader);

	return true;
}

void VertexGroup::fromStream(FileReader & in_stream)
{
	int    vertex_number;
	float  weight;
	string vgroup_ident;

	clear();

	std::string buffer;
	while(in_stream.getline(buffer)) {
		if(isalpha(buffer[0])) // line is a vgroup identifier
		{
			vgroup_ident = buffer;

			UtilStringDelSurround(vgroup_ident, "\n\r\t:");
		} else // line is a vgroup number
		{
			if(sscanf(buffer.c_str(), "%d,%f\n", &vertex_number, &weight) == 2) {
				if(vgroup_ident != "") {
					VGroupData & vgd   = (*this)[vgroup_ident];
					vgd[vertex_number] = weight;
				}
			} else {
				log_error("illegal VertexGroup format line: {}", buffer);
			}
		}
	}
}

}
