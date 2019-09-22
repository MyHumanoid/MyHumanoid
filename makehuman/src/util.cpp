/* SPDX-License-Identifier: GPL-3.0-or-later */
/*
 *  Copyright (C) 2005-2007  MakeHuman Project
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
 *  File   : util.cpp
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  App    : makehuman
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#include "util.h"
#include "CharacterSettingPanel.h"
#include "ComponentID.h"
#include "Global.h"
#include <animorph/DirectoryList.h>
#include <animorph/util.h>
#include <gui/CGUtilities.h>

#ifdef _WIN32
#include <winbase.h>
#include <windows.h>
#else
#include "signal.h"
#include <sys/wait.h>
#endif

#ifndef _WIN32
// Not available on WIN32 'cos still not tested there!
#include "FileTools.h"
#endif

#ifdef _WIN32
extern int errno; // for error return of _spawn
static const string getEXEDir();
#endif

#define USE_NEW_PATH 1

// TODO: Need to test the "/" vs. "\" problematic! Targets...

using namespace std;
// using namespace mhgui;
enum {
	RUGHE_PARAM = 0,

	LAST_PARAM = 9,
};

StringList::iterator sl_it, sl_end;
string used_templates_path;
// editable RIB paramenters
// string parameters

enum {
	RENDPATH_PARAM_STRING = 0,
	TEXTPATH_PARAM_STRING,

	HEADCOLOR_PARAM_STRING,
	BODYCOLOR_PARAM_STRING,
	HEADBUMP_PARAM_STRING,
	BODYBUMP_PARAM_STRING,

	KM_PARAM_STRING,
	VEINS_PARAM_STRING,
	WRINKLES_PARAM_STRING,
	OILVAL_PARAM_STRING,
	SSSSCALE_PARAM_STRING,
	KA_PARAM_STRING,
	COLORR_PARAM_STRING,
	COLORG_PARAM_STRING,
	COLORB_PARAM_STRING,
	DISPLAYX_PARAM_STRING,
	DISPLAYY_PARAM_STRING,

	EYESCOL_PARAM_STRING,
	EYEBCOL_PARAM_STRING,
	EYELCOL_PARAM_STRING,
	HEADSPEC_PARAM_STRING,
	BODYSPEC_PARAM_STRING,

	EYESALPHA_PARAM_STRING,
	EYEBALPHA_PARAM_STRING,
	HEADALPHA_PARAM_STRING,
	BODYALPHA_PARAM_STRING,
	EYELALPHA_PARAM_STRING,
	LAST_PARAM_STRING,
};

//------ end editable parameters section ----------//

string render;
;
string options;
StringList str_list;
int n_step;
int current_step;

static vector<int> process;

const string searchTextureFile(const string &texture_file)
{
	return searchFile(getTexturesAlternatives(texture_file));
}

const string searchShaderFile(const string &shader_file)
{
	return searchFile(getShadersAlternatives(shader_file));
}

const string searchBackgroundFile(const string &data_file)
{
	return searchFile(getBackgroundsAlternatives(data_file));
}

const string searchDataFile(const string &data_file)
{
	string s(searchFile(getDataAlternatives(data_file)));
	if (s.empty()) {
		fprintf(stderr,
		        "*** WARNING in searchDataFile() : attempt to load file '%s'"
		        "' but this could not found in data/!\n",
		        data_file.c_str());
	}
	return s;
}

const string searchPixmapFile(const string &pixmap_file)
{
	string s(searchFile(getPixmapsAlternatives(pixmap_file)));
	if (s.empty()) {
		fprintf(stderr,
		        "*** WARNING in searchPixmapFile() : attempt to load file '%s'"
		        "' but this could not found in pixmaps/!\n",
		        pixmap_file.c_str());
	}
	return s;
}

const string searchDataDir(const string &data_dir)
{
	return searchDir(getDataAlternatives(data_dir));
}

const string searchPixmapDir(const string &pixmap_dir)
{
	return searchDir(getPixmapsAlternatives(pixmap_dir));
}

const StringVector getTexturesAlternatives(const string &texture)
{
	StringVector name_vector;

	name_vector.push_back(getTexturesPath() + texture);

	return name_vector;
}

const StringVector getShadersAlternatives(const string &shader)
{
	StringVector name_vector;

	name_vector.push_back(getShadersPath() + shader);

	return name_vector;
}

const StringVector getPixmapsAlternatives(const string &pixmap)
{
	StringVector name_vector;

	name_vector.push_back("pixmaps" + PATH_SEPARATOR + pixmap);
	name_vector.push_back(".." + PATH_SEPARATOR + "pixmaps" + PATH_SEPARATOR +
	                      pixmap);

#ifdef _WIN32
	name_vector.push_back(getEXEDir());
#else
#ifdef PACKAGE_PIXMAPS_DIR
	name_vector.push_back(string(PACKAGE_PIXMAPS_DIR) + PATH_SEPARATOR + pixmap);
#endif
#endif
	return name_vector;
}

const StringVector getDataAlternatives(const string &data)
{
	StringVector name_vector;

	name_vector.push_back("data" + PATH_SEPARATOR + data);
	name_vector.push_back(".." + PATH_SEPARATOR + "data" + PATH_SEPARATOR + data);

#ifdef _WIN32
	name_vector.push_back(getEXEDir());
#else
#ifdef PACKAGE_PIXMAPS_DIR
	name_vector.push_back(string(PACKAGE_DATA_DIR) + PATH_SEPARATOR + data);
#endif
#endif
	return name_vector;
}

const StringVector getBackgroundsAlternatives(const string &data)
{
	StringVector name_vector;

	name_vector.push_back("backgrounds" + PATH_SEPARATOR + data);
	name_vector.push_back(".." + PATH_SEPARATOR + "backgrounds" + PATH_SEPARATOR +
	                      data);

#ifdef _WIN32
	name_vector.push_back(getEXEDir());
#else
#ifdef PACKAGE_BACKGROUNDS_DIR
	name_vector.push_back(string(PACKAGE_BACKGROUNDS_DIR) + PATH_SEPARATOR +
	                      data);
#endif
#endif
	return name_vector;
}

const string searchFile(const StringVector &name_vector)
{
	struct stat buf;

	for (unsigned int i = 0; i < name_vector.size(); i++) {
		const string &try_name = name_vector[i];
		int result;

		result = stat(try_name.c_str(), &buf);

		if (result != 0) {
			continue;
		} else {
			if (buf.st_mode & S_IFREG)
				return try_name;
		}
	}

	return string();
}

const string searchDir(const StringVector &name_vector)
{
	struct stat buf;

	for (unsigned int i = 0; i < name_vector.size(); i++) {
		const string &try_name = name_vector[i];
		int result;

		result = stat(try_name.c_str(), &buf);

		if (result != 0) {
			continue;
		} else {
			if (buf.st_mode & S_IFDIR)
				return try_name;
		}
	}
	return string();
}

#ifdef _WIN32
static const string getEXEDir()
{
	char path[PATH_MAX];
	char *pos;

	GetModuleFileName(NULL, path, sizeof(path));
	pos = strrchr(path, '\\');

	if (pos)
		*pos = '\0';

	return path;
}
#endif

const string getHomeDir()
{

#ifndef _WIN32
	uid_t uid;
	struct passwd *pass;

	uid = getuid();
	pass = getpwuid(uid);

	return string(string(pass->pw_dir) + PATH_SEPARATOR);
#else  // _WIN32

	HINSTANCE hinstLib;
	MYPROC ProcAdd;
	BOOL fFreeResult, fRunTimeLinkSuccess = FALSE;
	TCHAR szPath[MAX_PATH];

	/* Get a handle to the DLL module. */
	hinstLib = LoadLibrary("shfolder");

	/* If the handle is valid, try to get the function address. */
	if (hinstLib != NULL) {
		ProcAdd = (MYPROC)GetProcAddress(hinstLib, "SHGetFolderPathA");

		/* If the function address is valid, call the function. */
		if (NULL != ProcAdd) {
			fRunTimeLinkSuccess = TRUE;
			ProcAdd(NULL, CSIDL_PERSONAL, /* Test CSIDL_FLAG_CREATE !! */
			        NULL, 0, szPath);
		}

		/* Free the DLL module. */
		fFreeResult = FreeLibrary(hinstLib);
	}

	/* If unable to call the DLL function, use an alternative. */
	if (!fRunTimeLinkSuccess) {
		/* later use getWindir?? or something else as fallback */
		return string("c:\\");
	}

	return string(string(szPath) + "\\");
#endif // _WIN32
}

/* Return the Users Working directory where the load and save dialog should be
   point to. On Linux and Window this will be the same as getHomeDir() but
   on a OS X Box this will point to the Users Desktop because it is not common
   to store files which are supposed for further processing on the users home
   dir. */
const string getUserWorkDir() { return getHomeDir(); }

const string getMyObjPath()
{
	return string(getUserWorkDir() + "makehuman" + PATH_SEPARATOR + "myobjs" +
	              PATH_SEPARATOR);
}

const string getMyColladaPath()
{
	return string(getUserWorkDir() + "makehuman" + PATH_SEPARATOR + "mycollada" +
	              PATH_SEPARATOR);
}

const string getMyPosesPath()
{
	return string(getUserWorkDir() + "makehuman" + PATH_SEPARATOR + "myposes" +
	              PATH_SEPARATOR);
}

const string getMyPosesBasePath()
{
	return string(getUserWorkDir() + "makehuman" + PATH_SEPARATOR + "myposes" +
	              PATH_SEPARATOR);
}

const string getMyBodysettingsPath()
{
	return string(getUserWorkDir() + "makehuman" + PATH_SEPARATOR + "mybs" +
	              PATH_SEPARATOR);
}

const string getMyBodysettingsBasePath()
{
	return string(getUserWorkDir() + "makehuman" + PATH_SEPARATOR + "mybs" +
	              PATH_SEPARATOR);
}

const string getRenderingPath()
{
	return string(getUserWorkDir() + "makehuman" + PATH_SEPARATOR + "rendering" +
	              PATH_SEPARATOR);
}

const string getTexturesPath()
{
	// return string(getUserWorkDir() + "makehuman" + PATH_SEPARATOR + "textures"
	// + PATH_SEPARATOR);
	return string(getUserWorkDir() + "makehuman" + PATH_SEPARATOR + "rendering" +
	              PATH_SEPARATOR);
}

const string getShadersPath()
{
	// return string(getUserWorkDir() + "makehuman" + PATH_SEPARATOR + "shaders" +
	// PATH_SEPARATOR);
	return string(getUserWorkDir() + "makehuman" + PATH_SEPARATOR + "rendering" +
	              PATH_SEPARATOR);
}

const string getMyBackgroundsPath()
{
	return string(getUserWorkDir() + "makehuman" + PATH_SEPARATOR +
	              "mybackgrounds" + PATH_SEPARATOR);
}

void createDirWhenNotExists(const string &inPath)
{
#if defined(_WIN32)
	DIR *dirHd;

	// Does the given dir still exists?
	if ((dirHd = opendir(inPath.c_str())) != NULL) {
		// Yes, then release the handle again and return
		::closedir(dirHd);
		return;
	}
	::mkdir(inPath.c_str()); // create it otherwise (does not exists)!

// On Linux and OS X specific code benefit from the FileTools
#else
	FileTools::makeDirHier(inPath);
#endif
}

void createWorkingDirs()
{
	createDirWhenNotExists(getUserWorkDir() + "makehuman" + PATH_SEPARATOR);
	createDirWhenNotExists(getMyObjPath());
	// createDirWhenNotExists(getMyPosesBasePath());
	createDirWhenNotExists(getMyPosesPath());
	createDirWhenNotExists(getMyPosesPath() + "lib");
	// createDirWhenNotExists(getMyBodysettingsBasePath());
	createDirWhenNotExists(getMyBodysettingsPath());
	createDirWhenNotExists(getMyBodysettingsPath() + "lib");
	createDirWhenNotExists(getRenderingPath());
	// createDirWhenNotExists(getShadersPath());
	// createDirWhenNotExists(getTexturesPath());
	createDirWhenNotExists(getMyBackgroundsPath());
	createDirWhenNotExists(getMyColladaPath());
}

bool GetSymmVertexConfig(int *symm_vertex)
{
	FileReader file_reader;
	file_reader.open("../data/base.sym");

	if (!file_reader)
		return false;

	char buffer[MAX_LINE_BUFFER];
	int sx, dx;

	while (file_reader.getline(buffer, MAX_LINE_BUFFER)) {

		if (sscanf(buffer, "%d,%d", &sx, &dx) == 2) {
			symm_vertex[dx] = sx;
		}
	}
	file_reader.close();
	return true;
}
int getSymmJoint(int joint)
{
	Global &global = g_global;

	Mesh *mesh = global.getMesh();

	return mesh->getSymmetricJoint((SKELETON_JOINT)joint);
}

void CreateWeightsFile()
{
	return;

	FileReader file_reader;
	FileWriter file_write;

	file_write.open("../data/vertex_complete.weights");

	char buffer[MAX_LINE_BUFFER * 4];
	int weight[SK_JOINT_END];
	int actual_weight[SK_JOINT_END];

	Global &global = g_global;

	Mesh *mesh = global.getMesh();

	VertexVector &vertexvector(mesh->getVertexVectorMorphOnlyRef());
	unsigned int size_vx = vertexvector.size();

	int *symm_vertex = new int[size_vx];
	std::ostringstream *out_stream =
	    (std::ostringstream *)new std::ostringstream[size_vx];

	int index;

	file_reader.open("../data/vertex_sx.weights");

	if (!file_reader)
		return;

	for (unsigned int v = 0; v < size_vx; v++) {
		symm_vertex[v] = -1;
	}

	if (GetSymmVertexConfig(&symm_vertex[0]) == false)
		return;

	for (unsigned int i = 0; i < size_vx; i++) {
		memset(buffer, 0, MAX_LINE_BUFFER * 4);
		out_stream[i].str("");

		file_reader.getline(buffer, MAX_LINE_BUFFER * 4);

		out_stream[i] << buffer << endl;
	}

	for (unsigned int j = 0; j < size_vx; j++) {

		if (symm_vertex[j] == -1) { // sx vertex
			file_write << out_stream[j].str().c_str();
		} else {

			int symm = symm_vertex[j];

			memset(weight, 0, sizeof(int) * SK_JOINT_END);
			memset(actual_weight, 0, sizeof(int) * SK_JOINT_END);

			sscanf((char *)out_stream[symm].str().c_str(),
			       "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d "
			       "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d "
			       "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d "
			       "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d "
			       "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d "
			       "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d "
			       "%d %d %d %d %d %d %d %d %d %d",
			       &index, &weight[0], &index, &weight[1], &index, &weight[2], &index,
			       &weight[3], &index, &weight[4], &index, &weight[5], &index,
			       &weight[6], &index, &weight[7], &index, &weight[8], &index,
			       &weight[9], &index, &weight[10], &index, &weight[11], &index,
			       &weight[12], &index, &weight[13], &index, &weight[14], &index,
			       &weight[15], &index, &weight[16], &index, &weight[17], &index,
			       &weight[18], &index, &weight[19], &index, &weight[20], &index,
			       &weight[21], &index, &weight[22], &index, &weight[23], &index,
			       &weight[24], &index, &weight[25], &index, &weight[26], &index,
			       &weight[27], &index, &weight[28], &index, &weight[29], &index,
			       &weight[30], &index, &weight[31], &index, &weight[32], &index,
			       &weight[33], &index, &weight[34], &index, &weight[35], &index,
			       &weight[36], &index, &weight[37], &index, &weight[38], &index,
			       &weight[39], &index, &weight[40], &index, &weight[41], &index,
			       &weight[42], &index, &weight[43], &index, &weight[44], &index,
			       &weight[45], &index, &weight[46], &index, &weight[47], &index,
			       &weight[48], &index, &weight[49], &index, &weight[50], &index,
			       &weight[51], &index, &weight[52], &index, &weight[53], &index,
			       &weight[54], &index, &weight[55], &index, &weight[56], &index,
			       &weight[57], &index, &weight[58], &index, &weight[59], &index,
			       &weight[60], &index, &weight[61], &index, &weight[62], &index,
			       &weight[63], &index, &weight[64], &index, &weight[65], &index,
			       &weight[66], &index, &weight[67]);

			for (int joint = 0; joint < SK_JOINT_END; joint++) {
				int symm_joint = getSymmJoint(joint);
				actual_weight[joint] = weight[symm_joint];
				file_write << joint << " ";
				file_write << actual_weight[joint] << " ";
			}
			file_write << endl;
		}
	}
	file_write.close();
	file_reader.close();
}

void loadDefaultBodySettings()
{
	Global &global = g_global;
	Mesh *mesh = global.getMesh();
	assert(mesh);
	Window &mainWindow = *g_mainWindow;

	FaceGroup &clothesgroup(mesh->getClothesGroupRef());

	BodySettings bodyset;
	bool state = bodyset.load(searchDataFile("default.bs"));
	if (state) {
		state = clothesgroup.loadVisibilities(searchDataFile("default.bs"));
	}

	if (state) {
		global.resetFuzzyValues();
		state = loadSelectorsPositions(searchDataFile("default.bs"));

		CharacterSettingPanel *tmpPanel =
		    (CharacterSettingPanel *)mainWindow.getPanel(
		        kComponentID_CharacterSettingPanel);
		if (tmpPanel != NULL) {
			tmpPanel->calcSelectorValues(kComponentID_CharacterSettingPanel_Age);
			tmpPanel->calcSelectorValues(kComponentID_CharacterSettingPanel_Breast);
			tmpPanel->calcSelectorValues(
			    kComponentID_CharacterSettingPanel_MuscleSize);
			tmpPanel->calcSelectorValues(kComponentID_CharacterSettingPanel_Shape);
		}
	}

	if (state) {
		mesh->doMorph(bodyset);
		mesh->calcNormals();

		if (global.getSubdivision()) {
			mesh->calcSubsurf();
		}
	}
}

bool loadSelectorsPositions(const std::string &filename)
{
	Global &global = g_global;
	char buffer[MAX_LINE];
	char tmp[MAX_LINE];
	char tmp1[MAX_LINE];
	uint32_t id;
	int x, y;
	FileReader file_reader;

	file_reader.open(filename);

	if (!file_reader)
		return false;

	while (file_reader.getline(buffer, MAX_LINE)) {
		if (sscanf(buffer, "#t,%*c,%u,%i,%i", &id, &x, &y) == 3) {
			global.setFuzzyValue(id, Point(x, y));
		}
	}

	return true;
}

bool loadSelectorsPositions(const std::vector<string> &strings,
                            const float value)
{
	Global &global = g_global;
	char buffer[MAX_LINE];
	char tmp[MAX_LINE];
	char tmp1[MAX_LINE];
	uint32_t id;
	int x, y;

	for (vector<string>::const_iterator it = strings.begin(); it != strings.end();
	     it++) {
		if (sscanf((*it).c_str(), "#t,%*c,%u,%i,%i", &id, &x, &y) == 3) {
			global.setFuzzyValue(id,
			                     Point((int32_t)(x * value), (int32_t)(y * value)));
		}
	}

	return true;
}

bool saveSelectorsPositions(const std::string &filename,
                            std::ios_base::openmode mode)
{
	Global &global = g_global;
	FileWriter file_writer;
	file_writer.open(filename, mode);

	if (!file_writer)
		return false;

	std::ostringstream out_stream;

	Point *tmp = global.getFuzzyValue(kComponentID_CharacterSettingPanel_Age);
	if (tmp != NULL) {
		out_stream << "#t,A," << kComponentID_CharacterSettingPanel_Age << ","
		           << tmp->getX() << "," << tmp->getY() << endl;
	}

	tmp = global.getFuzzyValue(kComponentID_CharacterSettingPanel_MuscleSize);
	if (tmp != NULL) {
		out_stream << "#t,M," << kComponentID_CharacterSettingPanel_MuscleSize
		           << "," << tmp->getX() << "," << tmp->getY() << endl;
	}

	tmp = global.getFuzzyValue(kComponentID_CharacterSettingPanel_Breast);
	if (tmp != NULL) {
		out_stream << "#t,B," << kComponentID_CharacterSettingPanel_Breast << ","
		           << tmp->getX() << "," << tmp->getY() << endl;
	}

	tmp = global.getFuzzyValue(kComponentID_CharacterSettingPanel_Shape);
	if (tmp != NULL) {
		out_stream << "#t,S," << kComponentID_CharacterSettingPanel_Shape << ","
		           << tmp->getX() << "," << tmp->getY();
	}

	file_writer << out_stream.str();

	return true;
}
