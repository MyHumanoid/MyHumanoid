#include "animorph/ObjExporter.h"

using namespace std;
using namespace Animorph;

void ObjExporter::createOBJStream(ostringstream & out_stream, const string & basename)
{
	FaceGroup & facegroup(mesh.facegroup);
	facegroup.calcVertexes(mesh.getFaceVectorRef());

	const VertexVector &   vertexvector(mesh.getVertexVectorRef());
	const FaceVector &     facevector(mesh.getFaceVectorRef());
	const TextureVector &  texturevector(mesh.texture_vector);
	const MaterialVector & materialvector = mesh.materials();

	// write header
	out_stream << "# OBJ File" << endl;
	// out_stream << "# www..." << endl;

	out_stream << "mtllib "
	           << "materials.mtl" << endl;

	out_stream << "o " << basename << endl; // name of mesh

	for(auto & [partname, groupValue] : facegroup.m_groups) {

		const VertexData & vertexgroupdata(facegroup.getPartVertexesRef(partname));

		for(VertexData::const_iterator vertexgroup_it = vertexgroupdata.begin();
		    vertexgroup_it != vertexgroupdata.end(); vertexgroup_it++) {
			const Vertex & vertex(vertexvector[(*vertexgroup_it).first]);
			glm::vec3      vector(vertex.co * tm);

			out_stream << "v " << vector.x << " " << vector.y << " " << vector.z
			           << endl;
		}
	}

	for(auto & [partname, groupValue] : facegroup.m_groups) {

		const FGroupData & facegroupdata(groupValue.facesIndexes);
		// VertexData vertexgroupdata = facegroup.getPartVertexesRef(partname);

		// write texture UV coordinates
		if(facevector.size() == texturevector.size()) {
			for(unsigned int i = 0; i < facegroupdata.size(); i++) {
				const TextureFace & texture_face(texturevector[facegroupdata[i]]);

				for(unsigned int n = 0; n < texture_face.size(); n++) {
					const glm::vec2 & uv(texture_face[n]);

					// the -uv.y+1.0 stuff is a hack for renderman input UV data
					// TODO: change renderman import UV data
					out_stream << "vt " << uv.x << " " << -uv.y << " 0.0"
					           << endl;
				}
			}
		} else {
			cerr << "Couldn't export texture coordinates! " << facevector.size()
			     << " != " << texturevector.size() << endl;
		}
	}

	int v_offset  = 0;
	int vt_offset = 0;

	for(const auto & [partname, groupValue] : facegroup.m_groups) {

		const FGroupData & facegroupdata(groupValue.facesIndexes);
		const VertexData & vertexgroupdata(facegroup.getPartVertexesRef(partname));

		// write faces
		int old_material_index = -1;
		int texture_number     = 1;
		for(unsigned int i = 0; i < facegroupdata.size(); i++) {
			const Face & face(facevector[facegroupdata[i]]);

			int material_index = face.getMaterialIndex();

			if((material_index != -1) && (material_index != old_material_index)) {
				// material reference
				out_stream << "usemtl " << materialvector[material_index].getName()
				           << endl;
			}

			if(face.getSize() > 0)
				out_stream << "f ";

			for(unsigned int j = 0; j < face.getSize(); j++) {
				// fix: Prevent to access non existent hash entries (the operator[])
				// does implicitly insert an entry for a particular has if it does
				// not exists!
				VertexData::const_iterator vertex_iterator;
				if((vertex_iterator = vertexgroupdata.find(
				            face.getVertexAtIndex(j))) != vertexgroupdata.end()) {
					int vertex_number = vertex_iterator->second;
					// cout << texture_number << endl;

					// face vertex geometry
					out_stream << vertex_number + 1 + v_offset;

					out_stream << "/";

					out_stream << (texture_number + vt_offset) << " ";

					texture_number++;
				}
			}
			out_stream << endl;

			old_material_index = material_index;
		}
		v_offset += vertexgroupdata.size();

		for(unsigned int i = 0; i < facegroupdata.size(); i++) {
			const TextureFace & texture_face(texturevector[facegroupdata[i]]);
			vt_offset += texture_face.size();
		}
	}
}

void ObjExporter::createFullOBJStream(ostringstream & out_stream, const string & basename)
{
	VertexVector &   vertexvector = mesh.getVertexVectorRef();
	FaceVector &     facevector = mesh.getFaceVectorRef();
	const MaterialVector & materialvector = mesh.materials();
	TextureVector &  texturevector = mesh.texture_vector;

	// TODO: decide how much accracy we need
	// out_stream << setprecision (12);

	// write header
	out_stream << "# OBJ File" << endl;

	out_stream << "mtllib "
	           << "materials.mtl" << endl;

	out_stream << "o " << basename << endl; // name of mesh

	// write vertices
	for(unsigned int i = 0; i < vertexvector.size(); i++) {
		Vertex &  vertex = vertexvector[i];
		glm::vec3 vector(vertex.co * tm);

		out_stream << "v " << vector.x << " " << vector.y << " " << vector.z << endl;
	}

	// write texture UV coordinates
	if(facevector.size() == texturevector.size()) {
		for(unsigned int i = 0; i < facevector.size(); i++) {
			TextureFace & texture_face = texturevector[i];

			for(unsigned int n = 0; n < texture_face.size(); n++) {
				glm::vec2 & uv = texture_face[n];

				// the -uv.y+1.0 stuff is a hack for renderman input UV data
				// TODO: change renderman import UV data
				out_stream << "vt " << uv.x << " " << -uv.y << " 0.0" << endl;
			}
		}
	} else {
		cerr << "Couldn't export texture coordinates! " << facevector.size()
		     << " != " << texturevector.size() << endl;
	}

	// write faces
	int old_material_index = -1;
	int texture_number     = 1;
	for(unsigned int i = 0; i < facevector.size(); i++) {
		const Face & face(facevector[i]);

		int material_index = face.getMaterialIndex();

		if((material_index != -1) && (material_index != old_material_index)) {
			// material reference
			out_stream << "usemtl " << materialvector[material_index].getName() << endl;
		}

		if(face.getSize() > 0)
			out_stream << "f ";

		for(unsigned int j = 0; j < face.getSize(); j++) {
			int vertex_number = face.getVertexAtIndex(j);
			// cout << texture_number << endl;

			// face vertex geometry
			out_stream << vertex_number + 1;

			out_stream << "/";

			out_stream << texture_number << " ";

			texture_number++;
		}
		out_stream << endl;

		old_material_index = material_index;
	}
}

/*
void ObjExporter::createOBJStream (ostringstream &out_stream,
                                   const string& basename,
                                   const FGroupData &facegroupdata,
                                   VertexData &vertexgroupdata)
{
  VertexVector   &vertexvector   (mesh.getVertexVectorRef ());
  FaceVector     &facevector     (mesh.getFaceVectorRef ());
  TextureVector  &texturevector  (mesh.getTextureVectorRef ());

  // TODO: decide how much accracy we need
  //out_stream << setprecision (12);

  // write header
  out_stream << "# OBJ File" << endl;
  //out_stream << "# www..." << endl;

  out_stream << "o " << basename << endl; // name of mesh

  for (VertexData::iterator vertexgroup_it = vertexgroupdata.begin ();
       vertexgroup_it != vertexgroupdata.end ();
       vertexgroup_it++)
  {
    Vertex &vertex = vertexvector[(*vertexgroup_it).first];
    Vector3f vector = vertex.co * tm;

    out_stream <<
    "v " << vector.x <<
    " "  << vector.y <<
    " "  << vector.z << endl;
  }

  // write texture UV coordinates
  if (facevector.size () == texturevector.size ())
  {
    for (unsigned int i = 0; i < facegroupdata.size (); i++)
    {
      TextureFace &texture_face = texturevector[facegroupdata[i]];

      for (unsigned int n = 0; n < texture_face.size (); n++)
      {
        Vector2f &uv = texture_face[n];

        // the -uv.y+1.0 stuff is a hack for renderman input UV data
        // TODO: change renderman import UV data
        out_stream << "vt " << uv.x << " " << -uv.y << " 0.0" << endl;
      }
    }
  }
  else
  {
    cerr << "Couldn't export texture coordinates! "
    << facevector.size () << " != " << texturevector.size ()
    << endl;
  }

  // write faces
  int texture_number = 1;
  for (unsigned int i = 0; i < facegroupdata.size (); i++)
  {
    const Face &face(facevector[facegroupdata[i]]);

    if (face.getSize () > 0)
      out_stream << "f ";

    for (unsigned int j = 0; j < face.getSize(); j++)
    {
      int vertex_number = vertexgroupdata[face.getVertexAtIndex(j)];
      //cout << texture_number << endl;

      // face vertex geometry
      out_stream << vertex_number + 1;

      out_stream << "/";

      out_stream << texture_number << " ";

      texture_number++;
    }
    out_stream << endl;
  }
}
*/
bool ObjExporter::exportFile(const string & exportpath, bool full)
{

	FaceGroup & facegroup(mesh.facegroup);
	facegroup.calcVertexes(mesh.getFaceVectorRef());

	// FaceVector &facevector (mesh.getFaceVectorRef());

	FileWriter file_writer;

	file_writer.open(exportpath + (full ? "full" : "") + "mesh.obj");
	if(file_writer) {
		std::ostringstream out_stream;

		if(full) {
			createFullOBJStream(out_stream, "fullmesh.obj");
		} else {
			createOBJStream(out_stream, "mesh.obj");
		}

		file_writer << out_stream.str();
		file_writer.close();
	}

	if(!file_writer)
		return false;

	file_writer.open(exportpath + "materials.mtl");

	if(file_writer) {
		std::ostringstream out_stream;
		createMTLStream(out_stream, full ? "fullmesh.obj" : "mesh.obj");

		file_writer << out_stream.str();
		file_writer.close();
	}

	if(!file_writer)
		return false;

	return true;
}

void ObjExporter::createMTLStream(std::ostringstream & out_stream, const std::string & basename)
{
	const MaterialVector & materialvector = mesh.materials();

	out_stream << "# Material file for " << basename << endl << endl;

	for(unsigned int i = 0; i < materialvector.size(); i++) {
		const Material &    material = materialvector[i];
		const Color & colRGB   = material.getRGBCol();

		out_stream << "newmtl " << material.getName() << endl;
		out_stream << "Kd " << colRGB.red() << " " << colRGB.green() << " " << colRGB.blue()
		           << endl
		           << endl;
	}
}
