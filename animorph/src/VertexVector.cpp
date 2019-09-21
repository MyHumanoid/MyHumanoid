#include "../include/animorph/VertexVector.h"
#include "../include/animorph/util.h"

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

/*Subdivision surfaces */

bool subdVertexVector::load(const std::string &filename)
{
	FileReader file_reader;

	file_reader.open(filename);

	if (!file_reader)
		return false;

	fromStream(file_reader);

	return true;
}

void subdVertexVector::loadFromFaceVector(FaceVector &facevector)
{
	for (FaceVector::iterator facevector_it = facevector.begin();
	     facevector_it != facevector.end(); facevector_it++) {
		int v0, v1, v2, v3;
		Face face = *facevector_it;
		unsigned int facesize = face.getSize();

		v0 = face.getVertexAtIndex(0);
		v1 = face.getVertexAtIndex(1);
		v2 = face.getVertexAtIndex(2);

		if (facesize == 3) {
			push_back(subdVertex(v0, v1, v2));
		} else if (facesize == 4) {
			v3 = face.getVertexAtIndex(3);
			push_back(subdVertex(v0, v1, v2, v3));
		}
	}
}

void subdVertexVector::fromStream(std::ifstream &in_stream)
{
	char buffer[MAX_LINE_BUFFER];
	int v0, v1, v2, v3, nr_indices;
	v0 = v1 = v2 = v3 = 0;

	clear();

	while (in_stream.getline(buffer, MAX_LINE_BUFFER)) {
		nr_indices = sscanf(buffer, "%d,%d,%d,%d\n", &v0, &v1, &v2, &v3);

		if (nr_indices == 4) {
			push_back(subdVertex(v0, v1, v2, v3));
		} else if (nr_indices == 3) {
			push_back(subdVertex(v0, v1, v2));
		} else {
			cerr << "illegal line in subdvertex data file:" << endl << buffer << endl;
			continue;
		}
	}
}

void subdVertexVector::updateFacePoints(VertexVector &vertexvector)
{
	for (subdVertexVector::iterator subdvv_it = (*this).begin();
	     subdvv_it != (*this).end(); subdvv_it++) {
		// subdVertex &subdvertex = *subdvv_it;
		(*subdvv_it).co.zero();
		// unsigned int facesize = subdvertex.getSize();
		int j;

		for (j = 0; j != (*subdvv_it).getSize(); ++j) {
			(*subdvv_it).co += vertexvector[(*subdvv_it).getVertexAtIndex(j)].co;
		}
		(*subdvv_it).co /= (*subdvv_it).getSize();
	}
}

void subdVertexVector::updateEdgePoints(VertexVector &vertexvector,
                                        subdVertexVector &facepoints)
{
	for (subdVertexVector::iterator subdvv_it = (*this).begin();
	     subdvv_it != (*this).end(); subdvv_it++) {
		// subdVertex &subdvertex = *subdvv_it;
		(*subdvv_it).co.zero();
		float size = (*subdvv_it).getSize();
		int j;

		(*subdvv_it).co = (vertexvector[(*subdvv_it).getVertexAtIndex(0)].co +
		                   vertexvector[(*subdvv_it).getVertexAtIndex(1)].co);

		for (j = 2; j != size; ++j) {
			(*subdvv_it).co += facepoints[(*subdvv_it).getVertexAtIndex(j)].co;
		}
		(*subdvv_it).co /= size;
	}
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

void origVertexVector::updateOrigVertexPoints(VertexVector &vertexvector,
                                              subdVertexVector &facepoints,
                                              subdVertexVector &edgepoints)
{
	size_t j(0);

	for (origVertexVector::iterator origv_it = (*this).begin();
	     origv_it != (*this).end(); origv_it++) {

		(*origv_it).co.zero();

		(*origv_it).co = vertexvector[j].co;

		(*origv_it).co *= ((*origv_it).getValence() - 3);

		int k(0);
		Vector3f tempR;
		Vector3f tempQ;
		tempR.zero();
		tempQ.zero();

		for (k = 0; k != (*origv_it).getValence(); ++k) {
			tempR += edgepoints[(*origv_it).getEdgeVertexAtIndex(k)].co;
		}

		tempR /= (*origv_it).getValence();
		tempR *= 2.0f;

		(*origv_it).co += tempR;

		for (k = 0; k != (*origv_it).getFValence(); ++k) {
			tempQ += facepoints[(*origv_it).getFaceVertexAtIndex(k)].co;
		}

		tempQ /= ((*origv_it).getFValence());
		(*origv_it).co += tempQ;
		(*origv_it).co /= (*origv_it).getValence();
		j++;
	}
}
