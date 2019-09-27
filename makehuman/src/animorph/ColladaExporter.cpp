#include "animorph/ColladaExporter.h"
#include "animorph/xmlParser.h"
using namespace std;
using namespace Animorph;

bool ColladaExporter::exportFile(const string &filename)
{

	mesh.loadSkeleton();

	string loc_filename = filename;
	string mesh_name("mesh.dae");

	string temp2;
	loc_filename += mesh_name;
	string temp = "mesh"; //.substr(index+1);

	XMLNode xMainNode;
	XMLNode xNode_collada, xNode_asset, xNode_contributor, xNode_author,
	    xNode_unitmeter, xNode_upaxis, xNode_library_geometries,
	    xNode_library_visual_scenes;
	XMLNode xNode_geometry, xNode_mesh, xNode_library_effects,
	    xNode_library_materials, xNode_library_controller;
	XMLNode xNode_p, xNode_input, xNode_source_position, xNode_float_array,
	    xNode_technique_common, xNode_accessor, xNode_param, xNode_source_uv,
	    xNode_source_normals, xNode_vertices, xNode_polygons;

	xMainNode = XMLNode::createXMLTopNode("xml", TRUE);
	xMainNode.addAttribute("version", "1.0");
	xMainNode.addAttribute("encoding", "utf-8");

	xNode_collada = xMainNode.addChild("COLLADA");
	xNode_collada.addAttribute("version", "1.4.0");
	xNode_collada.addAttribute("xmlns",
	                           "http://www.collada.org/2005/11/COLLADASchema");

	xNode_asset = xNode_collada.addChild("asset");
	xNode_contributor = xNode_asset.addChild("contributor");
	xNode_author = xNode_contributor.addChild("author");
	xNode_author.addText("MH");

	/*
	 xNode_unitmeter = xNode_asset.addChild("unit");
	 xNode_unitmeter.addAttribute("meter","0.01");
	 xNode_unitmeter.addAttribute("name","centimeter");

	 xNode_upaxis = xNode_asset .addChild("up_axiz");
	 xNode_upaxis.addText("Z_UP");
	*/
	// node library creation
	xNode_library_effects = xNode_collada.addChild("library_effects");
	xNode_library_geometries = xNode_collada.addChild("library_geometries");
	xNode_library_materials = xNode_collada.addChild("library_materials");
	xNode_library_visual_scenes = xNode_collada.addChild("library_visual_scenes");
	xNode_library_controller = xNode_collada.addChild("library_controllers");

	xNode_geometry = xNode_library_geometries.addChild("geometry");
	string tempname = temp;
	temp += "-Geometry";

	AddGeometry(&xNode_geometry, temp);
	AddController(&xNode_library_controller, temp);

	// THE SCENE
	MaterialVector &materialvector = mesh.getMaterialVectorRef();

	XMLNode xNode_visual_scene;
	XMLNode xNode_matrix, xNode_node, xNode_instance_geometry,
	    xNode_bind_material, xNode_instance_controller;
	XMLNode xNode_scene, xNode_instance_visual_scene, xNode_instance_material,
	    xNode_skeleton, xNode_node_controller;

	xNode_visual_scene = xNode_library_visual_scenes.addChild("visual_scene");
	xNode_visual_scene.addAttribute("id", "Scene");
	xNode_visual_scene.addAttribute("name", "Scene");

	createSkeleton(&xNode_visual_scene);

	// the node in the visual scene --> moved in the geometry controller
	/*
	  xNode_node_controller = xNode_visual_scene.addChild("node");
	  xNode_node_controller.addAttribute("id",(temp+"-contr").c_str());
	  xNode_node_controller.addAttribute("name",temp.c_str());
	  xNode_node_controller.addAttribute("type","NODE");
	  xNode_instance_controller =
	  xNode_node_controller.addChild("instance_controller");
	  xNode_instance_controller.addAttribute("url",("#"+ temp + "-skin").c_str());
	  xNode_skeleton = xNode_instance_controller.addChild("skeleton");
	  xNode_skeleton.addText("#joint0");
	*/
	//----------------------------------//

	xNode_node = xNode_visual_scene.addChild("node");
	xNode_node.addAttribute("id", (temp + "-scene").c_str());
	xNode_node.addAttribute("name", temp.c_str());

	xNode_matrix = xNode_node.addChild("matrix");
	xNode_matrix.addText("");

	std::ostringstream matrix_stream;

	matrix_stream << "1.0 0.0 0.0 0.0";
	xNode_matrix.addText(matrix_stream.str().c_str());

	matrix_stream.str("");
	matrix_stream << "0.0 1.0 0.0 0.0";
	xNode_matrix.addText(matrix_stream.str().c_str());

	matrix_stream.str("");
	matrix_stream << "0.0 0.0 1.0 0.0";
	xNode_matrix.addText(matrix_stream.str().c_str());

	matrix_stream.str("");
	matrix_stream << "0.0 0.0 0.0 1.0";
	xNode_matrix.addText(matrix_stream.str().c_str());

	// geometetry-->controller
	xNode_instance_geometry = xNode_node.addChild(
		"instance_controller"); // it is notr an error...before it was geometry
	xNode_instance_geometry.addAttribute("url", ("#" + temp + "-skin").c_str());

	xNode_bind_material = xNode_instance_geometry.addChild("bind_material");
	xNode_technique_common = xNode_bind_material.addChild("technique_common");

	for (unsigned int i = 0; i < materialvector.size(); i++) {
		xNode_instance_material =
		    xNode_technique_common.addChild("instance_material");
		xNode_instance_material.addAttribute("symbol",
		                                     materialvector[i].getName().c_str());
		xNode_instance_material.addAttribute(
		    "target", ("#" + materialvector[i].getName()).c_str());
	}

	xNode_scene = xNode_collada.addChild("scene");
	xNode_instance_visual_scene = xNode_scene.addChild("instance_visual_scene");

	xNode_instance_visual_scene.addAttribute("url", "#Scene");

	CreateLibraryMaterialsNode(&xNode_library_materials, &xNode_library_effects);

	XMLError error = xMainNode.writeToFile(loc_filename.c_str());

	if (error > 0) {
		return false;
	} else {
		return true;
	}
}

void ColladaExporter::AddGeometry(XMLNode *xNode_geometry, string temp)
{
	string temp2;
	XMLNode xNode_mesh, xNode_source_position, xNode_float_array,
	    xNode_technique_common, xNode_accessor, xNode_param;
	XMLNode xNode_source_normals, xNode_source_uv, xNode_vertices, xNode_input;

	VertexVector &vertexvector(mesh.getVertexVectorMorphOnlyRef());
	TextureVector &texturevector(mesh.getTextureVectorRef());
	FaceVector &facevector(mesh.getFaceVectorRef());
	MaterialVector &materialvector = mesh.getMaterialVectorRef();
	// number << "_" << index_material <<"_";

	// name += number;

	xNode_geometry->addAttribute("id", temp.c_str());
	xNode_geometry->addAttribute("name", temp.c_str());

	// position

	std::ostringstream vertex_stream;

	xNode_mesh = xNode_geometry->addChild("mesh");
	temp2 = temp + "-Position";
	xNode_source_position = xNode_mesh.addChild("source");
	xNode_source_position.addAttribute("id", temp2.c_str());
	xNode_float_array = xNode_source_position.addChild("float_array");

	std::ostringstream float_array_stream;
	float_array_stream << vertexvector.size() * 3;

	xNode_float_array.addAttribute("count", float_array_stream.str().c_str());
	xNode_float_array.addAttribute("id", (temp2 + "-array").c_str());

	for (unsigned int i = 0; i < vertexvector.size(); i++) {
		Vertex &vertex = vertexvector[i];

		glm::vec3 vector = vertex.co * tm;

		vertex_stream << vector.x << " " << vector.y << " " << vector.z << " ";
	}
	xNode_float_array.addText(vertex_stream.str().c_str());

	xNode_technique_common = xNode_source_position.addChild("technique_common");
	xNode_accessor = xNode_technique_common.addChild("accessor");

	std::ostringstream accessor_count_stream;
	accessor_count_stream << vertexvector.size();
	xNode_accessor.addAttribute("count", accessor_count_stream.str().c_str());
	xNode_accessor.addAttribute("source", ("#" + temp2 + "-array").c_str());
	xNode_accessor.addAttribute("stride", "3");
	xNode_param = xNode_accessor.addChild("param");
	xNode_param.addAttribute("name", "X");
	xNode_param.addAttribute("type", "float");

	xNode_param = xNode_accessor.addChild("param");
	xNode_param.addAttribute("name", "Y");
	xNode_param.addAttribute("type", "float");

	xNode_param = xNode_accessor.addChild("param");
	xNode_param.addAttribute("name", "Z");
	xNode_param.addAttribute("type", "float");

	// normals
	std::ostringstream normals_stream;
	temp2 = temp + "-Normals";
	xNode_source_normals = xNode_mesh.addChild("source");
	xNode_source_normals.addAttribute("id", temp2.c_str());
	// fa
	xNode_float_array = xNode_source_normals.addChild("float_array");

	std::ostringstream float_array_normals_stream;
	float_array_normals_stream << vertexvector.size() * 3;

	xNode_float_array.addAttribute("count",
	                               float_array_normals_stream.str().c_str());
	xNode_float_array.addAttribute("id", (temp2 + "-array").c_str());

	for (unsigned int i = 0; i < vertexvector.size(); i++) {
		Vertex &vertex = vertexvector[i];
		glm::vec3 vector = vertex.no * tm;

		normals_stream << vector.x << " " << vector.y << " " << vector.z << " ";
		normals_stream << vector.x << " " << vector.y << " " << vector.z << " ";
	}
	xNode_float_array.addText(normals_stream.str().c_str());

	xNode_technique_common = xNode_source_normals.addChild("technique_common");
	xNode_accessor = xNode_technique_common.addChild("accessor");

	xNode_accessor.addAttribute("count", accessor_count_stream.str().c_str());
	xNode_accessor.addAttribute("source", ("#" + temp2 + "-array").c_str());
	xNode_accessor.addAttribute("stride", "3");
	xNode_param = xNode_accessor.addChild("param");
	xNode_param.addAttribute("name", "X");
	xNode_param.addAttribute("type", "float");

	xNode_param = xNode_accessor.addChild("param");
	xNode_param.addAttribute("name", "Y");
	xNode_param.addAttribute("type", "float");

	xNode_param = xNode_accessor.addChild("param");
	xNode_param.addAttribute("name", "Z");
	xNode_param.addAttribute("type", "float");

	// uv
	temp2 = temp + "-UV";
	xNode_source_uv = xNode_mesh.addChild("source");
	xNode_source_uv.addAttribute("id", temp2.c_str());

	// fa
	xNode_float_array = xNode_source_uv.addChild("float_array");

	std::ostringstream float_array_uv_stream, float_array_counter_uv_stream,
	    float_accessor_counter_uv_stream;

	unsigned int counteruv = 0;

	for (unsigned int i = 0; i < facevector.size(); i++) {
		TextureFace &texture_face = texturevector[i];

		for (unsigned int n = 0; n < texture_face.size(); n++) {
			Vector2f &uv = texture_face[n];
			counteruv++;
			// the -uv.y+1.0 stuff is a hack for renderman input UV data
			// TODO: change renderman import UV data
			float_array_uv_stream << uv.x << " " << -uv.y << " ";
		}
	}

	float_array_counter_uv_stream << counteruv * 2;

	xNode_float_array.addAttribute("count",
	                               float_array_counter_uv_stream.str().c_str());
	xNode_float_array.addAttribute("id", (temp2 + "-array").c_str());

	xNode_float_array.addText(float_array_uv_stream.str().c_str());

	xNode_technique_common = xNode_source_uv.addChild("technique_common");
	xNode_accessor = xNode_technique_common.addChild("accessor");

	float_accessor_counter_uv_stream << counteruv;

	xNode_accessor.addAttribute("count",
	                            float_accessor_counter_uv_stream.str().c_str());
	xNode_accessor.addAttribute("source", ("#" + temp2 + "-array").c_str());
	xNode_accessor.addAttribute("stride", "2");
	xNode_param = xNode_accessor.addChild("param");
	xNode_param.addAttribute("name", "S");
	xNode_param.addAttribute("type", "float");

	xNode_param = xNode_accessor.addChild("param");
	xNode_param.addAttribute("name", "T");
	xNode_param.addAttribute("type", "float");

	// vertices
	xNode_vertices = xNode_mesh.addChild("vertices");
	xNode_vertices.addAttribute("id", (temp + "-Vertex").c_str());
	xNode_input = xNode_vertices.addChild("input");
	xNode_input.addAttribute("semantic", "POSITION");
	xNode_input.addAttribute("source", ("#" + temp + "-Position").c_str());
	/*
	 xNode_input = xNode_vertices.addChild("input");
	 xNode_input.addAttribute("semantic","NORMAL");
	 xNode_input.addAttribute("source",("#" +temp+"-Normals").c_str());
	*/

	// CreatePolygons(&xNode_mesh,temp);
	unsigned int texture_number = 0;

	// int mv_size = materialvector.size();
	for (unsigned int m = 0; m < materialvector.size(); m++) {
		bool found = false;
		for (unsigned int m1 = 0; m1 < m; m1++) {

			// already rendered this material???
			if (materialvector[m1].getName() == materialvector[m].getName()) {
				found = true;
				break;
			}
		}
		if (found == true)
			continue;
		// InsertASubmeshNode(&xNodeSubmeshes,NULL,m);
		CreatePolygons(&xNode_mesh, temp, m, texture_number);
	}
}

void ColladaExporter::CreatePolygons(XMLNode *xNode_mesh, string name,
                                     int material, unsigned int &texture_number)
{
	MaterialVector &materialvector = mesh.getMaterialVectorRef();
	// int number_p = facevector.size();
	FaceVector &facevector(mesh.getFaceVectorRef());
	XMLNode xNode_p, xNode_t, xNode_polygons, xNode_triangles, xNode_input;
	// register unsigned int i,j;

	int number_p = 0, number_t = 0;
	// int old_material_index = -1;

	unsigned int facevector_size = facevector.size();
	xNode_polygons = xNode_mesh->addChild("polygons");

	xNode_input = xNode_polygons.addChild("input");
	xNode_input.addAttribute("offset", "0");
	xNode_input.addAttribute("semantic", "VERTEX");
	xNode_input.addAttribute("source", ("#" + name + "-Vertex").c_str());

	xNode_input = xNode_polygons.addChild("input");
	xNode_input.addAttribute("offset", "1");
	xNode_input.addAttribute("semantic", "NORMAL");
	xNode_input.addAttribute("source", ("#" + name + "-Normals").c_str());

	xNode_input = xNode_polygons.addChild("input");
	xNode_input.addAttribute("offset", "2");
	xNode_input.addAttribute("semantic", "TEXCOORD");
	xNode_input.addAttribute("source", ("#" + name + "-UV").c_str());

	xNode_triangles = xNode_mesh->addChild("triangles");

	xNode_input = xNode_triangles.addChild("input");
	xNode_input.addAttribute("offset", "0");
	xNode_input.addAttribute("semantic", "VERTEX");
	xNode_input.addAttribute("source", ("#" + name + "-Vertex").c_str());

	xNode_input = xNode_triangles.addChild("input");
	xNode_input.addAttribute("offset", "1");
	xNode_input.addAttribute("semantic", "NORMAL");
	xNode_input.addAttribute("source", ("#" + name + "-Normals").c_str());

	xNode_input = xNode_triangles.addChild("input");
	xNode_input.addAttribute("offset", "2");
	xNode_input.addAttribute("semantic", "TEXCOORD");
	xNode_input.addAttribute("source", ("#" + name + "-UV").c_str());

	xNode_p = xNode_polygons.addChild("p");
	xNode_t = xNode_triangles.addChild("p");
	std::ostringstream polygon_out_stream;
	std::ostringstream triangle_out_stream;

	int texture_counter = 0;
	for (unsigned int i = 0; i < facevector_size; i++) {
		const Face &face(facevector[i]);

		int material_index = face.getMaterialIndex();
		string name = materialvector[material_index].getName();
		string name2 = materialvector[material].getName();

		if (name == name2) {

			unsigned int face_size = face.getSize();
			switch (face_size) {

			case 4:
				WriteTriangle(0, 1, 2, xNode_t, face, texture_counter,
				              triangle_out_stream);
				number_t++;

				//        xNode_p = xNode_triangles.addChild("p");
				WriteTriangle(0, 2, 3, xNode_t, face, texture_counter,
				              triangle_out_stream);
				number_t++;
				break;
			case 3:
				number_t++;
				WriteTriangle(0, 1, 2, xNode_t, face, texture_counter,
				              triangle_out_stream);
				break;
			default:
				xNode_p = xNode_triangles.addChild("p");
				number_t++;
				WriteTriangle(0, 1, 2, xNode_t, face, texture_counter,
				              triangle_out_stream);

				break;
			}
		}
		texture_counter += face.getSize();
	}
	if (number_p > 0) {

		std::ostringstream count_stream;
		count_stream << number_p;
		xNode_polygons.addAttribute("count", count_stream.str().c_str());
		xNode_polygons.addAttribute("material",
		                            materialvector[material].getName().c_str());

	} else {
		xNode_polygons.deleteNodeContent(0);
	}
	if (number_t > 0) {
		xNode_t.addText(triangle_out_stream.str().c_str());
		std::ostringstream count_stream;
		count_stream << number_t;
		xNode_triangles.addAttribute("count", count_stream.str().c_str());
		xNode_triangles.addAttribute("material",
		                             materialvector[material].getName().c_str());

	} else {
		xNode_triangles.deleteNodeContent(0);
	}
}

void ColladaExporter::WriteTriangle(int uno, int due, int tre, XMLNode &p,
                                    const Face face, int texture_counter,
                                    std::ostringstream &polygon_out_stream)

{
	int triangle[3];

	triangle[0] = uno;
	triangle[1] = due;
	triangle[2] = tre;

	for (int j = 0; j < 3; j++) {

		int vertex_number = face.getVertexAtIndex(triangle[j]);
		// cout << texture_number << endl;

		// face vertex geometry
		polygon_out_stream << vertex_number << " " << vertex_number << " "
		                   << texture_counter + triangle[j] << " ";
	}
}

void ColladaExporter::CreateLibraryMaterialsNode(
    XMLNode *xNode_library_materials, XMLNode *xNode_library_effects)
{
	MaterialVector &materialvector = mesh.getMaterialVectorRef();
	XMLNode xNode_diffuse, xNode_color, xNode_material, xNode_effect,
	    xNode_istance_effects, xNode_profile_common, xNode_technique, xNode_phong;

	for (unsigned int i = 0; i < materialvector.size(); i++) {
		std::ostringstream out_stream;
		Material &material = materialvector[i];
		Material material_2;
		const Color &colRGB = material.getRGBCol();
		bool found_duplicate = false;
		for (unsigned int j = 0; j < i; j++) {
			Material &material_2 = materialvector[j];
			if (material_2.getName() == material.getName())
				found_duplicate = true;
		}
		if (found_duplicate == true)
			continue;
		xNode_material = xNode_library_materials->addChild("material");

		xNode_material.addAttribute("id", material.getName().c_str());
		xNode_material.addAttribute("name", material.getName().c_str());
		xNode_istance_effects = xNode_material.addChild("instance_effect");
		xNode_istance_effects.addAttribute(
		    "url", ("#" + material.getName() + "-fx").c_str());

		xNode_effect = xNode_library_effects->addChild("effect");
		xNode_effect.addAttribute("id", (material.getName() + "-fx").c_str());
		xNode_effect.addAttribute("name", (material.getName() + "-fx").c_str());

		xNode_profile_common = xNode_effect.addChild("profile_COMMON");
		xNode_technique = xNode_profile_common.addChild("technique");
		xNode_technique.addAttribute("sid", "");
		xNode_phong = xNode_technique.addChild("phong");

		xNode_diffuse = xNode_phong.addChild("diffuse");
		xNode_color = xNode_diffuse.addChild("color");

		out_stream << colRGB.red() << " " << colRGB.green() << " " << colRGB.blue()
		           << " 1";
		xNode_color.addText(out_stream.str().c_str());
	}
}

void ColladaExporter::AddController(XMLNode *xNode_library_controller,
                                    string name)
{
	XMLNode xNode_controller, xNode_skin, xNode_jointsource, xNode_weightsource,
	    xNode_bind_poses;
	XMLNode xNode_IDREFArray, xNode_tecnique_common, xNode_accessor, xNode_param;
	XMLNode xNode_tecnique_common_2, xNode_accessor_2, xNode_param_2,
	    xNode_float_array, xNode_float_array_1;
	XMLNode xNode_vertex_weights, xNode_input1, xNode_input2, xNode_vcount,
	    xNode_v;
	XMLNode xNode_tecnique_common_3, xNode_accessor_3, xNode_param_3;
	XMLNode xNode_joints, xNode_inputA, xNode_inputB, xNode_bind_shape_matrix;

	VertexVector &vertexvector(mesh.getVertexVectorMorphOnlyRef());
	std::ostringstream out_stream;

	xNode_controller = xNode_library_controller->addChild("controller");
	xNode_controller.addAttribute("id", (name + "-skin").c_str());
	xNode_controller.addAttribute("name", name.c_str());

	xNode_skin = xNode_controller.addChild("skin");
	xNode_skin.addAttribute("source", ("#" + name).c_str());

	xNode_bind_shape_matrix = xNode_skin.addChild("bind_shape_matrix");
	out_stream.str("");
	out_stream << " 1.000000 "
	           << " 0.000000 "
	           << " 0.000000 "
	           << " 0.000000 ";
	xNode_bind_shape_matrix.addText(out_stream.str().c_str());

	out_stream.str("");
	out_stream << " 0.000000 "
	           << " 1.000000 "
	           << " 0.000000 "
	           << " 0.000000 ";
	xNode_bind_shape_matrix.addText(out_stream.str().c_str());

	out_stream.str("");
	out_stream << " 0.000000 "
	           << " 0.000000 "
	           << " 1.000000 "
	           << " 0.000000 ";
	xNode_bind_shape_matrix.addText(out_stream.str().c_str());

	out_stream.str("");
	out_stream << " 0.000000 "
	           << " 0.000000 "
	           << " 0.000000 "
	           << " 1.000000 ";
	xNode_bind_shape_matrix.addText(out_stream.str().c_str());

	out_stream.str("");

	xNode_jointsource = xNode_skin.addChild("source");
	xNode_jointsource.addAttribute("id", (name + "-skin" + "-joints").c_str());

	xNode_IDREFArray = xNode_jointsource.addChild("IDREF_array");
	xNode_IDREFArray.addAttribute(
	    "id", (name + "-skin" + "-joints" + "-array").c_str());

	int jointcounter = 0;
	// calculate the joints that enter in this array
	out_stream.str("");
	for (int i = SK_JOINT_0; i < SK_JOINT_END; i++) {
		//   if(subjoint[i][0] == SK_NONE)
		//     continue;

		if (CheckIfJointIsLinked((SKELETON_JOINT)i) == true) {
			jointcounter++;
			out_stream << "joint" << i << " ";
		}
	}

	xNode_IDREFArray.addText(out_stream.str().c_str());

	out_stream.str("");
	out_stream << jointcounter;
	xNode_IDREFArray.addAttribute("count", out_stream.str().c_str());

	xNode_tecnique_common = xNode_jointsource.addChild("technique_common");
	xNode_accessor = xNode_tecnique_common.addChild("accessor");

	out_stream.str("");
	out_stream << jointcounter;

	xNode_accessor.addAttribute("source", (name + "-skin-joints-array").c_str());
	xNode_accessor.addAttribute("count", out_stream.str().c_str());
	xNode_accessor.addAttribute("stride", "1");

	xNode_param = xNode_accessor.addChild("param");

	xNode_param.addAttribute("name", "JOINT");
	xNode_param.addAttribute("type", "IDREF");

	// SOURCES BIND-POSES
	xNode_bind_poses = xNode_skin.addChild("source");
	xNode_bind_poses.addAttribute("id", (name + "-skin-bind_poses").c_str());
	xNode_float_array_1 = xNode_bind_poses.addChild("float_array");
	xNode_float_array_1.addAttribute("id",
	                                 (name + "-skin-bind_poses-array").c_str());

	out_stream.str("");
	out_stream << jointcounter * 16;
	xNode_float_array_1.addAttribute("count", out_stream.str().c_str());

	loadBindPoses("../data/binding.matrix", &xNode_float_array_1, jointcounter);

	xNode_tecnique_common_3 = xNode_bind_poses.addChild("technique_common");
	xNode_accessor_3 = xNode_tecnique_common_3.addChild("accessor");
	xNode_accessor_3.addAttribute(
	    "source", ("#" + name + "-skin-bind_poses-array").c_str());

	out_stream.str("");
	out_stream << jointcounter;
	xNode_accessor_3.addAttribute("count", out_stream.str().c_str());

	xNode_accessor_3.addAttribute("stride", "16");
	xNode_param_3 = xNode_accessor_3.addChild("param");
	xNode_param_3.addAttribute("type", "float4x4");

	// SOURCES WEIGHTS

	xNode_weightsource = xNode_skin.addChild("source");
	xNode_weightsource.addAttribute("id", (name + "-skin-weights").c_str());
	xNode_float_array = xNode_weightsource.addChild("float_array");
	xNode_float_array.addAttribute("id", (name + "-skin-weights-array").c_str());
	xNode_float_array.addAttribute("count", "10822");
	loadWeightsVector("../data/weights.vector", &xNode_float_array);

	xNode_tecnique_common_2 = xNode_weightsource.addChild("technique_common");
	xNode_accessor_2 = xNode_tecnique_common_2.addChild("accessor");
	xNode_param_2 = xNode_accessor_2.addChild("param");

	xNode_accessor_2.addAttribute("source",
	                              ("#" + name + "-skin-weights-array").c_str());
	xNode_accessor_2.addAttribute("count", "10822");
	xNode_accessor_2.addAttribute("stride", "1");

	xNode_param_2.addAttribute("name", "WEIGHT");
	xNode_param_2.addAttribute("type", "float");

	// JOINTS
	xNode_joints = xNode_skin.addChild("joints");
	xNode_inputA = xNode_joints.addChild("input");
	xNode_inputB = xNode_joints.addChild("input");

	xNode_inputA.addAttribute("semantic", "JOINT");
	xNode_inputA.addAttribute("source", ("#" + name + "-skin-joints").c_str());

	xNode_inputB.addAttribute("semantic", "INV_BIND_MATRIX");
	xNode_inputB.addAttribute("source",
	                          ("#" + name + "-skin-bind_poses").c_str());
	// VERTEX WEIGHTS
	xNode_vertex_weights = xNode_skin.addChild("vertex_weights");

	out_stream.str("");
	out_stream << vertexvector.size();
	xNode_vertex_weights.addAttribute("count", out_stream.str().c_str());
	xNode_input1 = xNode_vertex_weights.addChild("input");
	xNode_input1.addAttribute("semantic", "JOINT");
	xNode_input1.addAttribute("source",
	                          ("#" + name + "-skin" + "-joints").c_str());
	xNode_input1.addAttribute("offset", "0");

	xNode_input2 = xNode_vertex_weights.addChild("input");
	xNode_input2.addAttribute("semantic", "WEIGHT");
	xNode_input2.addAttribute("source",
	                          ("#" + name + "-skin" + "-weights").c_str());
	xNode_input2.addAttribute("offset", "1");

	xNode_vcount = xNode_vertex_weights.addChild("vcount");
	out_stream.str("");
	for (unsigned int i = 0; i < vertexvector.size(); i++) {
		out_stream << jointcounter << " ";
	}
	xNode_vcount.addText(out_stream.str().c_str());

	xNode_v = xNode_vertex_weights.addChild("v");

	loadVertexWeights("../data/weights.matrix", &xNode_v, jointcounter);
}
bool ColladaExporter::CheckIfJointIsLinked(SKELETON_JOINT joint)
{
	// check if someone point to this
	for (int k = SK_JOINT_0; k < SK_JOINT_END; k++) {

		for (int sub = 0; sub < MAX_NUMBER_SUBJOINT; sub++) {

			if (subjoint[k][sub] == joint) {

				return true;
			}
		}
	}
	return false;
}
void ColladaExporter::createSkeleton(XMLNode *mainNode)
{
	// subjoint map:
	XMLNode xNode_joint, xNode_translate;
	xNode_joint = mainNode->addChild("node");

	std::ostringstream out_stream;
	std::ostringstream out_stream_translate;

	out_stream << "joint" << SK_JOINT_0;

	xNode_joint.addAttribute("name", out_stream.str().c_str());
	xNode_joint.addAttribute("id", out_stream.str().c_str());
	xNode_joint.addAttribute("sid", out_stream.str().c_str());
	xNode_joint.addAttribute("type", "JOINT");

	xNode_translate = xNode_joint.addChild("translate");
	xNode_translate.addAttribute("sid", "translate");

	out_stream_translate << mesh.GetJoint0_Pos().x << " "
	                     << mesh.GetJoint0_Pos().y << " "
	                     << mesh.GetJoint0_Pos().z;
	xNode_translate.addText(out_stream_translate.str().c_str());

	recursiveJointAdd(SK_JOINT_0, &xNode_joint);
}

void ColladaExporter::recursiveJointAdd(int row, XMLNode *xNode_Parent)
{
	int column = 0;

	while (column < 6 && subjoint[row][column] != SK_NONE) {

		int i = subjoint[row][column];
		SKELETON_JOINT numb = subjoint[i][0];

		XMLNode xNode = xNode_Parent->addChild("node");
		setChildNode(&xNode, (SKELETON_JOINT)i, row, column);

		if (numb != SK_NONE) {

			recursiveJointAdd(i, &xNode);
		}

		column++;
	}
}

void ColladaExporter::setChildNode(XMLNode *child, SKELETON_JOINT numb,
                                   unsigned int row, unsigned int column)
{
	if (numb < 0)
		return;

	XMLNode xNode_translate;
	// SKELETON_JOINT parent = (SKELETON_JOINT)row;
	std::ostringstream out_stream;
	std::ostringstream out_stream_translate;
	out_stream << "joint" << subjoint[row][column];

	child->addAttribute("name", out_stream.str().c_str());
	child->addAttribute("id", out_stream.str().c_str());
	child->addAttribute("sid", out_stream.str().c_str());
	child->addAttribute("type", "JOINT");

	xNode_translate = child->addChild("translate");
	xNode_translate.addAttribute("sid", "translate");

	glm::vec3 parentnode = mesh.getJointVector()->at(row);
	glm::vec3 childnode = mesh.getJointVector()->at(numb);

	glm::vec3 diff;
	if (childnode == glm::vec3(0, 0, 0))
		diff = glm::vec3(0, 0, 0);
	else
		diff = childnode - parentnode;

	out_stream_translate << diff.x << " " << diff.y << " " << diff.z;
	xNode_translate.addText(out_stream_translate.str().c_str());
}

void ColladaExporter::loadBindPoses(const std::string &filename,
                                    XMLNode *xNode_binding, int jointCounter)
{
	FileReader file_reader;
	std::ostringstream out_stream;
	file_reader.open(filename);
	int line = 0;
	float a, b, c, d;
	if (!file_reader)
		return;

	char buffer[MAX_LINE_BUFFER];
	while (file_reader.getline(buffer, MAX_LINE_BUFFER)) {
		if (sscanf(buffer, "%f %f %f %f", &a, &b, &c, &d) == 4) { // ok
			out_stream.str("");
			out_stream << buffer;
			xNode_binding->addText(out_stream.str().c_str());
			line++;
		} else {
			// ng
		}
		if (line == jointCounter * 4)
			break;
	}
	while (line < (jointCounter * 4)) {
		// fill with 0
		out_stream.str("");
		out_stream << " 1.000000 "
		           << " 0.000000 "
		           << " 0.000000 "
		           << " 0.000000 ";
		xNode_binding->addText(out_stream.str().c_str());

		out_stream.str("");
		out_stream << " 0.000000 "
		           << " 1.000000 "
		           << " 0.000000 "
		           << " 0.000000 ";
		xNode_binding->addText(out_stream.str().c_str());

		out_stream.str("");
		out_stream << " 0.000000 "
		           << " 0.000000 "
		           << " 1.000000 "
		           << " 0.000000 ";
		xNode_binding->addText(out_stream.str().c_str());

		out_stream.str("");
		out_stream << " 0.000000 "
		           << " 0.000000 "
		           << " 0.000000 "
		           << " 1.000000 ";
		xNode_binding->addText(out_stream.str().c_str());
		line += 4;
	}
}

void ColladaExporter::loadVertexWeights(const std::string &filename,
                                        XMLNode *xNode_binding,
                                        int jointcounter)
{
	VertexVector &vertexvector(mesh.getVertexVectorMorphOnlyRef());

	FileReader file_reader;
	std::ostringstream out_stream;
	file_reader.open(filename);
	unsigned int line = 0;

	if (!file_reader)
		return;

	char buffer[MAX_LINE_BUFFER * 4];
	while (file_reader.getline(buffer, MAX_LINE_BUFFER * 4)) {
		out_stream.str("");
		out_stream << buffer;

		xNode_binding->addText(out_stream.str().c_str());
		line++;
	}

	if (line != vertexvector.size()) {
		line++;
	}
}
void ColladaExporter::loadWeightsVector(const std::string &filename,
                                        XMLNode *xNode_weights)
{
	FileReader file_reader;
	std::ostringstream out_stream;
	file_reader.open(filename);

	if (!file_reader)
		return;

	out_stream.str("");
	char data;

	while (file_reader.get(data)) {
		out_stream << data;
	}

	xNode_weights->addText(out_stream.str().c_str());
}
