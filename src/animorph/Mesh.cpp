#include "animorph/Mesh.h"

#include <assert.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Logger.h"
#include "Profiler.h"
#include "Vfs.h"

#include <filesystem>
#include "CompositeMorph.h"

namespace Animorph
{

const SKELETON_JOINT subjoint[][MAX_NUMBER_SUBJOINT] = {
        /*SK_JOINT_0*/ {SK_JOINT_1, SK_JOINT_8, SK_JOINT_9, SK_NONE, SK_NONE, SK_NONE},
        /*SK_JOINT_1*/
        {SK_JOINT_2, SK_JOINT_4, SK_JOINT_5, SK_NONE, SK_NONE, SK_NONE},
        /*SK_JOINT_2*/ {SK_JOINT_3, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},
        /*SK_JOINT_3*/ {SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},
        /*SK_JOINT_4*/ {SK_JOINT_6, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},
        /*SK_JOINT_5*/ {SK_JOINT_7, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},
        /*SK_JOINT_6*/ {SK_JOINT_10, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},
        /*SK_JOINT_7*/ {SK_JOINT_11, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},
        /*SK_JOINT_8*/ {SK_JOINT_12, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},
        /*SK_JOINT_9*/ {SK_JOINT_13, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},

        /*SK_JOINT_10*/ {SK_JOINT_14, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},
        /*SK_JOINT_11*/ {SK_JOINT_30, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},
        /*SK_JOINT_12*/ {SK_JOINT_46, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},
        /*SK_JOINT_13*/ {SK_JOINT_57, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},

        // left hand
        /*SK_JOINT_14*/
        {SK_JOINT_15, SK_JOINT_18, SK_JOINT_21, SK_JOINT_24, SK_JOINT_27, SK_NONE},
        /*SK_JOINT_15*/ {SK_JOINT_16, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},
        /*SK_JOINT_16*/ {SK_JOINT_17, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},
        /*SK_JOINT_17*/ {SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},
        /*SK_JOINT_18*/ {SK_JOINT_19, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},
        /*SK_JOINT_19*/ {SK_JOINT_20, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},
        /*SK_JOINT_20*/ {SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},
        /*SK_JOINT_21*/ {SK_JOINT_22, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},
        /*SK_JOINT_22*/ {SK_JOINT_23, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},
        /*SK_JOINT_23*/ {SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},
        /*SK_JOINT_24*/ {SK_JOINT_25, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},
        /*SK_JOINT_25*/ {SK_JOINT_26, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},
        /*SK_JOINT_26*/ {SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},
        /*SK_JOINT_27*/ {SK_JOINT_28, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},
        /*SK_JOINT_28*/ {SK_JOINT_29, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},
        /*SK_JOINT_29*/ {SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},

        // right hand
        /*SK_JOINT_30*/
        {SK_JOINT_31, SK_JOINT_34, SK_JOINT_37, SK_JOINT_40, SK_JOINT_43, SK_NONE},
        /*SK_JOINT_31*/ {SK_JOINT_32, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},
        /*SK_JOINT_32*/ {SK_JOINT_33, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},
        /*SK_JOINT_33*/ {SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},
        /*SK_JOINT_34*/ {SK_JOINT_35, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},
        /*SK_JOINT_35*/ {SK_JOINT_36, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},
        /*SK_JOINT_36*/ {SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},
        /*SK_JOINT_37*/ {SK_JOINT_38, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},
        /*SK_JOINT_38*/ {SK_JOINT_39, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},
        /*SK_JOINT_39*/ {SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},
        /*SK_JOINT_40*/ {SK_JOINT_41, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},
        /*SK_JOINT_41*/ {SK_JOINT_42, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},
        /*SK_JOINT_42*/ {SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},
        /*SK_JOINT_43*/ {SK_JOINT_44, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},
        /*SK_JOINT_44*/ {SK_JOINT_45, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},
        /*SK_JOINT_45*/ {SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},

        // left foot
        /*SK_JOINT_46*/
        {SK_JOINT_47, SK_JOINT_49, SK_JOINT_51, SK_JOINT_53, SK_JOINT_55, SK_NONE},
        /*SK_JOINT_47*/ {SK_JOINT_48, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},
        /*SK_JOINT_48*/ {SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},
        /*SK_JOINT_49*/ {SK_JOINT_50, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},
        /*SK_JOINT_50*/ {SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},
        /*SK_JOINT_51*/ {SK_JOINT_52, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},
        /*SK_JOINT_52*/ {SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},
        /*SK_JOINT_53*/ {SK_JOINT_54, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},
        /*SK_JOINT_54*/ {SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},
        /*SK_JOINT_55*/ {SK_JOINT_56, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},
        /*SK_JOINT_56*/ {SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},

        // right foot
        /*SK_JOINT_57*/
        {SK_JOINT_58, SK_JOINT_60, SK_JOINT_62, SK_JOINT_64, SK_JOINT_66, SK_NONE},
        /*SK_JOINT_58*/ {SK_JOINT_59, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},
        /*SK_JOINT_59*/ {SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},
        /*SK_JOINT_60*/ {SK_JOINT_61, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},
        /*SK_JOINT_61*/ {SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},
        /*SK_JOINT_62*/ {SK_JOINT_63, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},
        /*SK_JOINT_63*/ {SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},
        /*SK_JOINT_64*/ {SK_JOINT_65, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},
        /*SK_JOINT_65*/ {SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},
        /*SK_JOINT_66*/ {SK_JOINT_67, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},
        /*SK_JOINT_67*/ {SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE, SK_NONE},
};
const SKELETON_JOINT jointSymmetric[SK_JOINT_END] = {
        SK_JOINT_0,
        SK_JOINT_1,  // SK_JOINT_1
        SK_JOINT_2,  // SK_JOINT_2
        SK_JOINT_3,  // SK_JOINT_3
        SK_JOINT_5,  // SK_JOINT_4
        SK_JOINT_4,  // SK_JOINT_5
        SK_JOINT_7,  // SK_JOINT_6
        SK_JOINT_6,  // SK_JOINT_7
        SK_JOINT_9,  // SK_JOINT_8
        SK_JOINT_8,  // SK_JOINT_9
        SK_JOINT_11, // SK_JOINT_10
        SK_JOINT_10, // SK_JOINT_11
        SK_JOINT_13, // SK_JOINT_12
        SK_JOINT_12, // SK_JOINT_13

        SK_JOINT_30, // SK_JOINT_14
        SK_JOINT_31, // SK_JOINT_15
        SK_JOINT_32, // SK_JOINT_16
        SK_JOINT_33, // SK_JOINT_17
        SK_JOINT_34, // SK_JOINT_18
        SK_JOINT_35, // SK_JOINT_19
        SK_JOINT_36, // SK_JOINT_20
        SK_JOINT_37, // SK_JOINT_21
        SK_JOINT_38, // SK_JOINT_22
        SK_JOINT_39, // SK_JOINT_23
        SK_JOINT_40, // SK_JOINT_24
        SK_JOINT_41, // SK_JOINT_25
        SK_JOINT_42, // SK_JOINT_26
        SK_JOINT_43, // SK_JOINT_27
        SK_JOINT_44, // SK_JOINT_28
        SK_JOINT_45, // SK_JOINT_29

        SK_JOINT_14, // SK_JOINT_30
        SK_JOINT_15, // SK_JOINT_31
        SK_JOINT_16, // SK_JOINT_32
        SK_JOINT_17, // SK_JOINT_33
        SK_JOINT_18, // SK_JOINT_34
        SK_JOINT_19, // SK_JOINT_35
        SK_JOINT_20, // SK_JOINT_36
        SK_JOINT_21, // SK_JOINT_37
        SK_JOINT_22, // SK_JOINT_38
        SK_JOINT_23, // SK_JOINT_39
        SK_JOINT_24, // SK_JOINT_40
        SK_JOINT_25, // SK_JOINT_41
        SK_JOINT_26, // SK_JOINT_42
        SK_JOINT_27, // SK_JOINT_43
        SK_JOINT_28, // SK_JOINT_44
        SK_JOINT_29, // SK_JOINT_45

        SK_JOINT_57, // SK_JOINT_46
        SK_JOINT_58, // SK_JOINT_47
        SK_JOINT_59, // SK_JOINT_48
        SK_JOINT_60, // SK_JOINT_49
        SK_JOINT_61, // SK_JOINT_50
        SK_JOINT_62, // SK_JOINT_51
        SK_JOINT_63, // SK_JOINT_52
        SK_JOINT_64, // SK_JOINT_53
        SK_JOINT_65, // SK_JOINT_54
        SK_JOINT_66, // SK_JOINT_55
        SK_JOINT_67, // SK_JOINT_56

        SK_JOINT_46, // SK_JOINT_57
        SK_JOINT_47, // SK_JOINT_58
        SK_JOINT_48, // SK_JOINT_59
        SK_JOINT_49, // SK_JOINT_60
        SK_JOINT_50, // SK_JOINT_61
        SK_JOINT_51, // SK_JOINT_62
        SK_JOINT_52, // SK_JOINT_63
        SK_JOINT_53, // SK_JOINT_64
        SK_JOINT_54, // SK_JOINT_65
        SK_JOINT_55, // SK_JOINT_66
        SK_JOINT_56, // SK_JOINT_67
};
const string jointName[SK_JOINT_END] = {
        "380_pivot",           // SK_JOINT_0  nome collada: joint0
        "360_torso",           // SK_JOINT_1  nome collada: joint1
        "320_neck",            // SK_JOINT_2  nome collada: joint2
        "300_head",            // SK_JOINT_3  nome collada: joint3
        "280_left_collar",     // SK_JOINT_4  nome collada: joint4
        "260_right_collar",    // SK_JOINT_5  nome collada: joint5
        "240_left_upper_arm",  // SK_JOINT_6  nome collada: joint6
        "220_right_upper_arm", // SK_JOINT_7  nome collada: joint7
        "200_left_upper_leg",  // SK_JOINT_8  nome collada: joint8
        "180_right_upper_leg", // SK_JOINT_9  nome collada: joint9
        "160_left_lower_arm",  // SK_JOINT_10  nome collada: joint10
        "140_right_lower_arm", // SK_JOINT_11  nome collada: joint11
        "120_left_lower_leg",  // SK_JOINT_12  nome collada: joint12
        "100_right_lower_leg", // SK_JOINT_13  nome collada: joint13
        "080_left_hand",       // SK_JOINT_14  nome collada: joint14

        "075_left_littlefinger_1", // SK_JOINT_15  nome collada: joint15
        "074_left_littlefinger_2", // SK_JOINT_16  nome collada: joint16
        "073_left_littlefinger_3", // SK_JOINT_17  nome collada: joint17
        "072_left_ringfinger_1",   // SK_JOINT_18  nome collada: joint18
        "071_left_ringfinger_2",   // SK_JOINT_19  nome collada: joint19
        "070_left_ringfinger_3",   // SK_JOINT_20  nome collada: joint20
        "069_left_middlefinger_1", // SK_JOINT_21  nome collada: joint21
        "068_left_middlefinger_2", // SK_JOINT_22  nome collada: joint22
        "067_left_middlefinger_3", // SK_JOINT_23  nome collada: joint23
        "066_left_forefinger_1",   // SK_JOINT_24  nome collada: joint24
        "065_left_forefinger_2",   // SK_JOINT_25  nome collada: joint25
        "064_left_forefinger_3",   // SK_JOINT_26  nome collada: joint26
        "063_left_pollex_1",       // SK_JOINT_27  nome collada: joint27
        "062_left_pollex_2",       // SK_JOINT_28  nome collada: joint28
        "061_left_pollex_3",       // SK_JOINT_29  nome collada: joint29

        "060_right_hand",           // SK_JOINT_30  nome collada: joint30
        "055_right_littlefinger_1", // SK_JOINT_31  nome collada: joint31
        "054_right_littlefinger_2", // SK_JOINT_32  nome collada: joint32
        "053_right_littlefinger_3", // SK_JOINT_33  nome collada: joint33
        "052_right_ringfinger_1",   // SK_JOINT_34  nome collada: joint34
        "051_right_ringfinger_2",   // SK_JOINT_35  nome collada: joint35
        "050_right_ringfinger_3",   // SK_JOINT_36  nome collada: joint36
        "049_right_middlefinger_1", // SK_JOINT_37  nome collada: joint37
        "048_right_middlefinger_2", // SK_JOINT_38  nome collada: joint38
        "047_right_middlefinger_3", // SK_JOINT_39  nome collada: joint39
        "046_right_forefinger_1",   // SK_JOINT_40  nome collada: joint40
        "045_right_forefinger_2",   // SK_JOINT_41  nome collada: joint41
        "044_right_forefinger_3",   // SK_JOINT_42  nome collada: joint42
        "043_right_pollex_1",       // SK_JOINT_43  nome collada: joint43
        "042_right_pollex_2",       // SK_JOINT_44  nome collada: joint44
        "041_right_pollex_3",       // SK_JOINT_45  nome collada: joint45

        "040_left_foot",           // SK_JOINT_46  nome collada: joint46
        "030_left_footfinger_1_1", // SK_JOINT_47  nome collada: joint47
        "029_left_footfinger_1_2", // SK_JOINT_48  nome collada: joint48
        "028_left_footfinger_2_1", // SK_JOINT_49  nome collada: joint49
        "027_left_footfinger_2_2", // SK_JOINT_50  nome collada: joint50
        "026_left_footfinger_3_1", // SK_JOINT_51  nome collada: joint51
        "025_left_footfinger_3_2", // SK_JOINT_52  nome collada: joint52
        "024_left_footfinger_4_1", // SK_JOINT_53  nome collada: joint53
        "023_left_footfinger_4_2", // SK_JOINT_54  nome collada: joint54
        "022_left_footfinger_5_1", // SK_JOINT_55  nome collada: joint55
        "021_left_footfinger_5_2", // SK_JOINT_56  nome collada: joint56

        "020_right_foot",           // SK_JOINT_57  nome collada: joint57
        "009_right_footfinger_1_1", // SK_JOINT_58  nome collada: joint58
        "008_right_footfinger_1_2", // SK_JOINT_59  nome collada: joint59
        "007_right_footfinger_2_1", // SK_JOINT_60  nome collada: joint60
        "006_right_footfinger_2_2", // SK_JOINT_61  nome collada: joint61
        "005_right_footfinger_3_1", // SK_JOINT_62  nome collada: joint62
        "004_right_footfinger_3_2", // SK_JOINT_63  nome collada: joint63
        "003_right_footfinger_4_1", // SK_JOINT_64  nome collada: joint64
        "002_right_footfinger_4_2", // SK_JOINT_65  nome collada: joint65
        "001_right_footfinger_5_1", // SK_JOINT_66  nome collada: joint66
        "000_right_footfinger_5_2", // SK_JOINT_67  nome collada: joint67
                                    /*
                                    "dummy1",
                                    "dummy2",
                                    "dummy3",
                                    "dummy4",
                                    */
};


#define DUMMY_JOINTS (SK_JOINT_END - SK_JOINT_68)

const DummyJoint dummyJoints[DUMMY_JOINTS] = {
        {SK_NONE, glm::vec3(0, 0, 0)}, {SK_NONE, glm::vec3(0, 0, 0)}, {SK_NONE, glm::vec3(0, 0, 0)},
        {SK_NONE, glm::vec3(0, 0, 0)}, {SK_NONE, glm::vec3(0, 0, 0)},
};

const float M_PI_180 = (glm::pi<float>() / 180.0);

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
	clearTargetmap();
	clearPosemap();
}

void Mesh::clearTargetmap()
{
	TargetMap::iterator i;
	for(i = m_targets.begin(); i != m_targets.end(); ++i) {
		delete i->second;
	}
	m_targets.clear();
}

void Mesh::clearPosemap()
{
	PoseMap::iterator i;
	for(i = m_posemap.begin(); i != m_posemap.end(); ++i) {
		delete i->second;
	}
	m_posemap.clear();
}

void Mesh::calcSharedVertices()
{
	if(m_vert_meta.size() != 0) {
		m_vert_meta.clear();
	}
	
	m_vert_meta.resize(m_vert_morph.m_verts.size());
	
	for(unsigned int i = 0; i < m_faces.size(); i++) {
		int vertex_number;

		const Face & face(m_faces[i]);

		for(unsigned int j = 0; j < face.getSize(); j++) {
			vertex_number = face.getVertexAtIndex(j);

			VertexMeta & vertex = m_vert_meta[vertex_number];

			vertex.addSharedFace(i);
		}
	}
}

void Mesh::calcVertexNormals()
{
	assert(m_vert_morph.m_verts.size() == m_vert_meta.size());
	
	for(unsigned int i = 0; i < m_vert_morph.m_verts.size(); i++) {
		const VertexMeta & vMeta = m_vert_meta[i];
		glm::vec3 & normal = m_vert_morph.m_normals[i];

		const auto & faces = vMeta.getSharedFaces();
		for(unsigned int j = 0; j < faces.size(); j++) {
			normal += m_faces.at(faces[j]).no;
		}

		normal = glm::normalize(normal);
	}
}

void Mesh::calcFaceNormals()
{
	for(unsigned int i = 0; i < m_faces.size(); i++) {
		Face & face = m_faces[i];
		if(face.getSize() >= 3) {
			const Vertex & vertex1(m_vert_morph.m_verts[face.getVertexAtIndex(0)]);
			const Vertex & vertex2(m_vert_morph.m_verts[face.getVertexAtIndex(1)]);
			const Vertex & vertex3(m_vert_morph.m_verts[face.getVertexAtIndex(2)]);

			const glm::vec3 v1_tmp(vertex2.pos - vertex1.pos);
			const glm::vec3 v2_tmp(vertex3.pos - vertex1.pos);

			face.no = glm::normalize(glm::cross(v1_tmp, v2_tmp));
		} else {
			std::cerr << "Error: a face needs at least 3 vertices!" << std::endl;
			return;
		}
	}
}

void Mesh::calcNormals()
{
	calcFaceNormals();
	calcVertexNormals();
}

bool Mesh::loadGroups(const string & partsFile)
{
	auto groups_filename = searchDataFile(partsFile);
	
	return m_facegroup.load(groups_filename);
}

bool Mesh::loadSmoothVertex(const string & smoothFile)
{
	auto filename = searchDataFile(smoothFile);
	
	return m_smoothvertex.load(filename);
}

bool Mesh::loadMesh(const string & meshFile, const string & faceFile)
{
	auto mesh_filename = searchDataFile(meshFile);
	auto faces_filename = searchDataFile(faceFile);
	
	bool vload = m_vert_morph.load(mesh_filename);
	bool fload = m_faces.loadGeometry(faces_filename);

	if(!vload || !fload)
		return false;

	m_vert_morph_only.load(mesh_filename);
	calcSharedVertices();
	calcNormals();

	// create initial copy for original mesh
	m_vert_orginal.clear();
	for(unsigned int i = 0; i < m_vert_morph.m_verts.size(); i++) {
		const Vertex & vertex_morph(m_vert_morph.m_verts[i]);
		m_vert_orginal.push_back(vertex_morph.pos);
	}

	return true;
}

PoseTarget * Mesh::getPoseTargetForName(const string & inTargetname) const
{
	PoseMap::const_iterator i = m_posemap.find(inTargetname);
	if(i == m_posemap.end())
		return NULL;

	PoseTarget * poseTarget = i->second;
	if(!poseTarget)
		return NULL;

	return poseTarget;
}

bool Mesh::loadMaterial(const string & matFile, const string & colFile)
{
	auto material_filename = searchDataFile(matFile);
	auto face_colors_filename = searchDataFile(colFile);
	
	bool mload  = m_materials.loadMaterials(material_filename);
	bool fcload = m_faces.loadColors(face_colors_filename);

	if(!mload || !fcload)
		return false;
	return true;
}

bool Mesh::loadTextureVector(const string & uvFile)
{
	auto filename = searchDataFile(uvFile);
	
	return m_texture_vector.load(filename);
}

void Mesh::loadPoseTargets(const string & target_root_path, int recursive_level)
{
	clearPosemap(); // wipe out all old posemap entries first

	// reading all the targets recursively
	DirectoryList dir_list;

	dir_list.setRootPath(target_root_path);
	dir_list.setRecursive(recursive_level);
	dir_list.setFileType(dir_list.DIRECTORY);

	const StringList & str_list(dir_list.getDirectoryList());

	for(const string & file : str_list) {
		string target_name(file);

		target_name.erase(0, target_root_path.length() + 1);
		string::size_type loc = target_name.find("/", 0);
		if(loc == string::npos) {
			continue;
		}

		PoseTarget * poseTarget = new PoseTarget();
		bool         rc         = poseTarget->load(file);
		if(!rc) {
			delete poseTarget;
		} else {
			m_posemap[target_name] = poseTarget;
		}
	}
}

void Mesh::loadCharacters(const string & characters_root_path, int recursive_level)
{
	// reading all the targets recursively
	DirectoryList dir_list;

	dir_list.setRootPath(characters_root_path);
	dir_list.setRecursive(recursive_level);
	dir_list.setFileFilter(".bs");

	for(const string & file : dir_list.getDirectoryList()) {
		string character_name(file);

		character_name.erase(0, characters_root_path.length() + 1);

		BodySettings character;
		character.load(file);
		m_characters[character_name] = character;
	}
}



void Mesh::roundtripCharacters() {
	// reading all the targets recursively
	DirectoryList dir_list;
	
	dir_list.setRootPath(searchDataDir("bs_data/characters1"));
	dir_list.setRecursive(1);
	dir_list.setFileFilter(".bs");
	
	for(const string & file : dir_list.getDirectoryList()) {
		string character_name(file);
		
		BodySettings in;
		in.load(file);
		
		Grids grid;
		grid.fromSavedPositions(in.m_comp);
		
		BodySettings out;
		grid.toSavedPositions(out.m_comp);
		
	//	for(auto a : in.m_targets)
	//		out.m_targets[a.first] = a.second;
		
		m_morphTargets.m_targets.clear();
		grid.applyCompositeMorphTargets(*this);
		for(auto a : m_morphTargets.m_targets)
			out.m_targets[a.first] = a.second;
		
		
		namespace fs = std::filesystem;
		fs::path p = fs::path("../MyHumanoid/") / fs::path(file);
		auto foo = fs::canonical(fs::absolute(p));
		log_info("OUT: {}", foo.string());
		//fs::create_directories(foo.parent_path());
		out.save(foo.string());
	}
}

void Mesh::loadTargets(const string & target_root_path, int recursive_level, bool clearmap)
{

	if(clearmap) {
		clearTargetmap(); // wipe out all old targetmap entries first
	}

	// reading all the targets recursively
	DirectoryList dir_list;

	dir_list.setRootPath(target_root_path);
	dir_list.setRecursive(recursive_level);
	dir_list.setFileFilter(".target");

	const StringList & str_list(dir_list.getDirectoryList());

	for(const string & file : str_list) {
		string target_name(file);
		target_name.erase(0, target_root_path.length() + 1);

		Target * targetEntry = new Target();
		bool     rc          = targetEntry->load(file);
		if(!rc) {
			delete targetEntry;
			targetEntry = nullptr;
		}

		m_targets[target_name] = targetEntry;
	}
}

const Target * Mesh::getTargetForName(const string & inTargetname)
{
	TargetMap::iterator i = m_targets.find(inTargetname);
	if(i == m_targets.end())
		return NULL;

	return i->second;
}

bool Mesh::setMorphTarget(const string & target_name, float morph_value)
{
	ZoneScoped;

	// return if target doesn't exist
	if(!m_targets.count(target_name)) {
		std::cerr << "a target with name \"" << target_name << "\" wasn't found in targetmap"
		     << std::endl;
		return false;
	}

	float real_morph_value;
	float bs_morph_value = m_morphTargets.m_targets[target_name];

	// TODO: round morph value??

	// get correct additive morph value
	if(morph_value == 0.0) {
		real_morph_value = -bs_morph_value;
	} else {
		real_morph_value = morph_value - bs_morph_value;
	}

	const Target * target = getTargetForName(target_name);

	for(const TargetData & td : *target) {
		// vertexvector_morph[td.vertex_number].co += (td.morph_vector -
		// vertexvector_orginal[td.vertex_number]) * real_morph_value;
		m_vert_morph.m_verts[td.vertex_number].pos += td.morph_vector * real_morph_value;

		m_vert_morph_only.m_verts[td.vertex_number].pos += td.morph_vector * real_morph_value;
	}

	if(morph_value == 0.0) {
		m_morphTargets.m_targets.erase(target_name);
	} else {
		m_morphTargets.m_targets[target_name] = morph_value;
	}

	return true;
}

void Mesh::doMorph(const BodySettings & bs)
{
	ZoneScoped;

	m_morphTargets.m_targets.clear();
	m_vert_morph.setCoordinates(m_vert_orginal);
	m_vert_morph_only.setCoordinates(m_vert_orginal);

	for(const auto & [target_name, morph_value] : bs.m_targets) {
		setMorphTarget(target_name, morph_value);
	}
}

void Mesh::initPoses()
{
	for(auto & [key, poseTarget] : m_posemap) {
		assert(poseTarget);
		poseTarget->calcRotationsCenteroids(m_vert_morph_copy);
		poseTarget->calcTranslationsFormFactors(m_vert_morph_copy);
		poseTarget->calcNormalizations();
	}
}

void Mesh::poseMode()
{
	m_vert_morph_copy = m_vert_morph;

	initPoses();

	doPose(m_poseTargets, false);
}

void Mesh::bodyDetailsMode()
{
	m_vert_morph = m_vert_morph_copy;
}

void Mesh::resetPose()
{
	m_poseTargets.m_targets.clear();
	m_vert_morph      = m_vert_morph_copy;
	m_vert_morph_only = m_vert_morph_copy;
}

bool Mesh::setPose(const std::string & target_name, float morph_value, bool removeOnZero)
{
	// return if target doesn't exist
	if(!m_posemap.count(target_name)) {
		log_error("a target with name '{}' wasn't found in posemap", target_name);
		return false;
	}

	if(removeOnZero && morph_value == 0.0) {
		m_poseTargets.m_targets.erase(target_name);
	} else {
		m_poseTargets.m_targets[target_name] = morph_value;
	}

	m_vert_morph = m_vert_morph_copy;

	for(const auto & [target_name, morph_value] : m_poseTargets.m_targets) {
		PoseTarget * poseTarget = getPoseTargetForName(target_name);
		assert(poseTarget);
		doPose(target_name, morph_value, poseTarget->getModVertex());
	}

	applySmooth(2);

	return true;
}

void Mesh::doPoseRotation(const PoseRotation & pr, float morph_value, const UsedVertex & modVertex)
{
	float  real_value = 0;
	float  theta;
	Matrix rotMatrix;

	if(pr.getNormalize()) {
		if(morph_value < 0) {
			if(morph_value < pr.getMaxAngle()) {
				real_value = glm::max(morph_value, pr.getMinAngle()) - pr.getMaxAngle();
			}
		} else {
			if(morph_value > pr.getMinAngle()) {
				real_value = glm::min(morph_value, pr.getMaxAngle()) - pr.getMinAngle();
			}
		}
	} else {
		real_value = morph_value;
	}

	RotateAxis axis = pr.getAxis();

	for(const PoseTargetData & td : pr.m_targetData) {

		// continue if the Pose Target datas Vertex is not part of the modVertex
		if(modVertex.find(td.vertex_number) == modVertex.end()) {
			continue;
		}

		theta = real_value * td.rotation;
		rotMatrix.setRotation(theta * M_PI_180, axis);

		m_vert_morph.m_verts[td.vertex_number].pos =
		        ((m_vert_morph.m_verts[td.vertex_number].pos - pr.getCenter()) * rotMatrix) +
		        pr.getCenter();
	}
}

void Mesh::doPoseTranslation(const PoseTranslation & pt, float morph_value,
                             const UsedVertex & modVertex)
{
	const Target &  tmpTarget  = pt.getTarget();
	const glm::vec3 formFactor = pt.getFormFactor();
	float           real_value = 0;

	if(pt.getNormalize()) {
		if(morph_value < 0) {
			if(morph_value < pt.getMaxAngle()) {
				real_value = glm::max(morph_value, pt.getMinAngle()) - pt.getMaxAngle();
			}
		} else {
			if(morph_value > pt.getMinAngle()) {
				real_value = glm::min(morph_value, pt.getMaxAngle()) - pt.getMinAngle();
			}
		}
	} else {
		real_value = morph_value;
	}

	for(const TargetData & td : tmpTarget) {

		// continue if the Pose Target datas Vertex is not part of the modVertex
		if(modVertex.find(td.vertex_number) == modVertex.end()) {
			continue;
		}

		// vertexvector_morph[td.vertex_number].co += (td.morph_vector -
		// vertexvector_morph[td.vertex_number].co) * (morph_value * formFactor);
		// vertexvector_morph[td.vertex_number].co += (td.morph_vector * formFactor)
		// * real_value;
		m_vert_morph.m_verts[td.vertex_number].pos += glm::vec3(formFactor.x * td.morph_vector.x,
		                                               formFactor.y * td.morph_vector.y,
		                                               formFactor.z * td.morph_vector.z) *
		                                     real_value;
	}
}

void Mesh::doPose(const string & target_name, float morph_value, const UsedVertex & modVertex)
{
	string cat("00");

	PoseTarget * poseTarget = getPoseTargetForName(target_name);
	assert(poseTarget);

	list<PoseRotation> & rotations = (morph_value < 0) ? poseTarget->getNegativeRotations()
	                                                   : poseTarget->getPositiveRotations();

	list<PoseTranslation> & translations = (morph_value < 0)
	                                               ? poseTarget->getNegativeTranslations()
	                                               : poseTarget->getPositiveTranslations();

	list<PoseTranslation>::iterator translations_it;
	list<PoseRotation>::iterator    rotations_it = rotations.begin();

	// translations
	for(const PoseTranslation & pt : translations) {

		if(cat != pt.getCat()) {
			while(rotations_it != rotations.end()) {
				const PoseRotation & pr(*rotations_it);
				if(pr.getCat() != cat) {
					break;
				}

				doPoseRotation(pr, morph_value, modVertex);
				rotations_it++;
			}

			cat = pt.getCat();
		}

		doPoseTranslation(pt, morph_value, modVertex);
	}

	while(rotations_it != rotations.end()) {
		const PoseRotation & pr(*rotations_it);

		doPoseRotation(pr, morph_value, modVertex);
		rotations_it++;
	}
}

void Mesh::applySmooth(const int recursive_level)
{
	int i, vToMove;

	for(i = 0; i < recursive_level; i++) {
		for(std::vector<SmoothData>::iterator smooth_it = m_smoothvertex.begin();
		    smooth_it != m_smoothvertex.end(); smooth_it++) {
			SmoothData &               smooth        = (*smooth_it);
			std::vector<int>::iterator smoothData_it = smooth.begin();

			vToMove = (*smoothData_it);

			glm::vec3 centeroid(calcCenteroid(
			        vector<int>(smoothData_it++, smooth.end()), m_vert_morph));
			m_vert_morph.m_verts[vToMove].pos = (centeroid + m_vert_morph.m_verts[vToMove].pos);
			m_vert_morph.m_verts[vToMove].pos /= 2;
		}
	}
}

bool Mesh::IsADummyJoint(SKELETON_JOINT joint, glm::vec3 & v3)
{
	for(int i = 0; i < DUMMY_JOINTS; i++) {

		if(dummyJoints[i].joint == joint) {

			v3 = dummyJoints[i].v3;
			return true;
		}
	}
	return false;
}
void Mesh::prepareSkeleton()
{
	m_jointvector.clear();

	for(int i = 0; i < SK_JOINT_END; i++) {

		glm::vec3         v(0.f, 0.f, 0.f);
		PoseMap::iterator temp = m_posemap.find(jointName[i] + "/ROT1");

		if(temp == m_posemap.end()) {

			temp = m_posemap.find(jointName[i] + "/ROT_BASE1");

			if(temp == m_posemap.end()) {
				glm::vec3 v3;
				if(IsADummyJoint((SKELETON_JOINT)i, v3) == true) {
					m_jointvector.push_back(v3);
					continue;
				}

				m_jointvector.push_back(v);
				continue;
			}
		}

		PoseTarget * poseTarget = temp->second;

		if(poseTarget == NULL) {
			m_jointvector.push_back(v);
			continue;
		}

		poseTarget->calcRotationsCenteroids(m_vert_morph_only);
		v = poseTarget->getFirstRotationCenteroid();

		m_jointvector.push_back(v);
	}
}

void Mesh::doPose(const BodySettings & bs, bool clear)
{
	ZoneScoped;
	//  bool pose = false;

	if(clear) {
		m_poseTargets.m_targets.clear();
		m_vert_morph      = m_vert_morph_copy;
		m_vert_morph_only = m_vert_morph_copy;
	}

	for(const auto & [target_name, morph_value] : bs.m_targets) {

		if(morph_value != 0.0) {
			m_poseTargets.m_targets[target_name] = morph_value;
		}

		PoseTarget * poseTarget = getPoseTargetForName(target_name);
		assert(poseTarget);
		UsedVertex & modVertex(poseTarget->getModVertex());

		doPose(target_name, morph_value, modVertex);
	}

	applySmooth(2);
}

void Mesh::doPose(const BodySettings & bs, const float value, bool clear)
{
	if(clear) {
		m_poseTargets.m_targets.clear();
		m_vert_morph      = m_vert_morph_copy;
		m_vert_morph_only = m_vert_morph_copy;
	}

	for(const auto & [target_name, val] : bs.m_targets) {
		float morph_value = val;
		morph_value *= value;

		PoseTarget * poseTarget = getPoseTargetForName(target_name);
		assert(poseTarget);

		setPose(target_name, morph_value);
	}
}

SKELETON_JOINT Mesh::getSymmetricJoint(SKELETON_JOINT joint)
{
	if(joint < SK_JOINT_0 || joint >= SK_JOINT_END)
		return SK_JOINT_0;

	else
		return jointSymmetric[joint];
}

}
