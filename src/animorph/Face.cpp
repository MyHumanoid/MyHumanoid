#include "animorph/Face.h"

namespace Animorph
{

Face::Face(int v0, int v1, int v2, int v3)
        : vertices()
        , size(4)
        , m_materialIndex(-1)
        , no()
{
	vertices[0] = v0;
	vertices[1] = v1;
	vertices[2] = v2;
	vertices[3] = v3;
}

Face::Face(int v0, int v1, int v2)
        : vertices()
        , size(3)
        , m_materialIndex(-1)
        , no()
{
	vertices[0] = v0;
	vertices[1] = v1;
	vertices[2] = v2;
}

}

