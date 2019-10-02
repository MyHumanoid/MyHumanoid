#include "animorph/ObjExporter.h"

#include "log/log.h"
#include "util/StringUtils.h"

using namespace std;
using namespace Animorph;

static void createOBJStream(Mesh & mesh,
                            const Matrix & tm,
                     ostringstream & out_stream,
                     const string & objRelPath,
                     const string & mtlRelPath)
{
	mesh.facegroupCalcVertexes();
	const FaceGroup & facegroup(mesh.facegroup());

	const VertexVector &   vertexvector(mesh.getVertexVectorRef());
	const FaceVector &     facevector(mesh.faces());
	const TextureVector &  texturevector(mesh.textureVector());
	const MaterialVector & materialvector = mesh.materials();

	// write header
	out_stream << "# OBJ File" << endl;
	// out_stream << "# www..." << endl;

	out_stream << "mtllib " << mtlRelPath << endl;

	out_stream << "o " << objRelPath << endl; // name of mesh

	for(auto & [partname, groupValue] : facegroup.m_groups) {

		const VertexData & vertexgroupdata(facegroup.getPartVertexesRef(partname));

		for(VertexData::const_iterator vertexgroup_it = vertexgroupdata.begin();
		    vertexgroup_it != vertexgroupdata.end(); vertexgroup_it++) {
			const Vertex & vertex(vertexvector[(*vertexgroup_it).first]);
			glm::vec3      vector(vertex.pos * tm);

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
			log_error("Couldn't export texture coordinates! {} != {}",
			          facevector.size(), texturevector.size());
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

// TODO always do "full" export
void ObjExporter::createFullOBJStream(ostringstream & out_stream, const string & basename)
{
	const VertexVector &   vertexvector   = mesh.getVertexVectorRef();
	const FaceVector &     facevector     = mesh.faces();
	const MaterialVector & materialvector = mesh.materials();
	const TextureVector &  texturevector  = mesh.textureVector();

	// TODO: decide how much accracy we need
	// out_stream << setprecision (12);

	// write header
	out_stream << "# OBJ File" << endl;

	out_stream << "mtllib "
	           << "materials.mtl" << endl;

	out_stream << "o " << basename << endl; // name of mesh

	// write vertices
	for(unsigned int i = 0; i < vertexvector.size(); i++) {
		const Vertex & vertex = vertexvector[i];
		glm::vec3      vector(vertex.pos * tm);

		out_stream << "v " << vector.x << " " << vector.y << " " << vector.z << endl;
	}

	// write texture UV coordinates
	if(facevector.size() == texturevector.size()) {
		for(unsigned int i = 0; i < facevector.size(); i++) {
			const TextureFace & texture_face = texturevector[i];

			for(unsigned int n = 0; n < texture_face.size(); n++) {
				const glm::vec2 & uv = texture_face[n];

				// the -uv.y+1.0 stuff is a hack for renderman input UV data
				// TODO: change renderman import UV data
				out_stream << "vt " << uv.x << " " << -uv.y << " 0.0" << endl;
			}
		}
	} else {
		log_error("Couldn't export texture coordinates! {} != {}",
		          facevector.size(), texturevector.size());
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

bool ObjExporter::exportFile(const string & objPath)
{
	mesh.facegroupCalcVertexes();

	FileWriter file_writer;

	file_writer.open(objPath);
	if(file_writer) {
		auto baseNameAbs = removeExtension(objPath);
		auto mtlPath = baseNameAbs + ".mtl";
		
		auto nameRel = pathBasename(objPath);
		auto mtlRel = pathBasename(mtlPath);
		
		std::ostringstream out_stream;
		createOBJStream(mesh, tm, out_stream, nameRel, mtlRel);
		file_writer << out_stream.str();
		file_writer.close();
		
		{
			auto mtlPath = baseNameAbs + ".mtl";
			FileWriter mtlWriter;
			mtlWriter.open(mtlPath);
			if(mtlWriter) {
				std::ostringstream out_stream;
				createMTLStream(out_stream, nameRel);
				file_writer << out_stream.str();
				file_writer.close();
			} else {
				log_error("Failed to write mtl file: {}", mtlPath);
			}
		}
	} else {
		log_error("Failed to open file for writing: {}", objPath);
	}

//	if(!file_writer)
//		return false;

//	file_writer.open(objPath + "materials.mtl");

//	if(file_writer) {
//		std::ostringstream out_stream;
//		createMTLStream(out_stream, full ? "fullmesh.obj" : "mesh.obj");

//		file_writer << out_stream.str();
//		file_writer.close();
//	}

//	if(!file_writer)
//		return false;

	return true;
}

void ObjExporter::createMTLStream(std::ostringstream & out_stream, const std::string & basename)
{
	const MaterialVector & materialvector = mesh.materials();

	out_stream << "# Material file for " << basename << endl << endl;

	for(unsigned int i = 0; i < materialvector.size(); i++) {
		const Material & material = materialvector[i];
		const Color &    colRGB   = material.getRGBCol();

		out_stream << "newmtl " << material.getName() << endl;
		out_stream << "Kd " << colRGB.red() << " " << colRGB.green() << " " << colRGB.blue()
		           << endl
		           << endl;
	}
}
