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

#ifndef MESH_H
#define MESH_H 1

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "BodySettings.h"
#include "DirectoryList.h"
#include "EdgeStrip.h"
#include "Euler.h"
#include "Face.h"
#include "FaceGroup.h"
#include "FaceVector.h"
#include "Material.h"
#include "MathUtil.h"
#include "Matrix.h"
#include "PoseTarget.h"
#include "Skin.h"
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
	glm::vec3 v3;
} DummyJoint;

/*! \brief Wraps a (morph) Target object to make it capable of lazy loading.
 *
 * This class encapsulates a Target in order to load it lazily when
 * it is actually used.
 *
 * When an object of this class is created only the filename of the target
 * will be saved.  This enables to load the target when it is actually used
 * at run time.
 */
class TargetEntry
{
public:
	/** \brief The constructor remembers the filename of the Target
	 *
	 * @param inFilename The filename of the Target to load.
	 * @param inPreload Set this to true if you want to preload the target ie.
	 *                  to skip the lazy load.  If you omit this parameter,
	 *                  it defaults to false ie. lazy loading.
	 */
	TargetEntry(const string &inFilename, bool inPreload = false);
	/// The destructor destroys all encapsulated members, including the Target
	~TargetEntry();

	/** \brief Get the Target.  If it has not been loaded then load it now.
	 *
	 * @return The Target or NULL if a target with the filename given in the
	 * constructor does not exist.
	 */
	Target *getTarget();

private:
	/** \brief Try to load the target given in the constructor.  Loading is
	 * attempted only once.
	 *
	 *  @return true when the file could be loaded or false otherwise.
	 */
	bool loadFromFile();

	/// Intentionally not implemented
	TargetEntry(const TargetEntry &);
	/// Intentionally not implemented
	TargetEntry &operator=(const TargetEntry &);

	const string *mFilename;
	Target *mTarget;
	bool mTargetLoadTry;
}; // class TargetEntry

/*! \brief Wraps a PoseTarget object to make it capable of lazy loading.
 *
 * This class encapsulates a PoseTarget in order to load it lazily when
 * it is actually used.
 *
 * When an object of this class is created only the filename of the target
 * will be saved.  This enables to load the target when it is actually used
 * at run time.
 */
class PoseEntry
{
public:
	/** \brief The constructor remembers the filename associated with the target.
	 *
	 * @param inFilename The name of the PoseTarget to load.
	 * @param inFullPath The path to inFilename in a format suitable
	 *                  for opendir().
	 * @param inPreload Set this to true if you want to preload the target ie.
	 *                  to skip the lazy load.  If you omit this parameter,
	 *                  it defaults to false ie. lazy loading.
	 */
	PoseEntry(const string &inFilename, const string &inFullPath,
	          bool inPreload = false);

	/// The destructor destroys all encapsulated members, including the
	/// PoseTarget.
	~PoseEntry();

	/** \brief Get the PoseTarget.  If it has not been loaded then load it now.
	 *
	 * @return The PoseTarget or NULL if a target with the filename given in the
	 * constructor does not exist.
	 */
	PoseTarget *getTarget();

private:
	/** \brief Try to load the target given in the constructor.  Loading is
	 * attempted only once.
	 *
	 *  @return true when the file could be loaded or false otherwise.
	 */
	bool loadFromFile();

	/// Intentionally not implemented
	PoseEntry(const PoseEntry &);
	/// Intentionally not implemented
	PoseEntry &operator=(const PoseEntry &);

	const string *mFilename;
	const string *mFullPath;
	PoseTarget *mTarget;
	bool mTargetLoadTry;
}; // class PoseEntry

typedef map<string, TargetEntry *> TargetMap;
typedef map<string, glm::vec3> Centeroid;
typedef map<string, glm::vec3> FormFactor;
typedef map<string, PoseEntry *> PoseMap;
typedef map<string, BodySettings> CharactersMap;

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
private:
	FaceVector facevector;
	// HotspotMap        hotspotmap;
	VertexVector vertexvector_morph; ///< Modified mesh
	VertexVector vertexvector_morph_copy;
	VertexVector vertexvector_morph_only;

	vector<glm::vec3> vertexvector_orginal; ///< Orginal mesh

	// VertexGroup       vgroup;
	BodySettings bodyset;
	TargetMap targetmap;
	MaterialVector materialvector;
	Centeroid centeroid;
	TextureVector texture_vector;
	BodySettings poses; ///< Currently active PoseTargets
	PoseMap posemap;    ///< Possible pose transformations
	CharactersMap charactersmap;
	FaceGroup facegroup; ///< Access to the faces of body parts by name
	                     ///< - not affected by posing or morphing
	Skin skin;
	EdgeStrip edgestrip;
	SmoothVertex smoothvertex;

	/// Save with each vertex to which faces it belongs
	void calcSharedVertices();

	vector<glm::vec3> jointvector;

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
	const Target *getTargetForName(const string &inTargetname);

	void initPoses();

	void applySmooth(const int recursive_level = 1);

	void applySkin();

	void prepareSkeleton();

	void applySkeleton();

	bool IsADummyJoint(SKELETON_JOINT joint, glm::vec3 &v3);

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
	void doPoseRotation(const PoseRotation &pr, float morph_value,
	                    const UsedVertex &modVertex);

	/*! \brief Apply a PoseTranslation target
	 *
	 * PoseTranslation targets are subtargets of PoseTargets.
	 *
	 * This function is used by doPose (const string& target_name, float
	 * morph_value, const UsedVertex& modVertex).
	 *
	 */
	void doPoseTranslation(PoseTranslation &pt, float morph_value,
	                       const UsedVertex &modVertex);

public:
	Mesh();
	~Mesh();

	/*! \brief Get a Ptr to a PoseTarget object which belongs to a given name
	 *
	 * @param inTargetname The name of the Pose Target to get.
	 * @return a pointer to the pose target to get or NULL if this target does not
	 *         exist
	 */
	PoseTarget *getPoseTargetForName(const string &inTargetname) const;

	/** @name Getting pointers to member variables
	 */
	//@{

	vector<glm::vec3> *getJointVector() { return &jointvector; }
	glm::vec3 GetJoint0_Pos() { return getJointVector()->at(0); }
	//@}

	/** @name Getting references to member variables
	 */
	//@{
	/*!
	 * \return a reference to the morphed VertexVector of this Mesh
	 */
	VertexVector &getVertexVectorRef() { return vertexvector_morph; }

	VertexVector &getVertexVectorMorphOnlyRef()
	{
		return vertexvector_morph_only;
	}

	/*!
	 * \return a reference to the FaceVector of this Mesh
	 */
	FaceVector &getFaceVectorRef() { return facevector; }

	/*!
	 * \return a reference to the MaterialVector of this Mesh
	 */
	MaterialVector &getMaterialVectorRef() { return materialvector; }

	/*!
	 * \return a reference to the TargetMap of this Mesh
	 */
	TargetMap &getTargetMapRef() { return targetmap; }

	/*!
	 * \return TODO
	 */
	TextureVector &getTextureVectorRef() { return texture_vector; }

	/*!
	 * \return a reference to the PoseMap of this Mesh
	 */
	PoseMap &getPoseMapRef() { return posemap; }

	/*!
	 * \return a reference to the CharactersMap of this Mesh
	 */
	CharactersMap &getCharactersMapRef() { return charactersmap; }

	/*!
	 * \return a reference to the FaceGroup of the Mesh
	 */
	FaceGroup &getFaceGroupRef() { return facegroup; }

	//@}

	/*!
	 * \return the Mesh's BodySetting
	 */
	const BodySettings &getBodySettings() const { return bodyset; }

	/*!
	 * \return the Mesh's Poses
	 */
	const BodySettings &getPoses() const { return poses; }

	/** @name Loading
	 */
	//@{
	/// Load the Mesh geometry files.
	/*!
	 * \param mesh_filename the file with Vertex data to load
	 * \param faces_filename the file with Face data to load
	 * \return true if files are found
	 * \return false if files aren't found
	 */
	bool loadMeshFactory(const string &mesh_filename,
	                     const string &faces_filename);

	/// Load the Material files.
	/*!
	 * \param material_filename the file with Material data to load
	 * \param face_colors_filename the file with Face Color data to load
	 * \return true if files are found
	 * \return false if files aren't found
	 */
	bool loadMaterialFactory(const string &material_filename,
	                         const string &face_colors_filename);

	/// Load all (pose) Targets recursively from a directory.
	/*!
	 * \param target_root_path the root path with targets to load
	 * \param recursive_level Set the level of directory recursion. See
	 * DirectoryList for more help. \return true if files are found \return false
	 * if files aren't found
	 */
	void loadTargetsFactory(const string &target_root_path,
	                        int recursive_level = 1, bool preload = false,
	                        bool clearmap = true);

	/// Load all PoseTargets recursively from a directory.
	/*!
	 * \param target_root_path the root path with targets to load
	 * \param recursive_level Set the level of directory recursion. See
	 * DirectoryList for more help. \return true if files are found \return false
	 * if files aren't found
	 */
	void loadPoseTargetsFactory(const string &target_root_path,
	                            int recursive_level = 1);

	/// Load all characters (BodySettings) recursively from a directory.
	/*!
	 * \param characters_root_path the root path with characters to load
	 * \param recursive_level Set the level of directory recursion. See
	 * DirectoryList for more help. \return true if files are found \return false
	 * if files aren't found
	 */
	void loadCharactersFactory(const string &characters_root_path,
	                           int recursive_level = 1);

	/// Load faces groups from file
	bool loadGroupsFactory(const string &groups_filename);

	/// Load skin info from file
	bool loadSkinFactory(const string &filename);

	/// Load edges info from file
	bool loadEdgeStripFactory(const string &filename);

	/// Load smooth info from file
	bool loadSmoothVertexFactory(const string &filename);

	//@}

	void loadSkeleton() { prepareSkeleton(); }

	SKELETON_JOINT getSymmetricJoint(SKELETON_JOINT joint);
	/**** calculate API ****/
	/***********************/

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
	bool doMorph(const string &target_name, float morph_value);

	/*! \brief Fully apply morphs of a BodySettings object
	 *
	 * \param bs a BodySettings object to morph the Mesh
	 * \param clear default is to delete to yet applied targets
	 *        before using a BodySettings. Use 'false' to not clear
	 * the targets before morphing
	 */
	void doMorph(const BodySettings &bs, const bool clear = true);

	/*! \brief Apply morphs of a BodySettings object to a certain degree
	 *
	 * \param bs a BodySettings object to morph the Mesh
	 * \param value of bodysettings application in the range 0..1
	 * \param clear default is to delete to yet applied targets
	 *        before using a BodySettings. Use 'false' to not clear
	 *        the targets before morphing.
	 */
	void doMorph(const BodySettings &bs, float value, bool clear = true);

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
	void doPose(const BodySettings &bs, bool clear = true);

	void doPose(const BodySettings &bs, const float value, bool clear = true);

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
	void doPose(const string &target_name, float morph_value,
	            const UsedVertex &modVertex);

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
	bool setPose(const string &target_name, float morph_value);

	/// Reset the Mesh to loaded state without poses
	// void resetPose (const PoseRotation &target);
	void resetPose();
	//@}

	/// Switch to pose mode
	void poseMode();

	/// Switch to animation mode
	void animationMode();

	/// Switch to body details mode
	void bodyDetailsMode();

}; // class Mesh

} // namespace Animorph

#endif // MESH_H
