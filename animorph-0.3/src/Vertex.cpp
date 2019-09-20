#include "../include/animorph/Vertex.h"

using namespace Animorph;

void Vertex::addSharedFace (int shared_face)
{
  shared_faces_vector.push_back (shared_face);
}

std::vector <int> &Vertex::getSharedFaces ()
{
  return shared_faces_vector;
}

/*Subdivision surfaces */

subdVertex::subdVertex (int v0, int v1, int v2, int v3)
:  size(4)
{
  co.zero ();
  no.zero ();
  vertices[0] = v0;
  vertices[1] = v1;
  vertices[2] = v2;
  vertices[3] = v3;
}

subdVertex::subdVertex (int v0, int v1, int v2)
:  size (3)
{
  co.zero ();
  no.zero ();
  vertices[0] = v0;
  vertices[1] = v1;
  vertices[2] = v2;
}

origVertex::origVertex (std::vector <int> &i_faceVerts, std::vector <int> &i_edgeVerts)
{
  valence = i_edgeVerts.size ();
  fvalence = i_faceVerts.size ();
  faceVerts = i_faceVerts;
  edgeVerts = i_edgeVerts;
  
}

