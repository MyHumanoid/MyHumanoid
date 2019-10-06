#include "animorph/VertexVector.h"
#include "animorph/util.h"

#include <iostream>

#include "Logger.h"

namespace Animorph
{

bool VertexVector::load(const std::string & filename)
{
	FileReader file_reader;

	if(!file_reader.open(filename))
		return false;

	fromStream(file_reader);

	return true;
}

void VertexVector::fromStream(FileReader & in_stream)
{
	std::string buffer;
	float x, y, z;
	x = y = z = 0.0;

	clear();

	while(in_stream.getline(buffer)) {
		if(sscanf(buffer.c_str(), "%f,%f,%f\n", &x, &y, &z) == 3) {
			Vertex vertex(x, y, z);

			push_back(vertex);
		} else {
			log_error("illegal line in vertex data file: {}", buffer);
		}
	}
}

int VertexVector::setCoordinates(std::vector<glm::vec3> & vertexvector_orginal)
{
	VertexVector & vertexvector_morph = (*this);

	// false number of vertices!
	if(vertexvector_orginal.size() != size())
		return -1;

	for(unsigned int i = 0; i < size(); i++) {
		Vertex &    vertex_morph   = vertexvector_morph[i];
		glm::vec3 & vertex_orginal = vertexvector_orginal[i];

		vertex_morph.pos = vertex_orginal;
	}

	return 0;
}

bool origVertexVector::load(const std::string & filename)
{
	FileReader file_reader;

	if(!file_reader.open(filename))
		return false;

	fromStream(file_reader);

	return true;
}

void origVertexVector::fromStream(FileReader & in_stream)
{
	std::string buffer;
	std::string buffer1;

	clear();

	while(in_stream.getline(buffer) &&
	      in_stream.getline(buffer1)) {
		std::vector<int> i_faceVerts;

		stringTokeni(buffer, ",", i_faceVerts);

		std::vector<int> i_edgeVerts;

		stringTokeni(buffer1, ",", i_edgeVerts);
		push_back(origVertex(i_faceVerts, i_edgeVerts));
	}
}

}
