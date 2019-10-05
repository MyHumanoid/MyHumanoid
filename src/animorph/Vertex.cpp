#include "animorph/Vertex.h"

namespace Animorph
{

void VertexMeta::addSharedFace(int shared_face)
{
	shared_faces_vector.push_back(shared_face);
}

const std::vector<int> & VertexMeta::getSharedFaces() const
{
	return shared_faces_vector;
}

origVertex::origVertex(std::vector<int> & i_faceVerts, std::vector<int> & i_edgeVerts)
{
	valence   = i_edgeVerts.size();
	fvalence  = i_faceVerts.size();
	faceVerts = i_faceVerts;
	edgeVerts = i_edgeVerts;
}

}
