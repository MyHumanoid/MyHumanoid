#include "animorph/VertexVector.h"
#include "animorph/util.h"

#include <iostream>

using namespace std;
using namespace Animorph;

bool VertexVector::load(const std::string &filename)
{
	FileReader file_reader;

	file_reader.open(filename);

	if (!file_reader)
		return false;

	fromStream(file_reader);

	return true;
}

void VertexVector::fromStream(std::ifstream &in_stream)
{
	char buffer[MAX_LINE_BUFFER];
	float x, y, z;
	x = y = z = 0.0;

	clear();

	while (in_stream.getline(buffer, MAX_LINE_BUFFER)) {
		if (sscanf(buffer, "%f,%f,%f\n", &x, &y, &z) == 3) {
			Vertex vertex(x, y, z);

			push_back(vertex);
		} else {
			cerr << "illegal line in vertex data file:" << endl << buffer << endl;
		}
	}
}

int VertexVector::setCoordinates(std::vector<Vector3f> &vertexvector_orginal)
{
	VertexVector &vertexvector_morph = (*this);

	// false number of vertices!
	if (vertexvector_orginal.size() != size())
		return -1;

	for (unsigned int i = 0; i < size(); i++) {
		Vertex &vertex_morph = vertexvector_morph[i];
		Vector3f &vertex_orginal = vertexvector_orginal[i];

		vertex_morph.co = vertex_orginal;
	}

	return 0;
}

bool origVertexVector::load(const std::string &filename)
{
	FileReader file_reader;

	file_reader.open(filename);

	if (!file_reader)
		return false;

	fromStream(file_reader);

	return true;
}

void origVertexVector::fromStream(std::ifstream &in_stream)
{
	char buffer[MAX_LINE_BUFFER];
	char buffer1[MAX_LINE_BUFFER];

	clear();

	while (in_stream.getline(buffer, MAX_LINE_BUFFER) &&
	       in_stream.getline(buffer1, MAX_LINE_BUFFER)) {
		std::vector<int> i_faceVerts;

		stringTokeni(buffer, ",", i_faceVerts);

		std::vector<int> i_edgeVerts;

		stringTokeni(buffer1, ",", i_edgeVerts);
		push_back(origVertex(i_faceVerts, i_edgeVerts));
	}
}
