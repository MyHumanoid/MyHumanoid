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
 *  File   : Global.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  App    : makehuman
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#ifndef GLOBAL_H
#define GLOBAL_H 1

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "Animation.h"
#include <animorph/Mesh.h>
#include <gui/Camera.h>
#include <gui/Point.h>
#include <gui/Texture.h>

using namespace Animorph;
using namespace mhgui;

enum Modes {
	BODY_DETAILS,
	POSES,
	CHARACTER_SETTING,
	CLOTHES,
	ANIMATIONS,
	BODY_SETTINGS,
	POSES_BODY_SETTINGS,
};
enum ExportMode {
	WITHOUT_CONTROLLER = 0,
	WITH_CONTROLLER,
};
class Global
{
private:
	static Global *sSingleGlobal;

	// intentionally not implemented
	Global()
	    : mesh(NULL)
	    , drawGrid(false)
	    , flatShading(false)
	    , quotedBox(false)
	    , texture(false)
	    , canTexture(false)
	    , rendering(false)
	    , subdivision(false)
	    , lightMesh(false)
	    , appMode(CHARACTER_SETTING)
	    , fuzzyValues()
	    , expMode(WITHOUT_CONTROLLER)
	{
	}

	Global(const Global &);
	Global &operator=(const Global &);

	// some global variables
	Mesh *mesh;
	Camera *camera;
	map<string, Texture *> textures;

	Animation *animation;
	Autozoom *autozoom;
	bool drawGrid;
	bool flatShading;
	bool quotedBox;
	bool texture;
	bool canTexture;
	bool rendering;
	bool subdivision;
	bool lightMesh;
	Modes appMode;

	map<const uint32_t, Point *> fuzzyValues;
	ExportMode expMode;

	static Global glob;

public:
	static Global &instance();

	void setMesh(Mesh *mesh) { this->mesh = mesh; }
	Mesh *getMesh() { return mesh; }

	void setCamera(Camera *camera) { this->camera = camera; }
	Camera *getCamera() { return camera; }

	void setTexture(string name, Texture *texture) { textures[name] = texture; }
	Texture *getTexture(string name) { return textures[name]; }

	void setAnimation(Animation *animation) { this->animation = animation; }
	Animation *getAnimation() { return animation; }

	void setAutozoom(Autozoom *autozoom) { this->autozoom = autozoom; }
	Autozoom *getAutozoom() { return autozoom; }

	void setDrawGrid(bool grid) { drawGrid = grid; }
	bool getDrawGrid() { return drawGrid; }

	void setFlatShading(bool flat) { flatShading = flat; }
	bool getFlatShading() { return flatShading; }

	void setQuotedBox(bool qbox) { quotedBox = qbox; }
	bool getQuotedBox() { return quotedBox; }

	void setAppMode(Modes mode) { appMode = mode; }
	Modes getAppMode() { return appMode; }

	void setTexture(bool texture) { this->texture = texture; }
	bool getTexture() { return texture; }

	void setCanTexture(bool canTexture) { this->canTexture = canTexture; }
	bool getCanTexture() { return canTexture; }

	void setRendering(bool rendering) { this->rendering = rendering; }
	bool isRendering() { return rendering; }

	void setFuzzyValue(uint32_t index, const Point &inPoint)
	{
		this->fuzzyValues[index] = new Point(inPoint);
	}
	void resetFuzzyValues() { fuzzyValues.clear(); }
	Point *getFuzzyValue(const uint32_t index)
	{
		return this->fuzzyValues[index];
	}

	void setExportMode(ExportMode mode) { expMode = mode; }
	ExportMode getExpMode() { return expMode; }

	void setSubdivision(bool subd) { subdivision = subd; }
	bool getSubdivision() { return subdivision; }

	void setLightMesh(bool lmesh) { lightMesh = lmesh; }
	bool getLightMesh() { return lightMesh; }
};

#endif // GLOBAL_H
