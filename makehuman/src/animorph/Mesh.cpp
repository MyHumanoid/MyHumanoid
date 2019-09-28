#include "animorph/Mesh.h"

#include <glm/glm.hpp>

#include <assert.h>

const SKELETON_JOINT subjoint[][MAX_NUMBER_SUBJOINT] = {
    /*SK_JOINT_0*/ {SK_JOINT_1, SK_JOINT_8, SK_JOINT_9, SK_NONE, SK_NONE,
                    SK_NONE},
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

using namespace std;
using namespace Animorph;

#define DUMMY_JOINTS (SK_JOINT_END - SK_JOINT_68)

const DummyJoint dummyJoints[DUMMY_JOINTS] = {
    {SK_NONE, glm::vec3(0, 0, 0)}, {SK_NONE, glm::vec3(0, 0, 0)},
    {SK_NONE, glm::vec3(0, 0, 0)}, {SK_NONE, glm::vec3(0, 0, 0)},
    {SK_NONE, glm::vec3(0, 0, 0)},
};

const float M_PI_180 = (M_PI / 180.0);

Mesh::Mesh()
    : facevector()
    , vertexvector_morph() //!< container for modified mesh
    , vertexvector_morph_copy() //!< copy container for morphed mesh
    , vertexvector_morph_only()
    , vertexvector_orginal() //!< container for orginal mesh
    , bodyset()
    , targetmap()
    , materialvector()
    , centeroid()
    , texture_vector()
    , poses() //!< container for applied poses
    , posemap() //!< container for all poses
    , charactersmap() //!< container for all characters
    , facegroup()
    , skin()
    , smoothvertex()
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
	for (i = targetmap.begin(); i != targetmap.end(); ++i) {
		delete i->second;
	}
	targetmap.clear();
}

void Mesh::clearPosemap()
{
	PoseMap::iterator i;
	for (i = posemap.begin(); i != posemap.end(); ++i) {
		delete i->second;
	}
	posemap.clear();
}

void Mesh::calcSharedVertices()
{
	for (unsigned int i = 0; i < facevector.size(); i++) {
		int vertex_number;

		const Face &face(facevector[i]);

		for (unsigned int j = 0; j < face.getSize(); j++) {
			vertex_number = face.getVertexAtIndex(j);

			Vertex &vertex = vertexvector_morph[vertex_number];

			vertex.addSharedFace(i);
		}
	}
}

void Mesh::calcVertexNormals()
{
	for (unsigned int i = 0; i < vertexvector_morph.size(); i++) {
		Vertex &vertex(vertexvector_morph[i]);

		// sum up the normals of all shared faces
		vector<int> &faces(vertex.getSharedFaces());
		for (unsigned int j = 0; j < faces.size(); j++) {
			try {
				const glm::vec3 &face_normal(facevector.at(faces[j]).no);
				vertex.no += face_normal;
			} catch (const exception &e) {
				return;
			}
		}
		
		vertex.no = glm::normalize(vertex.no);
	}
}

void Mesh::calcFaceNormals()
{
	for (unsigned int i = 0; i < facevector.size(); i++) {
		Face &face = facevector[i];
		if (face.getSize() >= 3) {
			const Vertex &vertex1(vertexvector_morph[face.getVertexAtIndex(0)]);
			const Vertex &vertex2(vertexvector_morph[face.getVertexAtIndex(1)]);
			const Vertex &vertex3(vertexvector_morph[face.getVertexAtIndex(2)]);

			const glm::vec3 v1_tmp(vertex2.co - vertex1.co);
			const glm::vec3 v2_tmp(vertex3.co - vertex1.co);
			
			face.no = glm::normalize(glm::cross(v1_tmp, v2_tmp));
		} else {
			cerr << "Error: a face needs at least 3 vertices!" << endl;
			return;
		}
	}
}

void Mesh::calcNormals()
{
	calcFaceNormals();
	calcVertexNormals();
}

bool Mesh::loadGroupsFactory(const string &groups_filename)
{
	return facegroup.load(groups_filename);
}

bool Mesh::loadSkinFactory(const string &filename)
{
	return skin.load(filename);
}

bool Mesh::loadSmoothVertexFactory(const string &filename)
{
	return smoothvertex.load(filename);
}

bool Mesh::loadMeshFactory(const string &mesh_filename,
                           const string &faces_filename)
{
	bool vload = vertexvector_morph.load(mesh_filename);
	bool fload = facevector.loadGeometry(faces_filename);

	if (!vload || !fload)
		return false;

	vertexvector_morph_only.load(mesh_filename);
	calcSharedVertices();
	calcNormals();

	// create initial copy for original mesh
	vertexvector_orginal.clear();
	for (unsigned int i = 0; i < vertexvector_morph.size(); i++) {
		const Vertex &vertex_morph(vertexvector_morph[i]);
		vertexvector_orginal.push_back(vertex_morph.co);
	}

	return true;
}

PoseTarget *Mesh::getPoseTargetForName(const string &inTargetname) const
{
	// Check if a pose entry for the given name exists
	PoseMap::const_iterator i = posemap.find(inTargetname);
	if (i == posemap.end())
		return NULL; // There is no PoseTarget for this named entry

	PoseEntry *poseEntry = i->second;
	if (!poseEntry)
		return NULL; // There is no PoseEntry

	// get the actual poseTarget for this entry (eventually by lazy loading)
	PoseTarget *poseTarget = poseEntry->getTarget();
	if (!poseTarget)
		return NULL; // There is no poseTarget

	return poseTarget;
}

bool Mesh::loadMaterialFactory(const string &material_filename,
                               const string &face_colors_filename)
{
	bool mload = materialvector.loadMaterials(material_filename);
	bool fcload = facevector.loadColors(face_colors_filename);

	if (!mload || !fcload)
		return false;
	return true;
}

void Mesh::loadPoseTargetsFactory(const string &target_root_path,
                                  int recursive_level)
{
	clearPosemap(); // wipe out all old posemap entries first

	// reading all the targets recursively
	DirectoryList dir_list;

	dir_list.setRootPath(target_root_path);
	dir_list.setRecursive(recursive_level);
	dir_list.setFileType(dir_list.DIRECTORY);

	const StringList &str_list(dir_list.getDirectoryList());

	for (StringList::const_iterator sl_it = str_list.begin();
	     sl_it != str_list.end(); sl_it++) {
		const string &file(*sl_it);
		string target_name(file);

		target_name.erase(0, target_root_path.length() + 1);
		string::size_type loc = target_name.find("/", 0);
		if (loc == string::npos) {
			continue;
		}
		posemap[target_name] = new PoseEntry(target_name, file, false);
	}
}

void Mesh::loadCharactersFactory(const string &characters_root_path,
                                 int recursive_level)
{
	// reading all the targets recursively
	DirectoryList dir_list;

	dir_list.setRootPath(characters_root_path);
	dir_list.setRecursive(recursive_level);
	dir_list.setFileFilter(".bs");

	const StringList &str_list(dir_list.getDirectoryList());

	for (StringList::const_iterator sl_it = str_list.begin();
	     sl_it != str_list.end(); sl_it++) {
		const string &file(*sl_it);
		string character_name(file);

		character_name.erase(0, characters_root_path.length() + 1);

		BodySettings character;

		character.load(file);

		charactersmap[character_name] = character;
	}
}

void Mesh::loadTargetsFactory(const string &target_root_path,
                              int recursive_level, bool clearmap)
{

	if (clearmap) {
		clearTargetmap(); // wipe out all old targetmap entries first
	}

	// reading all the targets recursively
	DirectoryList dir_list;

	dir_list.setRootPath(target_root_path);
	dir_list.setRecursive(recursive_level);
	dir_list.setFileFilter(".target");

	const StringList &str_list(dir_list.getDirectoryList());

	for (StringList::const_iterator sl_it = str_list.begin();
	     sl_it != str_list.end(); sl_it++) {
		const string &file(*sl_it);
		string target_name(file);
		target_name.erase(0, target_root_path.length() + 1);

		Target *targetEntry = new Target();
		bool rc = targetEntry->load(file);
		if(!rc) {
			delete targetEntry;
			targetEntry = nullptr;
		}
		
		targetmap[target_name] = targetEntry;
	}
}

const Target *Mesh::getTargetForName(const string &inTargetname)
{
	TargetMap::iterator i = targetmap.find(inTargetname);
	if (i == targetmap.end())
		return NULL;

	return i->second;
}

bool Mesh::doMorph(const string &target_name, float morph_value)
{
#ifdef DEBUG
	StopClock sc;
	float elapsed_time;
	sc.resetClock();
#endif // DEBUG

	// return if target doesn't exist
	if (!targetmap.count(target_name)) {
		cerr << "a target with name \"" << target_name
		     << "\" wasn't found in targetmap" << endl;
		return false;
	}

	float real_morph_value;
	float bs_morph_value = bodyset[target_name];

	// TODO: round morph value??

	// get correct additive morph value
	if (morph_value == 0.0) {
		real_morph_value = -bs_morph_value;
	} else {
		real_morph_value = morph_value - bs_morph_value;
	}

	const Target *target = getTargetForName(target_name);

	for (Target::const_iterator target_it = target->begin();
	     target_it != target->end(); target_it++) {
		const TargetData &td(*target_it);

		// vertexvector_morph[td.vertex_number].co += (td.morph_vector -
		// vertexvector_orginal[td.vertex_number]) * real_morph_value;
		vertexvector_morph[td.vertex_number].co +=
		    td.morph_vector * real_morph_value;

		vertexvector_morph_only[td.vertex_number].co +=
		    td.morph_vector * real_morph_value;
	}

	if (morph_value == 0.0) {
		bodyset.erase(target_name);
	} else {
		bodyset[target_name] = morph_value;
	}

#ifdef DEBUG
	elapsed_time = sc.getElapsedTime(StopClock::TIME_UNIT_SECONDS);
	printf("morph time: %f\n", elapsed_time);
#endif // DEBUG

	return true;
}

void Mesh::doMorph(const BodySettings &bs, bool clear)
{
#ifdef DEBUG
	StopClock sc;
	float elapsed_time;
	sc.resetClock();
#endif // DEBUG

	if (clear) {
		bodyset.clear();

		vertexvector_morph.setCoordinates(vertexvector_orginal);
		vertexvector_morph_only.setCoordinates(vertexvector_orginal);
	}

	for (BodySettings::const_iterator bs_it = bs.begin(); bs_it != bs.end();
	     bs_it++) {
		const string &target_name((*bs_it).first);
		float morph_value = (*bs_it).second;

		doMorph(target_name, morph_value);
	}

#ifdef DEBUG
	elapsed_time = sc.getElapsedTime(StopClock::TIME_UNIT_SECONDS);
	printf("bodyset morph time: %f\n", elapsed_time);
#endif // DEBUG
}

void Mesh::doMorph(const BodySettings &bs, float value, bool clear)
{
	if (clear) {
		bodyset.clear();

		vertexvector_morph.setCoordinates(vertexvector_orginal);
		vertexvector_morph_only.setCoordinates(vertexvector_orginal);
	}

	for (BodySettings::const_iterator bs_it = bs.begin(); bs_it != bs.end();
	     bs_it++) {
		const string &target_name((*bs_it).first);
		float morph_value = (*bs_it).second;

		doMorph(target_name, morph_value * value);
	}
}

void Mesh::initPoses()
{
	for (PoseMap::iterator target_it = posemap.begin();
	     target_it != posemap.end(); target_it++) {
		PoseEntry *poseEntry = target_it->second;
		assert(poseEntry);
		PoseTarget *tmp = poseEntry->getTarget();

		tmp->calcRotationsCenteroids(vertexvector_morph_copy);
		tmp->calcTranslationsFormFactors(vertexvector_morph_copy);
		tmp->calcNormalizations();
	}

	for (std::vector<SkinVertex>::iterator skin_it = skin.begin();
	     skin_it != skin.end(); skin_it++) {
		SkinVertex &skinVertex = (*skin_it);

		glm::vec3 centeroid(
		    calcCenteroid(skinVertex.getLinkedMuscles(), vertexvector_morph));

		glm::vec3 oriDist =
		    vertexvector_morph[skinVertex.getSkinVertex()].co - centeroid;
		skinVertex.setOriginalDist(glm::length(oriDist));
	}
}

void Mesh::animationMode()
{
	vertexvector_morph_copy = vertexvector_morph;

	initPoses();

	applySkin();
	applySmooth(2);
}

void Mesh::poseMode()
{
	vertexvector_morph_copy = vertexvector_morph;

	initPoses();

	doPose(poses, false);
}

void Mesh::bodyDetailsMode()
{
	vertexvector_morph = vertexvector_morph_copy;
}

void Mesh::resetMorph()
{
	BodySettings bodyset_empty;
	doMorph(bodyset_empty, true);
}
void Mesh::resetPose()
{
	poses.clear();
	vertexvector_morph = vertexvector_morph_copy;
	vertexvector_morph_only = vertexvector_morph_copy;
}

bool Mesh::setPose(const std::string &target_name, float morph_value)
{
	// return if target doesn't exist
	if (!posemap.count(target_name)) {
		cerr << "a target with name \"" << target_name
		     << "\" wasn't found in posemap" << endl;
		return false;
	}

	if (morph_value == 0.0) {
		poses.erase(target_name);
	} else {
		poses[target_name] = morph_value;
	}

	vertexvector_morph = vertexvector_morph_copy;

	for (BodySettings::iterator poses_it = poses.begin(); poses_it != poses.end();
	     poses_it++) {
		string target_name = (*poses_it).first;
		float morph_value = (*poses_it).second;

		PoseTarget *poseTarget = getPoseTargetForName(target_name);
		assert(poseTarget);
		UsedVertex &modVertex(poseTarget->getModVertex());

		doPose(target_name, morph_value, modVertex);
	}

	applySkin();
	applySmooth(2);

	return true;
}

void Mesh::doPoseRotation(const PoseRotation &pr, float morph_value,
                          const UsedVertex &modVertex)
{
	float real_value = 0;
	float theta;
	Matrix rotMatrix;

	if (pr.getNormalize()) {
		if (morph_value < 0) {
			if (morph_value < pr.getMaxAngle()) {
				real_value = max(morph_value, pr.getMinAngle()) - pr.getMaxAngle();
			}
		} else {
			if (morph_value > pr.getMinAngle()) {
				real_value = min(morph_value, pr.getMaxAngle()) - pr.getMinAngle();
			}
		}
	} else {
		real_value = morph_value;
	}

	RotateAxis axis = pr.getAxis();

	for (PoseRotation::const_iterator target_it = pr.begin();
	     target_it != pr.end(); target_it++) {
		const PoseTargetData &td(*target_it);

		// continue if the Pose Target datas Vertex is not part of the modVertex
		if (modVertex.find(td.vertex_number) == modVertex.end()) {
			continue;
		}

		theta = real_value * td.rotation;
		rotMatrix.setRotation(theta * M_PI_180, axis);

		vertexvector_morph[td.vertex_number].co =
		    ((vertexvector_morph[td.vertex_number].co - pr.getCenter()) *
		     rotMatrix) +
		    pr.getCenter();
	}
}

void Mesh::doPoseTranslation(PoseTranslation &pt, float morph_value,
                             const UsedVertex &modVertex)
{
	Target &tmpTarget = pt.getTarget();
	glm::vec3 formFactor = pt.getFormFactor();
	float real_value = 0;

	if (pt.getNormalize()) {
		if (morph_value < 0) {
			if (morph_value < pt.getMaxAngle()) {
				real_value = max(morph_value, pt.getMinAngle()) - pt.getMaxAngle();
			}
		} else {
			if (morph_value > pt.getMinAngle()) {
				real_value = min(morph_value, pt.getMaxAngle()) - pt.getMinAngle();
			}
		}
	} else {
		real_value = morph_value;
	}

	for (Target::const_iterator target_it = tmpTarget.begin();
	     target_it != tmpTarget.end(); target_it++) {
		const TargetData &td(*target_it);

		// continue if the Pose Target datas Vertex is not part of the modVertex

		if (modVertex.find(td.vertex_number) == modVertex.end()) {
			continue;
		}

		// vertexvector_morph[td.vertex_number].co += (td.morph_vector -
		// vertexvector_morph[td.vertex_number].co) * (morph_value * formFactor);
		// vertexvector_morph[td.vertex_number].co += (td.morph_vector * formFactor)
		// * real_value;
		vertexvector_morph[td.vertex_number].co +=
		    glm::vec3(formFactor.x * td.morph_vector.x,
		             formFactor.y * td.morph_vector.y,
		             formFactor.z * td.morph_vector.z) *
		    real_value;
	}
}

void Mesh::doPose(const string &target_name, float morph_value,
                  const UsedVertex &modVertex)
{
	string cat("00");

	PoseTarget *poseTarget = getPoseTargetForName(target_name);
	assert(poseTarget);

	list<PoseRotation> &rotations = (morph_value < 0)
	                                    ? poseTarget->getNegativeRotations()
	                                    : poseTarget->getPositiveRotations();

	list<PoseTranslation> &translations =
	    (morph_value < 0) ? poseTarget->getNegativeTranslations()
	                      : poseTarget->getPositiveTranslations();

	list<PoseTranslation>::iterator translations_it;
	list<PoseRotation>::iterator rotations_it = rotations.begin();

	// translations
	for (translations_it = translations.begin();
	     translations_it != translations.end(); translations_it++) {
		PoseTranslation &pt(*translations_it);

		if (cat != pt.getCat()) {
			while (rotations_it != rotations.end()) {
				const PoseRotation &pr(*rotations_it);
				if (pr.getCat() != cat) {
					break;
				}

				doPoseRotation(pr, morph_value, modVertex);
				rotations_it++;
			}

			cat = pt.getCat();
		}

		doPoseTranslation(pt, morph_value, modVertex);
	}

	while (rotations_it != rotations.end()) {
		const PoseRotation &pr(*rotations_it);

		doPoseRotation(pr, morph_value, modVertex);
		rotations_it++;
	}
}

void Mesh::applySmooth(const int recursive_level)
{
	int i, vToMove;

	for (i = 0; i < recursive_level; i++) {
		for (std::vector<SmoothData>::iterator smooth_it = smoothvertex.begin();
		     smooth_it != smoothvertex.end(); smooth_it++) {
			SmoothData &smooth = (*smooth_it);
			std::vector<int>::iterator smoothData_it = smooth.begin();

			vToMove = (*smoothData_it);

			glm::vec3 centeroid(calcCenteroid(
			    vector<int>(smoothData_it++, smooth.end()), vertexvector_morph));
			vertexvector_morph[vToMove].co =
			    (centeroid + vertexvector_morph[vToMove].co);
			vertexvector_morph[vToMove].co /= 2;
		}
	}
}

void Mesh::applySkin()
{
	for (std::vector<SkinVertex>::iterator skin_it = skin.begin();
	     skin_it != skin.end(); skin_it++) {
		SkinVertex &skinVertex = (*skin_it);

		glm::vec3 centeroid(
		    calcCenteroid(skinVertex.getLinkedMuscles(), vertexvector_morph));

		glm::vec3 normal(calcAverageNormalLength(skinVertex.getLinkedMuscles(),
		                                        vertexvector_morph));
		
		float r = skinVertex.getOriginalDist() / glm::length(normal);//normal.getMagnitude();
		glm::vec3 delta = normal * r;
		vertexvector_morph[skinVertex.getSkinVertex()].co = centeroid + delta;
	}
}

bool Mesh::IsADummyJoint(SKELETON_JOINT joint, glm::vec3 &v3)
{
	for (int i = 0; i < DUMMY_JOINTS; i++) {

		if (dummyJoints[i].joint == joint) {

			v3 = dummyJoints[i].v3;
			return true;
		}
	}
	return false;
}
void Mesh::prepareSkeleton()
{
	jointvector.clear();

	for (int i = 0; i < SK_JOINT_END; i++) {

		glm::vec3 v(0.f, 0.f, 0.f);
		PoseTarget *tmp;
		PoseEntry *poseEntry;
		PoseMap::iterator temp = posemap.find(jointName[i] + "/ROT1");
		//   jointvector[y] =

		if (temp == posemap.end()) {

			temp = posemap.find(jointName[i] + "/ROT_BASE1");

			if (temp == posemap.end()) {
				glm::vec3 v3;
				if (IsADummyJoint((SKELETON_JOINT)i, v3) == true) {
					jointvector.push_back(v3);
					continue;
				}

				jointvector.push_back(v);
				continue;
			}
		}

		poseEntry = temp->second;

		if (poseEntry == NULL) {
			jointvector.push_back(v);
			continue;
		}

		tmp = poseEntry->getTarget();

		if (tmp == NULL) {
			jointvector.push_back(v);
			continue;
		}

		tmp->calcRotationsCenteroids(vertexvector_morph_only);
		v = tmp->getFirstRotationCenteroid();

		jointvector.push_back(v);
	}
}

void Mesh::doPose(const BodySettings &bs, bool clear)
{
#ifdef DEBUG
	StopClock sc;
	float elapsed_time;
	sc.resetClock();
#endif // DEBUG
	//  bool pose = false;

	if (clear) {
		poses.clear();
		vertexvector_morph = vertexvector_morph_copy;
		vertexvector_morph_only = vertexvector_morph_copy;
	}

	for (BodySettings::const_iterator bs_it = bs.begin(); bs_it != bs.end();
	     bs_it++) {
		string target_name = (*bs_it).first;
		float morph_value = (*bs_it).second;

		if (morph_value != 0.0) {
			poses[target_name] = morph_value;
		}

		PoseTarget *poseTarget = getPoseTargetForName(target_name);
		assert(poseTarget);
		UsedVertex &modVertex(poseTarget->getModVertex());

		doPose(target_name, morph_value, modVertex);
	}

	applySkin();
	applySmooth(2);

#ifdef DEBUG
	elapsed_time = sc.getElapsedTime(StopClock::TIME_UNIT_SECONDS);
	printf("bodyset morph time: %f\n", elapsed_time);
#endif // DEBUG
}

void Mesh::doPose(const BodySettings &bs, const float value, bool clear)
{
	if (clear) {
		poses.clear();
		vertexvector_morph = vertexvector_morph_copy;
		vertexvector_morph_only = vertexvector_morph_copy;
	}

	for (BodySettings::const_iterator bs_it = bs.begin(); bs_it != bs.end();
	     bs_it++) {
		string target_name = (*bs_it).first;
		float morph_value = (*bs_it).second;
		morph_value *= value;

		PoseTarget *poseTarget = getPoseTargetForName(target_name);
		assert(poseTarget);

		setPose(target_name, morph_value);
	}
}

SKELETON_JOINT Mesh::getSymmetricJoint(SKELETON_JOINT joint)
{
	if (joint < SK_JOINT_0 || joint >= SK_JOINT_END)
		return SK_JOINT_0;

	else
		return jointSymmetric[joint];
}


PoseEntry::PoseEntry(const string &inFilename, const string &inFullPath,
                     bool inPreload)
    : mFilename(new string(inFilename))
    , mFullPath(new string(inFullPath))
    , mTarget(NULL)
    ,                     // not initialized yet
    mTargetLoadTry(false) // not tried yet
{
	if (inPreload)
		loadFromFile();
}

PoseEntry::~PoseEntry()
{
	delete mFilename;
	delete mTarget;
	delete mFullPath;
}

PoseTarget *PoseEntry::getTarget()
{
	if (!mTargetLoadTry)
		loadFromFile();
	return mTarget;
}

bool PoseEntry::loadFromFile()
{
	// if we didn't try to load the target then try it NOW
	if (mTargetLoadTry == false) {
		mTargetLoadTry = true; // marks as 'tried to load' for further attempts

		mTarget = new (std::nothrow) PoseTarget(mFilename, mFullPath);
		assert(mTarget);
		mTarget->load();
	}
	return (mTarget != NULL); // when a target object exists, then return true
}
