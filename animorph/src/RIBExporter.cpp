#include "../include/animorph/RIBExporter.h"

using namespace std;
using namespace Animorph;


void RIBExporter::createObjectStream (ostringstream &out_stream,
                                      const FGroupData &facegroupdata,
                                      const VertexData &vertexgroupdata)
{
  const VertexVector &vertexvector(mesh.getVertexVectorRef ());
  const FaceVector &facevector(mesh.getFaceVectorRef ());
  const TextureVector &texturevector(mesh.getTextureVectorRef ());

  out_stream << "Declare \"st\" \"facevarying float[2]\"" << endl;

  // write header
  out_stream << "SubdivisionMesh \"catmull-clark\" ";

  // write number of vertices in a face
  out_stream << "[";

  for (unsigned int i = 0; i < facegroupdata.size (); i++)
  {
    const Face &face(facevector[facegroupdata[i]]);

    out_stream << face.getSize() << " ";
  }
  out_stream << "] " << endl;

  // write all vertex numbers in a face
  out_stream << "[";
  for (unsigned int i = 0; i < facegroupdata.size (); i++)
  {
    const Face &face(facevector[facegroupdata[i]]);

    for (unsigned int n = face.getSize(); n > 0; n--)
    {
      VertexData::const_iterator vertexData_it;
      vertexData_it = vertexgroupdata.find(face.getVertexAtIndex(n - 1));
      if (vertexData_it != vertexgroupdata.end())
      {
        const int v = vertexData_it->second;

        out_stream << v << " ";
      }
    }
  }
  out_stream << "]" << endl;
  out_stream << "[\"interpolateboundary\"] [0 0] [0] [0]" << endl;

  // write vertices
  out_stream << "\"P\" [";
  for (VertexData::const_iterator vertexgroup_it = vertexgroupdata.begin ();
       vertexgroup_it != vertexgroupdata.end ();
       vertexgroup_it++)
  {
    const Vertex &vertex(vertexvector[(*vertexgroup_it).first]);
    Vector3f vector = vertex.co/* * tm*/;

    out_stream << -vector.x <<
    " "  << vector.y <<
    " "  << vector.z << " ";
  }
  out_stream << "]";

  // write UV coordinates
  if (facevector.size () == texturevector.size ())
  {
    out_stream << "\"st\" [";

    for (unsigned int i = 0; i < facegroupdata.size (); i++)
    {
      const TextureFace &texture_face(texturevector[facegroupdata[i]]);

      for (unsigned int n = texture_face.size (); n > 0; n--)
      //for (unsigned int n = 0; n < texture_face.size (); n++)
      {

        const Vector2f &uv_texture(texture_face[n - 1]);

        // TODO: prevent useless spaces here?
        // TODO: if UV input data changes use 1.0-y here!
        out_stream << uv_texture.x << " "
        << uv_texture.y << " ";
      }
    }
    out_stream << "]";
  }
}

void RIBExporter::replaceRIBTags (ifstream                &in_stream,
                                  ostringstream           &outStream,
                                  const list <StringPair> &replaceList)
{
  char buffer[MAX_LINE_BUFFER];
  string hs_name;
  while (in_stream.getline (buffer, MAX_LINE_BUFFER))
  {
    string newLine (buffer);

    for (list<StringPair>::const_iterator sl_it = replaceList.begin ();
         sl_it != replaceList.end ();
         sl_it++)
    {
      const StringPair &strP_ref = *sl_it;
      const string &s1 = strP_ref.first;
      const string &s2 = strP_ref.second;

      replaceString (s1, s2, newLine);
    }


    outStream << newLine << endl;
  }
}

bool RIBExporter::exportFile (const string &outFile)
{
  FaceGroup &facegroup(mesh.getFaceGroupRef());
  facegroup.calcVertexes(mesh.getFaceVectorRef ());

  FaceGroup &clothesgroup (mesh.getClothesGroupRef ());
  clothesgroup.calcVertexes (mesh.getFaceVectorRef ());

  FileWriter file_writer;
  for (FaceGroup::iterator facegroup_it = facegroup.begin ();
       facegroup_it != facegroup.end ();
       facegroup_it++)
  {
    const string& partname((*facegroup_it).first);
    FGroupData &groupdata = (*facegroup_it).second.facesIndexes;

    file_writer.open (outFile + partname + ".rib");

    if (file_writer)
    {
      std::ostringstream out_stream;

      createObjectStream (out_stream,
                          groupdata,
                          facegroup.getPartVertexesRef(partname));

      file_writer << out_stream.str ();
      file_writer.close ();
    }

    if (!file_writer)
      return false;
  }

  for (FaceGroup::const_iterator clothesgroup_it = clothesgroup.begin ();
       clothesgroup_it != clothesgroup.end ();
       clothesgroup_it++)
  {
    FGroup clothes = (*clothesgroup_it).second;
    if(!clothes.visible) continue;

    string cl_name("_clothes_" + (*clothesgroup_it).first);
    FGroupData &groupdata = clothes.facesIndexes;
    file_writer.open (outFile + cl_name + ".rib");

    if (file_writer)
    {
      std::ostringstream out_stream;

      createObjectStream (out_stream,
                          groupdata,
                          facegroup.getPartVertexesRef(cl_name));

      file_writer << out_stream.str ();
      file_writer.close ();
    }

    if (!file_writer)
      return false;
  }

  return true;
}

bool RIBExporter::exportFile (const string            &templateDirectory,
                              const string            &templateFile,
                              const string            &outFile,
                              const list <StringPair> &replaceList)
{
  string file_ending_cut = cutFileEnding (outFile, ".rib");

  string bname = file_ending_cut.substr (outFile.find_last_of (PATH_SEPARATOR)+1, outFile.size ());

  FileReader file_reader;

  file_reader.open (templateDirectory + PATH_SEPARATOR + templateFile);

  if (!file_reader)
    return false;

  FileWriter file_writer;
  file_writer.open (file_ending_cut + ".rib");

  if (file_writer)
  {
    std::ostringstream out_stream;
    //createObjectStream (out_stream, outFile);
    replaceRIBTags (file_reader, out_stream, replaceList);

    file_writer << out_stream.str ();
    file_writer.close ();
  }

  if (!file_writer)
    return false;

  return true;
}
