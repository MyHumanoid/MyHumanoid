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
#include "ConsoleCommands.h"
#include "Global.h"
#include <animorph/DirectoryList.h>
#include <animorph/RIBExporter.h>
#include <animorph/util.h>
#include <animorph/xmlParser.h>
#include <gui/CGUtilities.h>
#include <gui/Console.h>

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

#if defined(_WIN32)
//    const string aqsis_base_path         ("");
//    const string aqsis_bin               ("");
#ifdef USE_NEW_PATH
const string aqsis_base_path("C:/Program Files/Aqsis/");
const string aqsis_bin("bin/");

const string pixie_base_path("C:/Program Files/Pixie/");
//    const string pixie_bin_path          (pixie_base_path + "bin/");
const string pixie_bin("bin/");
#else //  !USE_NEW_PATH
const string aqsis_base_path("");
const string aqsis_bin("");

const string pixie_base_path("");
//    const string pixie_bin_path          (pixie_base_path + "bin/");
const string pixie_bin("");

#endif // #ifdef USE_NEW_PATH

#else // neither WIN32 nor Mac OS X -> Linux!
const string aqsis_base_path("");
const string aqsis_bin("");

const string pixie_base_path("");
const string pixie_bin("");

#endif

#ifdef _WIN32
const static string pixie_render("rndr.exe");
const static string pixie_compiler("texmake.exe");
const static string pixie_shader_compiler("sdrc.exe");

const static string aqsis_render("aqsis.exe");
const static string aqsis_compiler("teqser.exe");
const static string aqsis_shader_compiler("aqsl.exe");
#else
const static string pixie_render("rndr");
const static string pixie_compiler("texmake");
const static string pixie_shader_compiler("sdrc");

const static string aqsis_render("aqsis");
const static string aqsis_compiler("teqser");
const static string aqsis_shader_compiler("aqsl");
#endif

// Please PAY ATTENTION:
// in the current implementation, the MultiProcess implementation
//(under Linux) of execute command will fail if the number of parameters
// is > 1. Currently it is implemented only in Rendering command
// For further development: use the exec function with a list of command:
// the exec("aqsis","aqsis","-v=2 -progress","/filename"); Doesn't work
// you need: exec("aqsis","aqsis","-v=2","-progress","/filename");
// It is different under WIN, you can use " -v=2 -progress "

// pixie options
const static string pixie_version_opt(" -h ");
const static string pixie_compiler_opt(" ");
const static string pixie_shader_compiler_opt(" -o ");
#ifdef _WIN32
const static string pixie_render_opt(" -p -t ");
#else
const static string pixie_render_opt("-t");   // MUST be only one parameter
#endif
// aqsis options
const static string aqsis_version_opt(" -version ");
const static string aqsis_compiler_opt(
    " -swrap=periodic -twrap=periodic -filter=gaussian -swidth=1 -twidth=1");
const static string aqsis_shader_compiler_opt(" -o ");
#ifdef _WIN32
const static string aqsis_render_opt(" -progress -v=2 ");
#else
const static string aqsis_render_opt("-v=2"); // MUST be only one parameter
#endif
// paths
const static string render_templates_path(searchDataDir("rib_data") +
                                          PATH_SEPARATOR + "rib_templates");
const static string render_templates_preview_path(searchDataDir("rib_data") +
                                                  PATH_SEPARATOR +
                                                  "rib_templates_preview");
const static string render_templates_toon_path(searchDataDir("rib_data") +
                                               PATH_SEPARATOR +
                                               "rib_templates_toon");

const static string aqsis_textures("aqsis");
const static string pixie_textures("pixie");

// TODO: Need to test the "/" vs. "\" problematic! Targets...

using namespace std;
// using namespace mhgui;
enum {
	RUGHE_PARAM = 0,

	LAST_PARAM = 9,
};

StringList::iterator sl_it, sl_end;
string used_templates_path;
list<StringPair> replaceList;
// editable RIB paramenters
#if 0
//float parameters

typedef struct{
  char *name;
  char *proto;
  float value;
}RIB_PARAM_FLOAT;

float Parameters_def[LAST_PARAM] = {
  0.025f,
  0.0f,
  0.0f,
  0.35f,
  0.02f,
  0.01f,
  1.0f,
  0.8f,
  0.8f,
};
RIB_PARAM_FLOAT editParameters[LAST_PARAM] = {
{"Km",      "${KM}",       Parameters_def[0]},
{"Veins",   "${VEINS}",    Parameters_def[1]},
{"Wrinkles","${WRINKLES}", Parameters_def[2]},
{"oilVal",  "${OIL_VAL}",  Parameters_def[3]},
{"sssScale","${SSS_SCALE}",Parameters_def[4]},
{"Ka",      "${KA}",       Parameters_def[5]},
{"ColorR",  "${COLOR_R}",  Parameters_def[6]},
{"ColorG",  "${COLOR_G}",  Parameters_def[7]},
{"ColorB",  "${COLOR_B}",  Parameters_def[8]},
};
static void SetParameter(unsigned int param,float value);
static void ResetParameter();
#endif
// string parameters

typedef struct {
	char *name;
	char *proto;
	string value;
} RIB_PARAM_STRING;

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

string ParametersString_def[LAST_PARAM_STRING] = {
    "default", // I must do a check and force it to ${MESH_PATH}
    "default",

    "head_color.tx",
    "body_color.tx",
    "head_bump.tx",
    "body_bump.tx",

    "0.025",
    "0.0",
    "0.0",
    "0.35",
    "0.02",
    "0.01",
    "1.0",
    "0.8",
    "0.8",
    "800",
    "600",
    "eyes_color.tx",
    "eyebrows_color.tx",
    "eyelashes_color.tx",
    "head_specular.tx",
    "body_specular.tx",

    "eyes_alpha.tx",
    "eyebrows_alpha.tx",
    "head_alpha.tx",
    "body_alpha.tx",
    "eyelashes_alpha.tx",
};

// set headtex, bodytex, headbump, bodybump
RIB_PARAM_STRING editParametersString[LAST_PARAM_STRING] = {
    {"rendpath", "${RENDERING_PATH}",
     ParametersString_def[RENDPATH_PARAM_STRING]},
    {"texpath", "${TEXTURES_PATH}",
     ParametersString_def[TEXTPATH_PARAM_STRING]},

    {"headcol", "${HEAD_TEX}", ParametersString_def[HEADCOLOR_PARAM_STRING]},
    {"bodycol", "${BODY_TEX}", ParametersString_def[BODYCOLOR_PARAM_STRING]},
    {"headbump", "${HEAD_BUMP}", ParametersString_def[HEADBUMP_PARAM_STRING]},
    {"bodybump", "${BODY_BUMP}", ParametersString_def[BODYBUMP_PARAM_STRING]},

    {"Km", "${KM}", ParametersString_def[KM_PARAM_STRING]},
    {"Veins", "${VEINS}", ParametersString_def[VEINS_PARAM_STRING]},
    {"Wrinkles", "${WRINKLES}", ParametersString_def[WRINKLES_PARAM_STRING]},
    {"oilVal", "${OIL_VAL}", ParametersString_def[OILVAL_PARAM_STRING]},
    {"sssScale", "${SSS_SCALE}", ParametersString_def[SSSSCALE_PARAM_STRING]},
    {"Ka", "${KA}", ParametersString_def[KA_PARAM_STRING]},
    {"ColorR", "${COLOR_R}", ParametersString_def[COLORR_PARAM_STRING]},
    {"ColorG", "${COLOR_G}", ParametersString_def[COLORG_PARAM_STRING]},
    {"ColorB", "${COLOR_B}", ParametersString_def[COLORB_PARAM_STRING]},
    {"DisplayX", "${DISPLAY_X}", ParametersString_def[DISPLAYX_PARAM_STRING]},
    {"DisplayY", "${DISPLAY_Y}", ParametersString_def[DISPLAYY_PARAM_STRING]},

    {"eyescol", "${EYES_COL}", ParametersString_def[EYESCOL_PARAM_STRING]},
    {"eyebcol", "${EYEB_COL}", ParametersString_def[EYEBCOL_PARAM_STRING]},
    {"eyelcol", "${EYEL_COL}", ParametersString_def[EYELCOL_PARAM_STRING]},
    {"headspec", "${HEAD_SPEC}", ParametersString_def[HEADSPEC_PARAM_STRING]},
    {"bodyspec", "${BODY_SPEC}", ParametersString_def[BODYSPEC_PARAM_STRING]},

    {"eyesalpha", "${EYES_ALPHA}",
     ParametersString_def[EYESALPHA_PARAM_STRING]},
    {"eyebalpha", "${EYEB_ALPHA}",
     ParametersString_def[EYEBALPHA_PARAM_STRING]},
    {"headalpha", "${HEAD_ALPHA}",
     ParametersString_def[HEADALPHA_PARAM_STRING]},
    {"bodyalpha", "${BODY_ALPHA}",
     ParametersString_def[BODYALPHA_PARAM_STRING]},
    {"eyelalpha", "${EYEL_ALPHA}",
     ParametersString_def[EYELALPHA_PARAM_STRING]},

};
static void ResetParameterString();

//------ end editable parameters section ----------//

string render;
;
string options;
StringList str_list;
int n_step;
int current_step;

static void loadRenderingPaths_Aqsis();
static void loadRenderingPaths_Pixie();
static vector<int> process;

/// aqsis
string g_aqsis_base_path = aqsis_base_path;
const string GetDefaultAqsisPath() { return aqsis_base_path; }
int SetAqsisBasePath(const string &path)
{

	DIR *dirHd = opendir(path.c_str());
	if (dirHd != NULL) {
		// Yes, then release the handle again and return
		::closedir(dirHd);
		FILE *fd;

		if ((fd = fopen((path + aqsis_bin + aqsis_render).c_str(), "r")) == NULL)
			return -2;

		fclose(fd);
		g_aqsis_base_path = path;
		return 0;
	}
	return -1;
}
const string GetAqsisBasePath() { return g_aqsis_base_path; }
const string GetAqsisBinPath() { return (GetAqsisBasePath() + aqsis_bin); }
const string GetAqsisRender() { return (GetAqsisBinPath() + aqsis_render); }
const string GetAqsisCompiler() { return GetAqsisBinPath() + aqsis_compiler; }
const string GetAqsisShaderCompiler()
{
	return GetAqsisBinPath() + aqsis_shader_compiler;
}

const string GetAqsisVersionOpt() { return aqsis_version_opt; }
const string GetAqsisCompilerOpt() { return aqsis_compiler_opt; }
const string GetAqsisShaderOpt() { return aqsis_shader_compiler_opt; }
/// pixie
string g_pixie_base_path = pixie_base_path;
const string GetDefaultPixiePath() { return pixie_base_path; }
int SetPixieBasePath(const string &path)
{

	DIR *dirHd = opendir(path.c_str());
	if (dirHd != NULL) {
		// Yes, then release the handle again and return
		::closedir(dirHd);
		FILE *fd;
		if ((fd = fopen((path + pixie_bin + pixie_render).c_str(), "r")) == NULL)
			return -2;
		fclose(fd);
		g_pixie_base_path = path;
		return 0;
	}
	return -1;
}
const string GetPixieBasePath() { return g_pixie_base_path; }
const string GetPixieBinPath() { return (GetPixieBasePath() + pixie_bin); }
const string GetPixieRender() { return (GetPixieBinPath() + pixie_render); }
const string GetPixieCompiler() { return GetPixieBinPath() + pixie_compiler; }
const string GetPixieShaderCompiler()
{
	return GetPixieBinPath() + pixie_shader_compiler;
}

const string GetPixieVersionOpt() { return pixie_version_opt; }
const string GetPixieCompilerOpt() { return pixie_compiler_opt; }
const string GetPixieShaderOpt() { return pixie_shader_compiler_opt; }

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
	// createDirWhenNotExists(getTexturesPath() + "aqsis");
	// createDirWhenNotExists(getTexturesPath() + "pixie");
	createDirWhenNotExists(getMyBackgroundsPath());
	createDirWhenNotExists(getMyColladaPath());
}

bool callRender(const string &cmd, const string &opt, const string &file_rib,
                Window &mainWindow)
{
	/*
	  if(std::system(cmd.c_str()) != 0)
	  {
	    cout << "Error: " << cmd << endl;
	    mainWindow.getConsole()->open();
	    mainWindow.getConsole()->printMessage(kConsoleMessage_RenderError);
	    return false;
	  }
	  else
	  {
	    return true;
	  }"C:\\Programmi\\Pixie\\bin\\rndr.exe"
	*/
	bool ret = ExecuteCommand_WFile(MT, cmd, opt, file_rib);

	if (ret < 0) {

		string local_cmd;
		if (cmd.find(aqsis_render))
			local_cmd = aqsis_render + " " + file_rib;
		else
			local_cmd = pixie_render + " " + opt + " " + file_rib;

		if (std::system(local_cmd.c_str()) != 0) {
			cout << "Error: " << cmd << endl;
			mainWindow.getConsole()->open();
			mainWindow.getConsole()->printMessage(kConsoleMessage_RenderError);
			return false;
		} else {
			return true;
		}
	} else {
		return true;
	}
}

bool compileShaders(const string &shader_compiler, const string &options,
                    Window &mainWindow)
{
	vector<string> shaders;

	DirectoryList dir_list;
	string shaders_root_path(searchDataDir("rib_data") + PATH_SEPARATOR +
	                         "shaders_data" + PATH_SEPARATOR);
	dir_list.setRootPath(shaders_root_path);
	dir_list.setRecursive(0);
	dir_list.setFileFilter(".sl");

	string compiled_extension =
	    (shader_compiler == GetPixieShaderCompiler() ? ".sdr" : ".slx");

	const StringList &str_list(dir_list.getDirectoryList());

	// bool must_compile = false;
	string shaders_dir = getShadersPath();

	for (StringList::const_iterator sl_it = str_list.begin();
	     sl_it != str_list.end(); sl_it++) {
		const string &file(*sl_it);
		string shader_name(file);

		shader_name.erase(0, shaders_root_path.length() + 1);
		shader_name.erase(shader_name.length() - 3, shader_name.length());
		shaders.push_back(shader_name);
	}

	for (vector<string>::iterator s_it = shaders.begin(); s_it != shaders.end();
	     s_it++) {
		string cmd(" \"" + getShadersPath() + (*s_it) + compiled_extension +
		           "\" \"" + searchDataDir("rib_data") + PATH_SEPARATOR +
		           "shaders_data" + PATH_SEPARATOR + (*s_it) + ".sl\"");

		if (ExecuteCommand(NMT, shader_compiler, options + cmd) == false) {

			cout << "Error: " << cmd << endl;

			mainWindow.getConsole()->open();
			mainWindow.getConsole()->printMessage(kConsoleMessage_ShaderCompileError);
			return false;
		}
	}

	return true;
}

bool compileTextures(const string &textures_dir, const string &texture_compiler,
                     const string &options, Window &mainWindow)
{
	cout << "entering texture" << endl;
	vector<string> textures;

	DirectoryList dir_list;
	string texture_root_path(searchDataDir("rib_data") + PATH_SEPARATOR +
	                         "textures_data" + PATH_SEPARATOR);
	dir_list.setRootPath(texture_root_path);
	dir_list.setRecursive(0);
	dir_list.setFileFilter(".tif");

	const StringList &str_list(dir_list.getDirectoryList());

	// bool must_compile = false;
	for (StringList::const_iterator sl_it = str_list.begin();
	     sl_it != str_list.end(); sl_it++) {
		const string &file(*sl_it);
		string texture_name(file);

		texture_name.erase(0, texture_root_path.length() + 1);
		texture_name.erase(texture_name.length() - 4, texture_name.length());

		textures.push_back(texture_name);
	}

	for (vector<string>::iterator s_it = textures.begin(); s_it != textures.end();
	     s_it++) {
		string cmd(" \"" + searchDataDir("rib_data") + PATH_SEPARATOR +
		           "textures_data" + PATH_SEPARATOR + (*s_it) + ".tif\" \"" +
		           getTexturesPath() /*+ textures_dir + PATH_SEPARATOR*/ + (*s_it) +
		           ".tx\"");

		if (ExecuteCommand(NMT, texture_compiler, options + cmd) == false) {
			cout << "Error: " << cmd << endl;

			mainWindow.getConsole()->open();
			mainWindow.getConsole()->printMessage(
			    kConsoleMessage_TextureCompileError);
			return false;
		}
	}

	return true;
}

void rendering(Window &mainWindow, const RenderType type)
{

	KillProcessList();

	Global &global = Global::instance();

	Mesh *mesh = global.getMesh();
	assert(mesh);
	Camera *camera = global.getCamera();
	assert(camera);

	bool error = false;
	string texture_compiler;
	string textures_dir;
	string shader_compiler;
	string texture_opt;
	string shader_opt;

	RIBExporter ribExporter(*mesh);

	if (ExecuteCommand(NMT, GetAqsisRender(), GetAqsisVersionOpt()) == true) {

		render = GetAqsisRender();
		options = aqsis_render_opt;

		if (type == NORMAL || type == TOON) {
			texture_compiler = GetAqsisCompiler();
			texture_opt = GetAqsisCompilerOpt();
			textures_dir = aqsis_textures;
			shader_compiler = GetAqsisShaderCompiler();
			shader_opt = GetAqsisShaderOpt();
		}
	} else if (ExecuteCommand(NMT, GetPixieRender(), GetPixieVersionOpt()) ==
	           true) {
		render = GetPixieRender();
		options = pixie_render_opt;

		if (type == NORMAL || type == TOON) {
			texture_compiler = GetPixieCompiler();
			texture_opt = GetPixieCompilerOpt();
			textures_dir = pixie_textures;
			shader_compiler = GetPixieShaderCompiler();
			shader_opt = GetPixieShaderOpt();
		}
	} else {
		cout << "Unable to find a render engine" << endl;
		mainWindow.getConsole()->open();
		mainWindow.getConsole()->printMessage(kConsoleMessage_RenderEngineNotFound);
		error = true;
	}

	if (!error) {
		if (type == PREVIEW || compileTextures(textures_dir, texture_compiler,
		                                       texture_opt, mainWindow)) {
			if (type == PREVIEW ||
			    compileShaders(shader_compiler, shader_opt, mainWindow)) {

				// export mesh
				ribExporter.exportFile(getRenderingPath() + "Base");

				// create render RIB
				// list <StringPair> replaceList;
				replaceList.clear();

				Vector3f position = camera->getPosition();
				float angleX = camera->getAngleX();
				float angleY = camera->getAngleY();

				ostringstream oss;

				StringPair strMeshPath("${MESH_PATH}", getRenderingPath());
				if (type == NORMAL || type == TOON) {
					StringPair strShadersPath("${SHADERS_PATH}", getShadersPath());
					//          StringPair strTexturesPath ("${TEXTURES_PATH}",
					//          getTexturesPath() /*+ textures_dir*/ + PATH_SEPARATOR);

					replaceList.push_back(strShadersPath);
					//          replaceList.push_back (strTexturesPath);

					if (!strcasecmp(
					        editParametersString[TEXTPATH_PARAM_STRING].value.c_str(),
					        "default"))
						oss << (getTexturesPath() /*+ textures_dir*/ + PATH_SEPARATOR);
					else
						oss << editParametersString[TEXTPATH_PARAM_STRING].value;

					StringPair strTexturesPath(
					    editParametersString[TEXTPATH_PARAM_STRING].proto, oss.str());
					replaceList.push_back(strTexturesPath);
				}
				oss.str("");
				oss << -position.x;
				StringPair strTranslateX("${TRANSLATE_X}", oss.str());

				oss.str("");
				oss << -position.z;
				StringPair strTranslateY("${TRANSLATE_Y}", oss.str());

				oss.str("");
				oss << position.y;
				StringPair strTranslateZ("${TRANSLATE_Z}", oss.str());

				oss.str("");
				oss << rad2deg(M_PI / 2 + angleX);
				StringPair strRotateX("${ROTATE_X}", oss.str());

				oss.str("");
				oss << rad2deg(-angleY);
				StringPair strRotateZ("${ROTATE_Y}", oss.str());

				replaceList.push_back(strMeshPath);
				replaceList.push_back(strTranslateX);
				replaceList.push_back(strTranslateY);
				replaceList.push_back(strTranslateZ);
				replaceList.push_back(strRotateX);
				replaceList.push_back(strRotateZ);

				//----------------RIB PARAMETERS -----------------//
				// string
				oss.str("");

				if (!strcasecmp(
				        editParametersString[RENDPATH_PARAM_STRING].value.c_str(),
				        "default"))
					oss << getRenderingPath();
				else
					oss << editParametersString[RENDPATH_PARAM_STRING].value;

				StringPair strParam(editParametersString[RENDPATH_PARAM_STRING].proto,
				                    oss.str());
				replaceList.push_back(strParam);

				for (int i = HEADCOLOR_PARAM_STRING; i < LAST_PARAM_STRING; i++) {

					StringPair strParam(editParametersString[i].proto,
					                    editParametersString[i].value.c_str());
					replaceList.push_back(strParam);
				}
				//------------------------------------------------//

				switch (type) {
				case NORMAL:
					used_templates_path = render_templates_path;
					break;
				case PREVIEW:
					used_templates_path = render_templates_preview_path;
					break;
				case TOON:
					used_templates_path = render_templates_toon_path;
					break;
				}

				DirectoryList dir_list;
				dir_list.setRootPath(used_templates_path);
				dir_list.setRecursive(0);
				dir_list.setFileFilter(".template");

				str_list = dir_list.getDirectoryList();
				n_step = str_list.size();
				current_step = 0;
				// const StringList &str_list(dir_list.getDirectoryList ());
				sl_it = str_list.begin();
				sl_end = str_list.end();
				Global::instance().setRendering(true);

				std::stringstream ss;
				std::string str;
				ss << n_step;
				ss >> str;

				mainWindow.getConsole()->open();
				mainWindow.getConsole()->printMessage("Rendering Started");
				cgutils::redisplay();
			}
		}
	}
}

void renderingStep()
{

	if (sl_it != sl_end) {
		current_step++;

		Global &global = Global::instance();
		Window &mainWindow = Window::instance();

		Mesh *mesh = global.getMesh();
		assert(mesh);

		RIBExporter ribExporter(*mesh);

		const string &file(*sl_it);
		string template_name(file);

		template_name.erase(0, used_templates_path.length() + 1);

		string out_file = cutFileEnding(template_name, ".template");
		ribExporter.exportFile(used_templates_path, template_name,
		                       getRenderingPath() + out_file, replaceList);

//   callRender(render + " \"" + getRenderingPath() + out_file + "\"",
//   mainWindow); callRender(render ,options,"\"" + getRenderingPath() +
//   out_file + "\"", mainWindow);
#ifdef _WIN32
		callRender(render, options, " \"" + getRenderingPath() + out_file + "\"",
		           mainWindow);
#else
		callRender(render, options, getRenderingPath() + out_file, mainWindow);
#endif
		sl_it++;

		if (current_step < n_step) {
			std::stringstream ss;
			std::stringstream ss2;
			std::string str;
			std::string str2;

			ss << (current_step + 1);
			ss >> str;

			ss2 << n_step;
			ss2 >> str2;

			mainWindow.getConsole()->open();
			mainWindow.getConsole()->printMessage("Rendering Started");
			cgutils::redisplay();
		}
	} else {
		Global::instance().setRendering(false);

		Window::instance().getConsole()->close();
		cgutils::redisplay();
	}
}
void loadRenderingPaths()
{
	loadRenderingPaths_Aqsis();
	loadRenderingPaths_Pixie();
}

void loadRenderingPaths_Aqsis()
{
	char str[MAX_LINE_BUFFER];

	string file = getRenderingPath() + "aqsis_setting.rps";

	FILE *fd = fopen(file.c_str(), "r");

	if (fd == NULL) {
		return;
	}
	fgets(str, MAX_LINE_BUFFER, fd);

	if (str == NULL) // end of file reached?
		return;

	string path_str = str;

	SetAqsisBasePath(path_str);

	fclose(fd);
}

void loadRenderingPaths_Pixie()
{
	char str[MAX_LINE_BUFFER];

	string file = getRenderingPath() + "pixie_setting.rps";

	FILE *fd = fopen(file.c_str(), "r");

	if (fd == NULL) {
		return;
	}
	fgets(str, MAX_LINE_BUFFER, fd);

	if (str == NULL) // end of file reached?
		return;

	string path_str = str;

	SetPixieBasePath(path_str);

	fclose(fd);
}

void saveRenderingPaths(string path, int RENDER_TYPE)
{
	/*
	  string file ;

	  switch(RENDER_TYPE){
	    case AQSIS:
	      file =   getRenderingPath() + "aqsis_setting.rps";
	      break;
	    case PIXIE:
	      file =   getRenderingPath() + "pixie_setting.rps";
	      break;
	    default:
	      return;
	  }


	  FILE *fd = fopen(file.c_str(),"w");

	  if (fd ==NULL ){
	    return ;
	  }

	  fwrite(path.c_str(),path.length(),1,fd);

	  fclose(fd);
	*/
}

int ParseParameter(string cmd)
{
	char param[MAX_LINE_BUFFER];
	char string_val[MAX_LINE_BUFFER];

	if (cmd.length() >= MAX_LINE_BUFFER) // safety
		return -1;                         // error

	if (sscanf(cmd.c_str(), "%s %s", &param[0], &string_val[0]) == 2) {
		return ParseStringRIBParameter(param, string_val);
	}

	if (sscanf(cmd.c_str(), "%s", &param[0]) != 1) {
		return -1; // error
	} else {
		string str(param);

		if (!strcasecmp(str.c_str(), "default")) {
			ResetParameterString();
			return 0;
		}
	}

	return -2; // not found
}

void ResetParameterString()
{
	for (int i = 0; i < LAST_PARAM_STRING; i++) {
		editParametersString[i].value = ParametersString_def[i];
	}
}
int ParseStringRIBParameter(char *param, char *value)
{
	Global &global = Global::instance();

	string parameter(param);
	string str_value(value);

	if (!strcasecmp(parameter.c_str(),
	                editParametersString[RENDPATH_PARAM_STRING].name)) {

		DIR *dirHd = opendir(str_value.c_str());
		if (dirHd != NULL) {
			// Yes, then release the handle again and return
			::closedir(dirHd);
			editParametersString[RENDPATH_PARAM_STRING].value =
			    str_value + PATH_SEPARATOR;
			return 0;
		}
		return -3; // path not found!
	}

	for (int i = HEADCOLOR_PARAM_STRING; i < LAST_PARAM_STRING; i++) {

		if (!strcasecmp(parameter.c_str(), editParametersString[i].name)) {

			editParametersString[i].value = str_value;
			return 0;
		}
	}

	if (!strcasecmp(parameter.c_str(),
	                editParametersString[TEXTPATH_PARAM_STRING].name)) {

		DIR *dirHd = opendir(str_value.c_str());
		if (dirHd != NULL) {
			// Yes, then release the handle again and return
			::closedir(dirHd);
			editParametersString[TEXTPATH_PARAM_STRING].value =
			    str_value + PATH_SEPARATOR;
			return 0;
		}
		return -3; // path not found!
	}

	if (!strcasecmp(parameter.c_str(), "exp_controller")) {

		if (str_value == "true") {
			global.setExportMode(WITH_CONTROLLER);
			return 0;
		} else if (str_value == "false") {
			global.setExportMode(WITHOUT_CONTROLLER);
			return 0;
		} else {
			return -2;
		}
	}
	return -2;
}

bool ExecuteCommand(int multithreading, const string &command,
                    const string &opt)
{
#ifdef _WIN32

	int option = 0;
	switch (multithreading) {
	case NMT:
		option = _P_WAIT;
		break;
	default:
		option = _P_NOWAIT;
		break;
	}

	int ret = _spawnl(option, command.c_str(), opt.c_str(), NULL);

	if (ret < 0)
		return false;

	if (multithreading == MT)
		process.push_back(ret);

	return true;

#else
	int ret = 0;

	if (multithreading == NMT) {
		cout << "exec " << command.c_str() << opt.c_str() << endl;
		ret = std::system((command + opt).c_str());

		return (ret == 0);
	}

	pid_t pID = fork();
	if (pID == 0) // child
	{
		// Code only executed by child process
		cout << "fork " << command.c_str() << opt.c_str() << endl;

		execlp(command.c_str(), command.c_str(), opt.c_str(), NULL);

		_exit(0);
	} else if (pID < 0) // failed to fork
	{
		cerr << "Failed to fork" << endl;
		return false;
		// Throw exception
	} else // parent
	{
		process.push_back((int)pID);
		cout << "   Push Back " << pID << endl;
		return true;
	}
#endif
}

bool ExecuteCommand_WFile(int multithreading, const string &command,
                          const string &opt, const string &file)
{
#ifdef _WIN32

	int option = 0;
	switch (multithreading) {
	case NMT:
		option = _P_WAIT;
		break;
	default:
		option = _P_NOWAIT;
		break;
	}

	int ret = _spawnl(option, command.c_str(), opt.c_str(), file.c_str(), NULL);

	if (ret < 0)
		return false;

	if (multithreading == MT)
		process.push_back(ret);

	return true;

#else
	int ret = 0;

	if (multithreading == NMT) {
		cout << "exec " << command.c_str() << opt.c_str() << endl;
		ret = std::system((command + opt).c_str());

		return (ret == 0);
	}

	pid_t pID = fork();
	if (pID == 0) // child
	{
		// Code only executed by child process
		cout << "fork " << command.c_str() << opt.c_str() << file.c_str() << endl;

		execlp(command.c_str(), command.c_str(), opt.c_str(), file.c_str(), NULL);

		_exit(0);
	} else if (pID < 0) // failed to fork
	{
		cerr << "Failed to fork" << endl;
		return false;
		// Throw exception
	} else // parent
	{
		process.push_back((int)pID);
		cout << "   Push Back " << pID << endl;
		return true;
	}
#endif
}
int KillProcessList()
{
	int killed = 0;
	for (std::vector<int>::iterator process_it = process.begin();
	     process_it != process.end(); process_it++) {
#ifdef _WIN32
		if (TerminateProcess((HANDLE)*process_it, 0) != 0)
			killed++;
	}
#else
		pid_t nRet;
		int status;
		kill((pid_t)*process_it, SIGTERM);
		nRet = waitpid((pid_t)*process_it, &status, WNOHANG); // Check specified
		if (nRet >= 0)
			killed++;
	}

#endif

	process.clear();
	return killed;
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
	Global &global = Global::instance();

	Mesh *mesh = global.getMesh();

	return mesh->getSymmetricJoint((SKELETON_JOINT)joint);
}

void CreateWeightsFile()
{
	ExportConfigurationXML();
	return;

	FileReader file_reader;
	FileWriter file_write;

	file_write.open("../data/vertex_complete.weights");

	char buffer[MAX_LINE_BUFFER * 4];
	int weight[SK_JOINT_END];
	int actual_weight[SK_JOINT_END];

	Global &global = Global::instance();

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

void ExportConfigurationXML()
{

	XMLNode xMainNode;
	Global &global = Global::instance();

	xMainNode = XMLNode::createXMLTopNode("xml", TRUE);
	xMainNode.addAttribute("version", "1.0");
	xMainNode.addAttribute("encoding", "utf-8");

	XMLNode xNode_makehuman = xMainNode.addChild("MAKEHUMAN");

	XMLNode xNode_rendering_parameters, xNode_collada_parameters,
	    xNode_aqsis_path, xNode_pixie_path, xNode_rendering_path,
	    xNode_texture_path;

	XMLNode xNode_head_texture, xNode_body_texture, xNode_head_bump,
	    xNode_body_bump;

	XMLNode xNode_km, xNode_Veins, xNode_Wrinkles, xNode_oilVal, xNode_sssScale,
	    xNode_Ka, xNode_ColorR, xNode_ColorG, xNode_ColorB;
	XMLNode xNode_DisplayX, xNode_DisplayY;

	XMLNode xNode_eyescol, xNode_eyelcol, xNode_eyebcol, xNode_headspec,
	    xNode_bodyspec;

	xNode_rendering_parameters = xNode_makehuman.addChild("rendering_parameters");

	// aqsis path  //aqs
	xNode_aqsis_path = xNode_rendering_parameters.addChild("aqsis_path");
	xNode_aqsis_path.addAttribute("command", "aqs");
	xNode_aqsis_path.addText(GetAqsisBasePath().c_str());

	// pixie path  //pix
	xNode_pixie_path = xNode_rendering_parameters.addChild("pixie_path");
	xNode_pixie_path.addAttribute("command", "pix");
	xNode_pixie_path.addText(GetPixieBasePath().c_str());

	// rendering path  //
	xNode_rendering_path = xNode_rendering_parameters.addChild("rendering_path");
	xNode_rendering_path.addAttribute("command", "set rendpath");
	xNode_rendering_path.addText(
	    editParametersString[RENDPATH_PARAM_STRING].value.c_str());

	// TEXTURES_PATH path  //
	xNode_texture_path = xNode_rendering_parameters.addChild("texture_path");
	xNode_texture_path.addAttribute("command", "set texpath");
	xNode_texture_path.addText(
	    editParametersString[TEXTPATH_PARAM_STRING].value.c_str());

	// head_texture  //
	xNode_head_texture = xNode_rendering_parameters.addChild("head_color");
	xNode_head_texture.addAttribute("command", "set headcol");
	xNode_head_texture.addText(
	    editParametersString[HEADCOLOR_PARAM_STRING].value.c_str());

	// body texture  //
	xNode_body_texture = xNode_rendering_parameters.addChild("body_color");
	xNode_body_texture.addAttribute("command", "set bodycol");
	xNode_body_texture.addText(
	    editParametersString[BODYCOLOR_PARAM_STRING].value.c_str());

	// head bump //
	xNode_head_bump = xNode_rendering_parameters.addChild("head_bump");
	xNode_head_bump.addAttribute("command", "set headbump");
	xNode_head_bump.addText(
	    editParametersString[HEADBUMP_PARAM_STRING].value.c_str());

	// body bump //
	xNode_body_bump = xNode_rendering_parameters.addChild("body_bump");
	xNode_body_bump.addAttribute("command", "set bodybump");
	xNode_body_bump.addText(
	    editParametersString[BODYBUMP_PARAM_STRING].value.c_str());

	// eyescol
	xNode_eyescol = xNode_rendering_parameters.addChild("eyes_color");
	xNode_eyescol.addAttribute("command", "set eyescol");
	xNode_eyescol.addText(
	    editParametersString[EYESCOL_PARAM_STRING].value.c_str());

	// eyebcol
	xNode_eyebcol = xNode_rendering_parameters.addChild("eyebrows_color");
	xNode_eyebcol.addAttribute("command", "set eyebcol");
	xNode_eyebcol.addText(
	    editParametersString[EYEBCOL_PARAM_STRING].value.c_str());

	// eyelcol
	xNode_eyelcol = xNode_rendering_parameters.addChild("eyelashes_color");
	xNode_eyelcol.addAttribute("command", "set eyelcol");
	xNode_eyelcol.addText(
	    editParametersString[EYELCOL_PARAM_STRING].value.c_str());

	// headspec
	xNode_headspec = xNode_rendering_parameters.addChild("head_specular");
	xNode_headspec.addAttribute("command", "set headspec");
	xNode_headspec.addText(
	    editParametersString[HEADSPEC_PARAM_STRING].value.c_str());

	// bodyspec
	xNode_bodyspec = xNode_rendering_parameters.addChild("body_specular");
	xNode_bodyspec.addAttribute("command", "set bodyspec");
	xNode_bodyspec.addText(
	    editParametersString[BODYSPEC_PARAM_STRING].value.c_str());

	XMLNode xNode_eyesalpha, xNode_eyebalpha, xNode_eyelalpha, xNode_headalpha,
	    xNode_bodyalpha;

	// eyesalpha
	xNode_eyesalpha = xNode_rendering_parameters.addChild("eyes_alpha");
	xNode_eyesalpha.addAttribute("command", "set eyesalpha");
	xNode_eyesalpha.addText(
	    editParametersString[EYESALPHA_PARAM_STRING].value.c_str());

	// eyebalpha
	xNode_eyebalpha = xNode_rendering_parameters.addChild("eyebrows_alpha");
	xNode_eyebalpha.addAttribute("command", "set eyebalpha");
	xNode_eyebalpha.addText(
	    editParametersString[EYEBALPHA_PARAM_STRING].value.c_str());

	// eyelalpha
	xNode_eyelalpha = xNode_rendering_parameters.addChild("eyelashes_alpha");
	xNode_eyelalpha.addAttribute("command", "set eyelalpha");
	xNode_eyelalpha.addText(
	    editParametersString[EYELALPHA_PARAM_STRING].value.c_str());

	// headalpha
	xNode_headalpha = xNode_rendering_parameters.addChild("head_alpha");
	xNode_headalpha.addAttribute("command", "set headalpha");
	xNode_headalpha.addText(
	    editParametersString[HEADALPHA_PARAM_STRING].value.c_str());

	// bodyalpha
	xNode_bodyalpha = xNode_rendering_parameters.addChild("body_alpha");
	xNode_bodyalpha.addAttribute("command", "set bodyalpha");
	xNode_bodyalpha.addText(
	    editParametersString[BODYALPHA_PARAM_STRING].value.c_str());

	// km//
	xNode_km = xNode_rendering_parameters.addChild("Km");
	xNode_km.addAttribute("command", "set Km");
	xNode_km.addText(editParametersString[KM_PARAM_STRING].value.c_str());

	// Veins//
	xNode_Veins = xNode_rendering_parameters.addChild("Veins");
	xNode_Veins.addAttribute("command", "set Veins");
	xNode_Veins.addText(editParametersString[VEINS_PARAM_STRING].value.c_str());

	// Wrinkles//
	xNode_Wrinkles = xNode_rendering_parameters.addChild("Wrinkles");
	xNode_Wrinkles.addAttribute("command", "set Wrinkles");
	xNode_Wrinkles.addText(
	    editParametersString[WRINKLES_PARAM_STRING].value.c_str());

	// oilVal//
	xNode_oilVal = xNode_rendering_parameters.addChild("oilVal");
	xNode_oilVal.addAttribute("command", "set oilVal");
	xNode_oilVal.addText(editParametersString[OILVAL_PARAM_STRING].value.c_str());

	// sssScale//
	xNode_sssScale = xNode_rendering_parameters.addChild("sssScale");
	xNode_sssScale.addAttribute("command", "set sssScale");
	xNode_sssScale.addText(
	    editParametersString[SSSSCALE_PARAM_STRING].value.c_str());

	// Ka//
	xNode_Ka = xNode_rendering_parameters.addChild("Ka");
	xNode_Ka.addAttribute("command", "set Ka");
	xNode_Ka.addText(editParametersString[KA_PARAM_STRING].value.c_str());

	// ColorR//
	xNode_ColorR = xNode_rendering_parameters.addChild("ColorR");
	xNode_ColorR.addAttribute("command", "set ColorR");
	xNode_ColorR.addText(editParametersString[COLORR_PARAM_STRING].value.c_str());

	// ColorG//
	xNode_ColorG = xNode_rendering_parameters.addChild("ColorG");
	xNode_ColorG.addAttribute("command", "set ColorG");
	xNode_ColorG.addText(editParametersString[COLORG_PARAM_STRING].value.c_str());

	// ColorB//
	xNode_ColorB = xNode_rendering_parameters.addChild("ColorB");
	xNode_ColorB.addAttribute("command", "set ColorB");
	xNode_ColorB.addText(editParametersString[COLORB_PARAM_STRING].value.c_str());

	// DisplayX
	xNode_DisplayX = xNode_rendering_parameters.addChild("DisplayX");
	xNode_DisplayX.addAttribute("command", "set DisplayX");
	xNode_DisplayX.addText(
	    editParametersString[DISPLAYX_PARAM_STRING].value.c_str());

	// DisplayY
	xNode_DisplayY = xNode_rendering_parameters.addChild("DisplayY");
	xNode_DisplayY.addAttribute("command", "set DisplayY");
	xNode_DisplayY.addText(
	    editParametersString[DISPLAYY_PARAM_STRING].value.c_str());

	XMLNode xNode_exp_controller;
	xNode_collada_parameters = xNode_makehuman.addChild("collada_parameters");
	xNode_exp_controller = xNode_collada_parameters.addChild("export_controller");
	xNode_exp_controller.addAttribute("command", "set exp_controller");

	if (global.getExpMode() == WITH_CONTROLLER) {
		xNode_exp_controller.addText("true");
	} else {
		xNode_exp_controller.addText("false");
	}

	XMLError error = xMainNode.writeToFile(
	    (getUserWorkDir() + "makehuman" + PATH_SEPARATOR + "makehuman.parameters")
	        .c_str());

	if (error > 0) {
		return;
	} else {
		return;
	}
}

void ParseConfigurationXML()
{

	string file = (getUserWorkDir() + "makehuman" + PATH_SEPARATOR +
	               "makehuman.parameters");
	XMLNode xNode_main = XMLNode::parseFile(file.c_str());
	string path, value;
	Global &global = Global::instance();

	if (xNode_main.isEmpty())
		return;

	XMLNode xNode_makehuman;
	XMLNode xNode_rendering_parameters, xNode_collada_parameters,
	    xNode_aqsis_path, xNode_pixie_path, xNode_rendering_path,
	    xNode_texture_path;

	XMLNode xNode_head_texture, xNode_body_texture, xNode_head_bump,
	    xNode_body_bump;

	XMLNode xNode_Km, xNode_Veins, xNode_Wrinkles, xNode_oilVal, xNode_sssScale,
	    xNode_Ka, xNode_ColorR, xNode_ColorG, xNode_ColorB;
	XMLNode xNode_DisplayX, xNode_DisplayY, xNode_exp_controller;

	xNode_makehuman = xNode_main.getChildNode("MAKEHUMAN");

	if (!xNode_makehuman.isEmpty()) {
		xNode_rendering_parameters =
		    xNode_makehuman.getChildNode("rendering_parameters");

		if (!xNode_rendering_parameters.isEmpty()) {

			xNode_aqsis_path = xNode_rendering_parameters.getChildNode("aqsis_path");
			if (!xNode_aqsis_path.isEmpty()) {

				if (xNode_aqsis_path.getText() != NULL) {
					path = xNode_aqsis_path.getText();
					SetAqsisBasePath(path);
				}
			}

			xNode_pixie_path = xNode_rendering_parameters.getChildNode("pixie_path");
			if (!xNode_pixie_path.isEmpty()) {
				if (xNode_pixie_path.getText() != NULL) {
					path = xNode_pixie_path.getText();
					SetPixieBasePath(path);
				}
			}

			xNode_rendering_path =
			    xNode_rendering_parameters.getChildNode("rendering_path");
			if (!xNode_rendering_path.isEmpty()) {

				if (xNode_rendering_path.getText() != NULL) {
					string value(xNode_rendering_path.getText());
					editParametersString[RENDPATH_PARAM_STRING].value = value;
				}
			}

			xNode_texture_path =
			    xNode_rendering_parameters.getChildNode("texture_path");
			if (!xNode_texture_path.isEmpty()) {
				if (xNode_texture_path.getText() != NULL) {
					value = xNode_texture_path.getText();
					editParametersString[TEXTPATH_PARAM_STRING].value = value;
				}
			}

			xNode_head_texture =
			    xNode_rendering_parameters.getChildNode("head_color");
			if (!xNode_head_texture.isEmpty()) {
				if (xNode_head_texture.getText() != NULL) {
					value = xNode_head_texture.getText();
					editParametersString[HEADCOLOR_PARAM_STRING].value = value;
				}
			}

			xNode_body_texture =
			    xNode_rendering_parameters.getChildNode("body_color");
			if (!xNode_body_texture.isEmpty()) {
				if (xNode_body_texture.getText() != NULL) {
					value = xNode_body_texture.getText();
					editParametersString[BODYCOLOR_PARAM_STRING].value = value;
				}
			}

			xNode_head_bump = xNode_rendering_parameters.getChildNode("head_bump");
			if (!xNode_head_bump.isEmpty()) {
				if (xNode_head_bump.getText() != NULL) {
					value = xNode_head_bump.getText();
					editParametersString[HEADBUMP_PARAM_STRING].value = value;
				}
			}

			xNode_body_bump = xNode_rendering_parameters.getChildNode("body_bump");
			if (!xNode_body_bump.isEmpty()) {
				if (xNode_body_bump.getText() != NULL) {
					value = xNode_body_bump.getText();
					editParametersString[BODYBUMP_PARAM_STRING].value = value;
				}
			}

			XMLNode xNode_eyescol, xNode_eyebcol, xNode_eyelcol, xNode_headspec,
			    xNode_bodyspec;

			xNode_eyescol = xNode_rendering_parameters.getChildNode("eyes_color");
			if (!xNode_eyescol.isEmpty()) {
				if (xNode_eyescol.getText() != NULL) {
					value = xNode_eyescol.getText();
					editParametersString[EYESCOL_PARAM_STRING].value = value;
				}
			}

			xNode_eyebcol = xNode_rendering_parameters.getChildNode("eyebrows_color");
			if (!xNode_eyebcol.isEmpty()) {
				if (xNode_eyebcol.getText() != NULL) {
					value = xNode_eyebcol.getText();
					editParametersString[EYEBCOL_PARAM_STRING].value = value;
				}
			}

			xNode_eyelcol =
			    xNode_rendering_parameters.getChildNode("eyelashes_color");
			if (!xNode_eyelcol.isEmpty()) {
				if (xNode_eyelcol.getText() != NULL) {
					value = xNode_eyelcol.getText();
					editParametersString[EYELCOL_PARAM_STRING].value = value;
				}
			}

			xNode_headspec = xNode_rendering_parameters.getChildNode("head_specular");
			if (!xNode_headspec.isEmpty()) {
				if (xNode_headspec.getText() != NULL) {
					value = xNode_headspec.getText();
					editParametersString[HEADSPEC_PARAM_STRING].value = value;
				}
			}

			xNode_bodyspec = xNode_rendering_parameters.getChildNode("body_specular");
			if (!xNode_bodyspec.isEmpty()) {
				if (xNode_bodyspec.getText() != NULL) {
					value = xNode_bodyspec.getText();
					editParametersString[BODYSPEC_PARAM_STRING].value = value;
				}
			}

			XMLNode xNode_eyesalpha, xNode_eyebalpha, xNode_eyelalpha,
			    xNode_headalpha, xNode_bodyalpha;

			xNode_eyesalpha = xNode_rendering_parameters.getChildNode("eyes_alpha");
			if (!xNode_eyesalpha.isEmpty()) {
				if (xNode_eyesalpha.getText() != NULL) {
					value = xNode_eyesalpha.getText();
					editParametersString[EYESALPHA_PARAM_STRING].value = value;
				}
			}

			xNode_eyebalpha =
			    xNode_rendering_parameters.getChildNode("eyebrows_alpha");
			if (!xNode_eyebalpha.isEmpty()) {
				if (xNode_eyebalpha.getText() != NULL) {
					value = xNode_eyebalpha.getText();
					editParametersString[EYEBALPHA_PARAM_STRING].value = value;
				}
			}

			xNode_eyelalpha =
			    xNode_rendering_parameters.getChildNode("eyelashes_alpha");
			if (!xNode_eyelalpha.isEmpty()) {
				if (xNode_eyelalpha.getText() != NULL) {
					value = xNode_eyelalpha.getText();
					editParametersString[EYELALPHA_PARAM_STRING].value = value;
				}
			}

			xNode_headalpha = xNode_rendering_parameters.getChildNode("head_alpha");
			if (!xNode_headalpha.isEmpty()) {
				if (xNode_headalpha.getText() != NULL) {
					value = xNode_headalpha.getText();
					editParametersString[HEADALPHA_PARAM_STRING].value = value;
				}
			}

			xNode_bodyalpha = xNode_rendering_parameters.getChildNode("body_alpha");
			if (!xNode_bodyalpha.isEmpty()) {
				if (xNode_bodyalpha.getText() != NULL) {
					value = xNode_bodyalpha.getText();
					editParametersString[BODYALPHA_PARAM_STRING].value = value;
				}
			}
			//--------------------------//

			xNode_Km = xNode_rendering_parameters.getChildNode("Km");
			if (!xNode_Km.isEmpty()) {
				if (xNode_Km.getText() != NULL) {
					value = xNode_Km.getText();
					editParametersString[KM_PARAM_STRING].value = value;
				}
			}

			xNode_Veins = xNode_rendering_parameters.getChildNode("Veins");
			if (!xNode_Veins.isEmpty()) {
				if (xNode_Veins.getText() != NULL) {
					value = xNode_Veins.getText();
					editParametersString[VEINS_PARAM_STRING].value = value;
				}
			}

			xNode_Wrinkles = xNode_rendering_parameters.getChildNode("Wrinkles");
			if (!xNode_Wrinkles.isEmpty()) {
				if (xNode_Wrinkles.getText() != NULL) {
					value = xNode_Wrinkles.getText();
					editParametersString[WRINKLES_PARAM_STRING].value = value;
				}
			}

			xNode_oilVal = xNode_rendering_parameters.getChildNode("oilVal");
			if (!xNode_oilVal.isEmpty()) {
				if (xNode_oilVal.getText() != NULL) {
					value = xNode_oilVal.getText();
					editParametersString[OILVAL_PARAM_STRING].value = value;
				}
			}

			xNode_sssScale = xNode_rendering_parameters.getChildNode("sssScale");
			if (!xNode_sssScale.isEmpty()) {
				if (xNode_sssScale.getText() != NULL) {
					value = xNode_sssScale.getText();
					editParametersString[SSSSCALE_PARAM_STRING].value = value;
				}
			}

			xNode_Ka = xNode_rendering_parameters.getChildNode("Ka");
			if (!xNode_Ka.isEmpty()) {
				if (xNode_Ka.getText() != NULL) {
					value = xNode_Ka.getText();
					editParametersString[KA_PARAM_STRING].value = value;
				}
			}

			xNode_ColorR = xNode_rendering_parameters.getChildNode("ColorR");
			if (!xNode_ColorR.isEmpty()) {
				if (xNode_ColorR.getText() != NULL) {
					value = xNode_ColorR.getText();
					editParametersString[COLORR_PARAM_STRING].value = value;
				}
			}

			xNode_ColorG = xNode_rendering_parameters.getChildNode("ColorG");
			if (!xNode_ColorG.isEmpty()) {
				if (xNode_ColorG.getText() != NULL) {
					value = xNode_ColorG.getText();
					editParametersString[COLORG_PARAM_STRING].value = value;
				}
			}

			xNode_ColorB = xNode_rendering_parameters.getChildNode("ColorB");
			if (!xNode_ColorB.isEmpty()) {
				if (xNode_ColorB.getText() != NULL) {
					value = xNode_ColorB.getText();
					editParametersString[COLORB_PARAM_STRING].value = value;
				}
			}

			xNode_DisplayX = xNode_rendering_parameters.getChildNode("DisplayX");
			if (!xNode_DisplayX.isEmpty()) {
				if (xNode_DisplayX.getText() != NULL) {
					value = xNode_DisplayX.getText();
					editParametersString[DISPLAYX_PARAM_STRING].value = value;
				}
			}

			xNode_DisplayY = xNode_rendering_parameters.getChildNode("DisplayY");
			if (!xNode_DisplayY.isEmpty()) {
				if (xNode_DisplayY.getText() != NULL) {
					value = xNode_DisplayY.getText();
					editParametersString[DISPLAYY_PARAM_STRING].value = value;
				}
			}
		}

		xNode_collada_parameters =
		    xNode_makehuman.getChildNode("collada_parameters");
		if (!xNode_collada_parameters.isEmpty()) {
			xNode_exp_controller =
			    xNode_collada_parameters.getChildNode("export_controller");
			if (!xNode_exp_controller.isEmpty()) {

				if (xNode_exp_controller.getText() != NULL) {
					if (!strcasecmp(xNode_exp_controller.getText(), "true")) {
						global.setExportMode(WITH_CONTROLLER);
					}
					if (!strcasecmp(xNode_exp_controller.getText(), "false")) {
						global.setExportMode(WITHOUT_CONTROLLER);
					}
				}
			}
		}
	}
}

void loadDefaultBodySettings()
{
	Global &global = Global::instance();
	Mesh *mesh = global.getMesh();
	assert(mesh);
	Window &mainWindow = Window::instance();

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
	Global &global = Global::instance();
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
	Global &global = Global::instance();
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
	Global &global = Global::instance();
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
