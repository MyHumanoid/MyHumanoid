#include "io/ObjExporter.h"

#include <fmt/format.h>

#include "Logger.h"
#include "StringUtils.h"
#include "Vfs.h"

namespace Animorph
{

struct ObjStream {
	fmt::memory_buffer m_out;
	
	template <typename... Args>
	void write(fmt::format_string<Args...> format, Args &&... args)
	{
		fmt::format_to(std::back_inserter(m_out), format, std::forward<Args>(args)...);
	}
	
	void comment(const std::string & comment)
	{
		write("# {}\n", comment);
	}
	void materialLib(const std::string & libFilePath)
	{
		write("mtllib {}\n", libFilePath);
	}
	void startObject(const std::string & name)
	{
		write("o {}\n", name);
	}
	void vertex(const glm::vec3 & v)
	{
		write("v {} {} {}\n", v.x, v.y, v.z);
	}
	void uv(const glm::vec2 & uv)
	{
		write("vt {} {} 0.0\n", uv.x, uv.y);
	}
	void startFaceGroup(const std::string & name)
	{
		write("g {}\n", name);
	}
	void writeSmoothingGroup(const int groupId)
	{
		write("s {}\n", groupId);
	}
	void startUseMaterial(const std::string & materialName)
	{
		write("usemtl {}\n", materialName);
	}
};


static void calcVertexes(Mesh & mesh, const FaceVector & facevector,
                         std::map<std::string, VertexData> & vertexes)
{
	int vertexCounter;
	for(const auto & [partname, groupValue] : mesh.facegroup().m_groups) {
		
		vertexCounter          = 0;
		
		for(const auto & faceIdx : groupValue.facesIndexes) {
			const Face & face = facevector[faceIdx];
			
			for(unsigned int j = 0; j < face.getSize(); j++) {
				const int tmp = face.getVertexAtIndex(j);
				
				if(vertexes[partname].find(tmp) == vertexes[partname].end()) {
					vertexes[partname][tmp] = 0;
				}
			}
		}
		
		for(auto & [key, val] : vertexes[partname]) {
			vertexes[partname][key] = vertexCounter++;
		}
	}
}


static void createOBJStream(Mesh & mesh,
                            const Matrix & tm,
                     ObjStream & obj,
                     const string & objRelPath,
                     const string & mtlRelPath)
{
	
	/// Maps FaceGroup identifiers via vertex group numbers to vertex indices
	std::map<std::string, VertexData> grpToVertIdxs;
	calcVertexes(mesh, mesh.faces(), grpToVertIdxs);
		
	const FaceGroup & facegroup(mesh.facegroup());

	const VertexVector &   vertexvector(mesh.getVertexVectorRef());
	const TextureVector &  texturevector(mesh.textureVector());

	// write header
	obj.comment("OBJ File");
	obj.materialLib(mtlRelPath);
	obj.startObject(objRelPath);
	
	for(auto & [partname, groupValue] : facegroup.m_groups) {

		const VertexData & vertexgroupdata(grpToVertIdxs[partname]);

		for(VertexData::const_iterator vertexgroup_it = vertexgroupdata.begin();
		    vertexgroup_it != vertexgroupdata.end(); vertexgroup_it++) {
			const Vertex & vertex(vertexvector.m_verts[(*vertexgroup_it).first]);
			glm::vec3      v(vertex.pos * tm);
			
			obj.vertex(v);
			
			// TODO broken ?
			//glm::vec3 n = vertex.no * tm;
			//out_stream << "vn " << n.x << " " << n.y << " " << n.z << endl;
		}
	}

	for(auto & [partname, groupValue] : facegroup.m_groups) {

		const FGroupData & facegroupdata(groupValue.facesIndexes);
		// VertexData vertexgroupdata = vertexes[partname];

		// write texture UV coordinates
		if(mesh.faces().size() == texturevector.size()) {
			for(unsigned int i = 0; i < facegroupdata.size(); i++) {
				const TextureFace & texture_face(texturevector[facegroupdata[i]]);

				for(unsigned int n = 0; n < texture_face.size(); n++) {
					glm::vec2 uv = texture_face[n];
					uv.y = (1.f - uv.y);// + 1.f;
					//uv = glm::vec2(0, 1) - uv;
					
					obj.uv(uv);
				}
			}
		} else {
			log_error("Couldn't export texture coordinates! {} != {}",
			          mesh.faces().size(), texturevector.size());
		}
	}

	int v_offset  = 0;
	int vt_offset = 0;
	
	int smoothGroup = 1;
	for(const auto & [partname, groupValue] : facegroup.m_groups) {
		
		// Group name
		obj.startFaceGroup(partname);
		// Smoothing group
		obj.writeSmoothingGroup(smoothGroup);
		smoothGroup++;
		
		const FGroupData & facegroupdata(groupValue.facesIndexes);
		const VertexData & vertexgroupdata(grpToVertIdxs[partname]);

		// write faces
		int old_material_index = -1;
		int texture_number     = 1;
		for(unsigned int i = 0; i < facegroupdata.size(); i++) {
			const Face & face = mesh.faces()[facegroupdata[i]];

			int matIdx = face.getMaterialIndex();

			if((matIdx != -1) && (matIdx != old_material_index)) {
				// material reference
				obj.startUseMaterial(mesh.materials()[matIdx].name);
			}
			
			if(face.size == 3) {
				obj.write("f ");
			} else if(face.size == 4) {
				obj.write("f ");
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
					obj.write("{}/{} ",
					             vertex_number + 1 + v_offset,
					             texture_number + vt_offset);
					
					texture_number++;
				}
			}
			obj.write("\n");

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
	stream << "# Material file for " << objName << std::endl << std::endl;
	
	for(const auto & mat : mesh.materials()) {
		const Color &    col   = mat.color;
		
		stream << "newmtl " << mat.name << std::endl;
		//stream << "illum " << 2 << endl;
		// Diffuse color
		stream << "Kd " << col.m_color.r << " " << col.m_color.g << " " << col.m_color.b << std::endl;
		
		{
			// Diffuse Texture
			std::string inTexPath = "data/pixmaps/ui/" + mat.name + "_color.png";
			if(vfs::exists(inTexPath)) {
				std::string foo = removeExtension(objName) + "_"+ mat.name + "_color.png";
				stream << "map_Kd " << foo << std::endl;
				
				std::string outTexPath = removeExtension(mtlPath) + "_"+ mat.name + "_color.png";
				vfs::copyToFilesystem(inTexPath, outTexPath);
			}
		}
		stream << std::endl;
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

}
