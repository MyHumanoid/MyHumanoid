#include "animorph/RawMesh.h"

#include <iostream>

#ifdef DEBUG
#include <StopClock/StopClock.h>
#endif // DEBUG

#include <assert.h>

using namespace std;
using namespace Animorph;

RawMesh::RawMesh()
        : facevector()
        , vertexvector()
        , bodyfaces()
        , name()
{
}

RawMesh::~RawMesh() {}
