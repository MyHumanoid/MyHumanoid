/* SPDX-License-Identifier: GPL-3.0-or-later */
/*
 *  Copyright (C) 2005  Andreas Volz
 *  Copyright (C) 2006-2007  MakeHuman Project
 *
 *  This program is free software; you  can  redistribute  it  and/or
 *  modify  it  under  the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either  version  3  of
 *  the License, or (at your option) any later version.
 *
 *  This  program  is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the  implied  warranty  of
 *  MERCHANTABILITY  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software Foun-
 *  dation, Inc., 59 Temple Place, Suite 330, Boston,  MA  02111-1307
 *  USA
 *
 *  File: Mesh.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: ANIMORPH
 *
 *  For individual developers look into the AUTHORS file.
 *
 */
#pragma once

#include "BodySettings.h"
#include "DirectoryList.h"
#include "Euler.h"
#include "Face.h"
#include "FaceGroup.h"
#include "FaceVector.h"
#include "Material.h"
#include "Matrix.h"
#include "PoseTarget.h"
#include "SmoothVertex.h"
#include "Target.h"
#include "TextureVector.h"
#include "Vertex.h"
#include "VertexGroup.h"
#include "VertexVector.h"
#include "util.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <stdio.h>
#include <string>
#include <vector>


using std::map;
using std::ostringstream;
using std::string;
using std::vector;


#define MAX_NUMBER_SUBJOINT 6
typedef enum SKELETON_JOINT {
	SK_NONE = -1,
	SK_JOINT_0,
	SK_JOINT_1,
	SK_JOINT_2,
	SK_JOINT_3,
	SK_JOINT_4,
	SK_JOINT_5,
	SK_JOINT_6,
	SK_JOINT_7,
	SK_JOINT_8,
	SK_JOINT_9,
	SK_JOINT_10,
	SK_JOINT_11,
	SK_JOINT_12,
	SK_JOINT_13,
	SK_JOINT_14,
	SK_JOINT_15,
	SK_JOINT_16,
	SK_JOINT_17,
	SK_JOINT_18,
	SK_JOINT_19,
	SK_JOINT_20,
	SK_JOINT_21,
	SK_JOINT_22,
	SK_JOINT_23,
	SK_JOINT_24,
	SK_JOINT_25,
	SK_JOINT_26,
	SK_JOINT_27,
	SK_JOINT_28,
	SK_JOINT_29,

	SK_JOINT_30,
	SK_JOINT_31,
	SK_JOINT_32,
	SK_JOINT_33,
	SK_JOINT_34,
	SK_JOINT_35,
	SK_JOINT_36,
	SK_JOINT_37,
	SK_JOINT_38,
	SK_JOINT_39,

	SK_JOINT_40,
	SK_JOINT_41,
	SK_JOINT_42,
	SK_JOINT_43,
	SK_JOINT_44,
	SK_JOINT_45,
	SK_JOINT_46,
	SK_JOINT_47,
	SK_JOINT_48,
	SK_JOINT_49,

	SK_JOINT_50,
	SK_JOINT_51,
	SK_JOINT_52,
	SK_JOINT_53,
	SK_JOINT_54,
	SK_JOINT_55,
	SK_JOINT_56,
	SK_JOINT_57,
	SK_JOINT_58,
	SK_JOINT_59,

	SK_JOINT_60,
	SK_JOINT_61,
	SK_JOINT_62,
	SK_JOINT_63,
	SK_JOINT_64,
	SK_JOINT_65,
	SK_JOINT_66,
	SK_JOINT_67,
	SK_JOINT_68, //= DUMMY
	SK_JOINT_69, //= DUMMY
	SK_JOINT_70, //= DUMMY
	SK_JOINT_71, //= DUMMY
	SK_JOINT_72, //= DUMMY
	SK_JOINT_END,

} SKELETON_JOINT;
extern const SKELETON_JOINT subjoint[][MAX_NUMBER_SUBJOINT];

namespace Animorph
{

typedef struct DummyJoint {

	SKELETON_JOINT joint;
	glm::vec3      v3;
} DummyJoint;

using TargetMap     = map<string, Target *>;
using PoseMap       = map<string, PoseTarget *>;
using CharactersMap = map<string, BodySettings>;

/*! \brief A poseable and morphable mesh.

  This is the central class of animorph.

  General usage:

  -# Mesh m = new Mesh; // Instantiante a Mesh object
  -# m.loadMeshFactory("base.vertices", "base.faces");
  -# m.loadXXXFactory(); // load possible morph targets and pose targets
  -# m.doMorph(); // perform intended morphings
  -# m.doPose(); // perform intended posings
  -# fetch the resulting faces, vertices, materials etc. for displaying

 */
class Mesh
{
	FaceVector        m_faces;
	VertexVector      m_vert_morph;      //!< container for modified mesh
	VertexVector      m_vert_morph_copy; //!< copy container for morphed mesh
	VertexVector      m_vert_morph_only;
	vector<glm::vec3> m_vert_orginal; //!< container for orginal mesh
	vector<VertexMeta> m_vert_meta;
	
	BodySettings      m_morphTargets;
	TargetMap         m_targets;
	MaterialVector    m_materials;
	BodySettings      m_poseTargets; ///< Currently active PoseTargets
	PoseMap           m_posemap;     ///< Possible pose transformations
	CharactersMap     m_characters;  //!< container for all characters
	SmoothVertex      m_smoothvertex;
	vector<glm::vec3> m_jointvector;
	TextureVector     m_texture_vector;
	FaceGroup         m_facegroup;

private:
	/// Save with each vertex to which faces it belongs
	void calcSharedVertices();

	/*! \brief Releases all targets in targetmap
	 *
	 * Used by the destructor.
	 */
	void clearTargetmap();

	/*! \brief Releases all targets in posemap
	 *
	 * Used by the destructor.
	 */
	void clearPosemap();

	/*! \brief Calculate the normals of all faces in vertexvector_morph
	 *
	 * Calculates the face normals from coordinates of the first three vertices
	 * of each face in vertexvector_morph.
	 */
	void calcFaceNormals();

	/*! \brief Calculate the normals of all vertices in vertexvector_morph
	 *
	 * Calculates the vertex normals by averaging the normals of shared faces
	 * from facevector.
	 */
	void calcVertexNormals();

	/// Looks up inTargetname in targetmap
	const Target * getTargetForName(const string & inTargetname);

	void initPoses();

	void applySmooth(const int recursive_level = 1);

	void prepareSkeleton();

	void applySkeleton();

	bool IsADummyJoint(SKELETON_JOINT joint, glm::vec3 & v3);

	/*! \brief Apply a PoseRotation target
	 *
	 * PoseRotation targets are subtargets of PoseTargets.  The affected vertices
	 * are rotated around an axis and a center specified by the PoseRotation
	 * target. The angle of the rotation is given by morph_value.
	 *
	 * This function is used by doPose (const string& target_name, float
	 * morph_value, const UsedVertex& modVertex).
	 *
	 */
	void doPoseRotation(const PoseRotation & pr, float morph_value,
	                    const UsedVertex & modVertex);

	/*! \brief Apply a PoseTranslation target
	 *
	 * PoseTranslation targets are subtargets of PoseTargets.
	 *
	 * This function is used by doPose (const string& target_name, float
	 * morph_value, const UsedVertex& modVertex).
	 *
	 */
	void doPoseTranslation(const PoseTranslation & pt, float morph_value,
	                       const UsedVertex & modVertex);

public:
	Mesh();
	~Mesh();

	/*! \brief Get a Ptr to a PoseTarget object which belongs to a given name
	 *
	 * @param inTargetname The name of the Pose Target to get.
	 * @return a pointer to the pose target to get or NULL if this target does not
	 *         exist
	 */
	PoseTarget * getPoseTargetForName(const string & inTargetname) const;

	/** @name Getting pointers to member variables
	 */
	//@{

	const vector<glm::vec3> & getJointVector() { return m_jointvector; }
	glm::vec3                 GetJoint0_Pos() { return getJointVector().at(0); }
	//@}

	//@{
	const VertexVector & getVertexVectorRef() { return m_vert_morph; }

	const VertexVector & getVertexVectorMorphOnlyRef() { return m_vert_morph_only; }

	const FaceVector & faces() { return m_faces; }

	const MaterialVector & materials() const { return m_materials; }

	const TargetMap & targets() { return m_targets; }

	const PoseMap & posemap() { return m_posemap; }

	const CharactersMap & characters() { return m_characters; }
	//@}

	const BodySettings & morphTargets() const { return m_morphTargets; }

	const BodySettings & poseTargets() const { return m_poseTargets; }

	const TextureVector & textureVector() const { return m_texture_vector; }
	
	// TODO make const
	FaceGroup & facegroup() { return m_facegroup; }

	/** @name Loading
	 */
	//@{
	bool loadMesh(const string & mesh_filename, const string & faces_filename);

	bool loadMaterial(const string & material_filename, const string & face_colors_filename);

	bool loadTextureVector(const string & filename);

	/// Load all (pose) Targets recursively from a directory.
	/*!
	 * \param target_root_path the root path with targets to load
	 * \param recursive_level Set the level of directory recursion. See
	 * DirectoryList for more help. \return true if files are found \return false
	 * if files aren't found
	 */
	void loadTargets(const string & target_root_path, int recursive_level = 1,
	                 bool clearmap = true);

	/// Load all PoseTargets recursively from a directory.
	/*!
	 * \param target_root_path the root path with targets to load
	 * \param recursive_level Set the level of directory recursion. See
	 * DirectoryList for more help. \return true if files are found \return false
	 * if files aren't found
	 */
	void loadPoseTargets(const string & target_root_path, int recursive_level = 1);

	/// Load all characters (BodySettings) recursively from a directory.
	/*!
	 * \param characters_root_path the root path with characters to load
	 * \param recursive_level Set the level of directory recursion. See
	 * DirectoryList for more help. \return true if files are found \return false
	 * if files aren't found
	 */
	void loadCharacters(const string & characters_root_path, int recursive_level = 1);

	bool loadGroups(const string & groups_filename);
	bool loadSmoothVertex(const string & filename);

	//@}

	void loadSkeleton() { prepareSkeleton(); }

	SKELETON_JOINT getSymmetricJoint(SKELETON_JOINT joint);

	/// Calculate normals for faces and vertices
	void calcNormals();

	/** @name Morphing
	 */
	//@{
	/*! \brief Morph base mesh to a target deformation
	 *
	 * This is a key function of animorph.
	 *
	 * It works by linearly interpolating the vertices comprising the target
	 * between the base mesh position and the target position.
	 *
	 * \param target_name the previously registered name of a target to morph
	 * \param morph_value the value to morph this target, where
	 *                    0 results in the base mesh position and
	 *                    1 results in the target position
	 * \return true if target was found in TargetMap and was morphed
	 * \return false if target wasn't found in TargetMap
	 */
	bool setMorphTarget(const string & target_name, float morph_value);

	/*! \brief Fully apply morphs of a BodySettings object
	 *
	 * \param bs a BodySettings object to morph the Mesh
	 * \param clear default is to delete to yet applied targets
	 *        before using a BodySettings. Use 'false' to not clear
	 * the targets before morphing
	 */
	void doMorph(const BodySettings & bs);

	/// Reset the Mesh to loaded state without deformation
	void resetMorph();
	//@}

	/** @name Posing
	 */
	//@{
	/*! \brief Apply poses of a BodySettings object
	 *
	 * Calls doPose(const string& target_name, float morph_value, const
	 * UsedVertex& modVertex) to do the actual work.
	 *
	 * \param bs a BodySettings object with target names and values
	 * \param clear default is to delete already applied targets
	 *        before using a BodySettings. Use 'false' to not clear
	 *        the targets before morphing.
	 */
	void doPose(const BodySettings & bs, bool clear = true);

	void doPose(const BodySettings & bs, const float value, bool clear = true);

	/*! \brief Apply a PoseTarget by name to the current mesh
	 *
	 * This is a key function of animorph and is called by the other variants of
	 * doPose(). It calls doPoseRotation() and doPoseTranslation() to do the
	 * actual work.
	 *
	 * \param target_name the previously registered name of a PoseTarget to morph
	 * \param morph_value the value to morph this target
	 * \param modVertex the vertices that can be modified
	 * \return true if target was found in PoseMap and was morphed
	 * \return false if target wasn't found in PoseMap and wasn't morphed
	 */
	void doPose(const string & target_name, float morph_value, const UsedVertex & modVertex);

	/*! \brief Apply an additional PoseTarget by name
	 *
	 * This function updates the list of currently active PoseTargets in poses
	 * and applies them all to the unposed but morphed base mesh using the method
	 * doPose(const string& target_name, float morph_value, const UsedVertex&
	 * modVertex).
	 *
	 * \param target_name the previously registered name of a target to morph
	 * \param morph_value angle to apply this PoseTarget at
	 * \return true if target was found in PoseMap and could be morphed
	 * \return false if target wasn't found in PoseMap and couldn't be morphed
	 */
	bool setPose(const string & target_name, float morph_value, bool removeOnZero = true);

	/// Reset the Mesh to loaded state without poses
	void resetPose();
	//@}

	/// Switch to pose mode
	void poseMode();

	/// Switch to body details mode
	void bodyDetailsMode();
};

} // namespace Animorph
