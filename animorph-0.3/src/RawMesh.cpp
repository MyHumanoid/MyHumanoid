#include <iostream>
#include "../include/animorph/RawMesh.h"

#ifdef DEBUG
  #include <StopClock/StopClock.h>
#endif //DEBUG

#include <assert.h>

using namespace std;
using namespace Animorph;

RawMesh::RawMesh()
: facevector    (),
  vertexvector  (),
  bodyfaces     (),
  clothesgroup  (),
  name()
{
}

RawMesh::~RawMesh()
{

}