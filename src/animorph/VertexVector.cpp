#include "animorph/VertexVector.h"

#include "animorph/FileReader.h"
#include "Logger.h"

namespace Animorph
{

bool loadVertexVector(VertexVector & vv, const std::string & filename)
{
	FileReader file_reader;
	
	log_debug("Open File: {}", filename);
	if(!file_reader.open(filename))
		return false;

	std::string buffer;
	float x, y, z;
	x = y = z = 0.0;

	vv.m_verts.clear();

	while(file_reader.getline(buffer)) {
		if(sscanf(buffer.c_str(), "%f,%f,%f\n", &x, &y, &z) == 3) {
			Vertex vertex(x, y, z);

			vv.m_verts.push_back(vertex);
		} else {
			log_error("illegal line in vertex data file: {}", buffer);
		}
	}
	
	vv.m_normals.resize(vv.m_verts.size());
	return true;
}

int VertexVector::setCoordinates(std::vector<glm::vec3> & vertexvector_orginal)
{
	VertexVector & vertexvector_morph = (*this);

	// false number of vertices!
	if(vertexvector_orginal.size() != m_verts.size())
		return -1;

	for(unsigned int i = 0; i < m_verts.size(); i++) {
		Vertex &    vertex_morph   = vertexvector_morph.m_verts[i];
		glm::vec3 & vertex_orginal = vertexvector_orginal[i];

		vertex_morph = vertex_orginal;
	}

	return 0;
}

}
