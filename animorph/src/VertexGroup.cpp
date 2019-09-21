#include "../include/animorph/VertexGroup.h"
#include "../include/animorph/util.h"

using namespace std;
using namespace Animorph;

bool VertexGroup::load(const std::string &filename)
{
	FileReader file_reader;

	file_reader.open(filename);

	if (!file_reader)
		return false;

	fromStream(file_reader);

	return true;
}

void VertexGroup::fromStream(std::ifstream &in_stream)
{
	int vertex_number;
	float weight;
	string vgroup_ident;

	clear();

	char buffer[MAX_LINE_BUFFER];
	while (in_stream.getline(buffer, MAX_LINE_BUFFER)) {
		if (isalpha(buffer[0])) // line is a vgroup identifier
		{
			vgroup_ident = buffer;

			UtilStringDelSurround(vgroup_ident, "\n\r\t:");
		} else // line is a vgroup number
		{
			if (sscanf(buffer, "%d,%f\n", &vertex_number, &weight) == 2) {
				if (vgroup_ident != "") {
					VGroupData &vgd = (*this)[vgroup_ident];
					vgd[vertex_number] = weight;
				}
			} else {
				cerr << "illegal VertexGroup format line:" << endl << buffer << endl;
			}
		}
	}
}
