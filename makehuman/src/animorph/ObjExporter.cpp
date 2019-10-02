#include "animorph/ObjExporter.h"

#include <fmt/format.h>

#include "log/log.h"
#include "util/StringUtils.h"
#include "Vfs.h"

using namespace std;
using namespace Animorph;


struct ObjStream {
	fmt::MemoryWriter m_out;
	
	void writeComment(const std::string & comment)
	{
		m_out.write("# {}\n", comment);
	}
	void writeMaterialLib(const std::string & libFilePath)
	{
		m_out.write("mtllib {}\n", libFilePath);
	}
	void writeObjectStart(const std::string & objectName)
	{
		m_out.write("o {}\n", objectName);
	}
	void writeVertex(const glm::vec3 & v)
	{
		m_out.write("v {} {} {}\n", v.x, v.y, v.z);
	}
	void writeUv(const glm::vec2 & uv)
	{
		m_out.write("vt {} {} 0.0\n", uv.x, uv.y);
	}
	void writeGroupStart(const std::string & groupName)
	{
		m_out.write("g {}\n", groupName);
	}
	void writeSmoothingGroup(const int groupId)
	{
		m_out.write("s {}\n", groupId);
	}
	void writeUseMaterial(const std::string & materialName)
	{
		m_out.write("usemtl {}\n", materialName);
	}
	template <typename... T>
	void writeRaw(T &&... p)
	{
		m_out.write(std::forward<T>(p)...);
	}
};


static void calcVertexes(Mesh & mesh, const FaceVector & facevector,
                         std::map<std::string, VertexData> & vertexes)
{
	
	
	int vertexCounter;
	
	for(const auto & [partname, groupValue] : mesh.facegroup().m_groups) {
		
		const FGroupData & groupdata = groupValue.facesIndexes;
		vertexCounter          = 0;
		
		for(unsigned int i = 0; i < groupdata.size(); i++) {
			const Face & face(facevector[groupdata[i]]);
			
			for(unsigned int j = 0; j < face.getSize(); j++) {
				const int tmp = face.getVertexAtIndex(j);
				
				if(vertexes[partname].find(tmp) == vertexes[partname].end()) {
					vertexes[partname][tmp] = 0;
				}
			}
		}
		
		for(VertexData::iterator vertexgroup_it = vertexes[partname].begin();
		     vertexgroup_it != vertexes[partname].end(); vertexgroup_it++) {
			vertexes[partname][(*vertexgroup_it).first] = vertexCounter++;
		}
	}
	
	// cout << vertexes.size() << endl;
}


static void createOBJStream(Mesh & mesh,
                            const Matrix & tm,
                     ObjStream & obj,
                     const string & objRelPath,
                     const string & mtlRelPath)
{
	
	/// Maps FaceGroup identifiers via vertex group numbers to vertex indices
	std::map<std::string, VertexData> vertexes;
	calcVertexes(mesh, mesh.faces(), vertexes);
		
	const FaceGroup & facegroup(mesh.facegroup());

	const VertexVector &   vertexvector(mesh.getVertexVectorRef());
	const FaceVector &     facevector(mesh.faces());
	const TextureVector &  texturevector(mesh.textureVector());
	const MaterialVector & materialvector = mesh.materials();

	// write header
	obj.writeComment("OBJ File");
	obj.writeMaterialLib(mtlRelPath);
	obj.writeObjectStart(objRelPath);
	
	for(auto & [partname, groupValue] : facegroup.m_groups) {

		const VertexData & vertexgroupdata(vertexes[partname]);

		for(VertexData::const_iterator vertexgroup_it = vertexgroupdata.begin();
		    vertexgroup_it != vertexgroupdata.end(); vertexgroup_it++) {
			const Vertex & vertex(vertexvector[(*vertexgroup_it).first]);
			glm::vec3      v(vertex.pos * tm);
			
			obj.writeVertex(v);
			
			// TODO broken ?
			//glm::vec3 n = vertex.no * tm;
			//out_stream << "vn " << n.x << " " << n.y << " " << n.z << endl;
		}
	}

	for(auto & [partname, groupValue] : facegroup.m_groups) {

		const FGroupData & facegroupdata(groupValue.facesIndexes);
		// VertexData vertexgroupdata = vertexes[partname];

		// write texture UV coordinates
		if(facevector.size() == texturevector.size()) {
			for(unsigned int i = 0; i < facegroupdata.size(); i++) {
				const TextureFace & texture_face(texturevector[facegroupdata[i]]);

				for(unsigned int n = 0; n < texture_face.size(); n++) {
					glm::vec2 uv = texture_face[n];
					uv.y = (1.f - uv.y);// + 1.f;
					//uv = glm::vec2(0, 1) - uv;
					
					obj.writeUv(uv);
				}
			}
		} else {
			log_error("Couldn't export texture coordinates! {} != {}",
			          facevector.size(), texturevector.size());
		}
	}

	int v_offset  = 0;
	int vt_offset = 0;
	
	int smoothGroup = 1;
	for(const auto & [partname, groupValue] : facegroup.m_groups) {
		
		// Group name
		obj.writeGroupStart(partname);
		// Smoothing group
		obj.writeSmoothingGroup(smoothGroup);
		smoothGroup++;
		
		const FGroupData & facegroupdata(groupValue.facesIndexes);
		const VertexData & vertexgroupdata(vertexes[partname]);

		// write faces
		int old_material_index = -1;
		int texture_number     = 1;
		for(unsigned int i = 0; i < facegroupdata.size(); i++) {
			const Face & face(facevector[facegroupdata[i]]);

			int matIdx = face.getMaterialIndex();

			if((matIdx != -1) && (matIdx != old_material_index)) {
				// material reference
				obj.writeUseMaterial(materialvector[matIdx].name);
			}
			
			if(face.size == 3) {
				obj.writeRaw("f ");
			} else if(face.size == 4) {
				obj.writeRaw("f ");
			}
			
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
					obj.writeRaw("{}/{} ",
					             vertex_number + 1 + v_offset,
					             texture_number + vt_offset);
					
					texture_number++;
				}
			}
			obj.writeRaw("\n");

			old_material_index = matIdx;
		}
		v_offset += vertexgroupdata.size();

		for(unsigned int i = 0; i < facegroupdata.size(); i++) {
			const TextureFace & texture_face(texturevector[facegroupdata[i]]);
			vt_offset += texture_face.size();
		}
	}
}

static void writeMtl(const Mesh & mesh, const string & mtlPath, const string & objName)
{
	FileWriter mtlWriter;
	mtlWriter.open(mtlPath);
	if(!mtlWriter) {
		log_error("Failed to write mtl file {}", mtlPath);
		return;
	}
	
	std::ostringstream stream;
	stream << "# Material file for " << objName << endl << endl;
	
	for(const auto & mat : mesh.materials()) {
		const Color &    col   = mat.color;
		
		stream << "newmtl " << mat.name << endl;
		//stream << "illum " << 2 << endl;
		// Diffuse color
		stream << "kd " << col.r << " " << col.g << " " << col.b << endl;
		
		{
			// Diffuse Texture
			std::string inTexPath = "pixmaps/ui/" + mat.name + "_color.png";
			if(vfs::exists(inTexPath)) {
				std::string foo = removeExtension(objName) + "_"+ mat.name + "_color.png";
				stream << "map_kd " << foo << endl;
				
				std::string outTexPath = removeExtension(mtlPath) + "_"+ mat.name + "_color.png";
				vfs::copyToFilesystem(inTexPath, outTexPath);
			}
		}
		stream << endl;
	}
	
	mtlWriter << stream.str();
	mtlWriter.close();
}

bool ObjExporter::exportFile(const string & objPath)
{
	auto baseNameAbs = removeExtension(objPath);
	auto mtlPath = baseNameAbs + ".mtl";
	
	auto nameRel = pathBasename(objPath);
	auto mtlRel = pathBasename(mtlPath);
	
	FileWriter file_writer;
	file_writer.open(objPath);
	if(!file_writer) {
		log_error("Failed to write obj: {}", objPath);
		return false;
	}
	
	ObjStream objStream;
	createOBJStream(mesh, tm, objStream, nameRel, mtlRel);
	file_writer.write(objStream.m_out.data(), objStream.m_out.size());
	file_writer.close();
	
	writeMtl(mesh, mtlPath, nameRel);

	return true;
}


