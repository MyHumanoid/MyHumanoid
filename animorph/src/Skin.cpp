/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 *  Copyright (C) 2006  MakeHuman Project
 *
 *  This program is free software; you  can  redistribute  it  and/or
 *  modify  it  under  the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either  version  2  of
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
 *  File   : Skin.cpp
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  App    : makehuman
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#include "../include/animorph/Skin.h"

using namespace std;
using namespace Animorph;

bool Skin::load(const std::string &filename)
{
	char str[MAX_LINE_BUFFER];
	vector<int> tokens;
	bool rc = true;
	int ret = 0;
	int skinVertex;

	clear();

	FILE *fd = fopen(filename.c_str(), "r");

	if (fd == NULL)
		return false;

	// get the current locale
	char *locale = ::setlocale(LC_NUMERIC, NULL);

	// set it to "C"-Style ( the . (dot) means the decimal marker for floats)
	::setlocale(LC_NUMERIC, "C");

	// Skin &skin(*this);

	for (;;) {
		ret = fscanf(fd, "%i,%s", &skinVertex, str);

		if (ret == EOF) // end of file reached?
			break;

		if (ret != 2 && (ret != 0)) {
			cerr << "Illegal line while reading skin info '" << filename << "'!"
			     << endl;
			clear();
			rc = false; // mark the error
			break;
		}

		string values(str);
		stringTokeni(values, ", ", tokens);

		if (tokens.empty()) {
			cerr << "Illegal line while reading skin info '" << filename << "'!"
			     << endl;
			clear();
			rc = false; // mark the error
			break;
			rc = false;
			break;
		}

		SkinVertex tmpSkinVertex(skinVertex, 0.0f, tokens);
		push_back(tmpSkinVertex);
		tokens.clear();
	}

	// reset locale after file was written
	::setlocale(LC_NUMERIC, locale);

	fclose(fd);

	return rc;
}
