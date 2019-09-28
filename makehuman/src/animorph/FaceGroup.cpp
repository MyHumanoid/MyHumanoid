#include "animorph/FaceGroup.h"

#include <string.h>

using namespace std;
using namespace Animorph;

const int MAX_LINE = 350000;

bool FaceGroup::load(const std::string &filename)
{
	FileReader file_reader;

	file_reader.open(filename);

	if (!file_reader)
		return false;

	fromStream(file_reader);

	loadVisibilities(filename);

	return true;
}

void FaceGroup::calcVertexes(const FaceVector &facevector)
{
	if (loaded)
		return;

	loaded = true;

	int vertexCounter;
	
	for(auto & [partname, groupValue]: m_groups) {
		
		FGroupData &groupdata = groupValue.facesIndexes;
		vertexCounter = 0;

		for (unsigned int i = 0; i < groupdata.size(); i++) {
			const Face &face(facevector[groupdata[i]]);

			for (unsigned int j = 0; j < face.getSize(); j++) {
				const int tmp = face.getVertexAtIndex(j);

				if (vertexes[partname].find(tmp) == vertexes[partname].end()) {
					vertexes[partname][tmp] = 0;
				}
			}
		}

		for (VertexData::iterator vertexgroup_it = vertexes[partname].begin();
		     vertexgroup_it != vertexes[partname].end(); vertexgroup_it++) {
			vertexes[partname][(*vertexgroup_it).first] = vertexCounter++;
		}
	}

	// cout << vertexes.size() << endl;
}

void FaceGroup::fromStream(std::ifstream &in_stream)
{
	FGroup data;
	string fgroup_ident;

	m_groups.clear();

	char buffer[MAX_LINE];
	while (in_stream.getline(buffer, MAX_LINE)) {
		if (isalpha(buffer[0])) // line is a fgroup identifier
		{
			char *p;
			p = strrchr(buffer, ',');
			if (p == NULL)
				p = buffer;
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

void FaceGroup::toggleVisible(const std::string &name)
{
	if (m_groups.count(name)) {
		m_groups[name].visible = !(m_groups[name].visible);
	}
}

bool FaceGroup::saveVisibilities(const std::string &filename,
                                 std::ios_base::openmode mode)
{
	FileWriter file_writer;
	file_writer.open(filename, mode);

	if (!file_writer)
		return false;

	std::ostringstream out_stream;
	createStreamVisibilities(out_stream);

	file_writer << out_stream.str();

	return true;
}

bool FaceGroup::loadVisibilities(const std::string &filename)
{
	FileReader file_reader;

	file_reader.open(filename);

	if (!file_reader)
		return false;

	fromStreamVisibilities(file_reader);

	return true;
}

void FaceGroup::createStreamVisibilities(std::ostringstream &out_stream)
{
	for(const auto & [partname, groupValue]: m_groups) {
		bool visible = groupValue.visible;

		if (visible) {
			out_stream << "#v," << partname << endl;
		}
	}
}

void FaceGroup::fromStreamVisibilities(std::ifstream &in_stream)
{
	char buffer[MAX_LINE];
	char line[MAX_LINE];
	while (in_stream.getline(buffer, MAX_LINE)) {
		if (sscanf(buffer, "nv,%s", line) == 1) {
			// ema     if(find(line) != end())
			{
				m_groups[line].visible = false;
			}
		}
	}
}
