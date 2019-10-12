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

}
