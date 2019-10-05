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
 *  File   : Component.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: MHGUI
 *
 *  For individual developers look into the AUTHORS file.
 *
 */
#pragma once

#include "Rect.h"
#include <iostream>
#include <stdint.h>
#include <string>

using std::cerr;
using std::endl;

namespace mhgui
{

class AbstractListener;

class Component
{
private:
	uint32_t           id;
	Rect               geometry;
	Rect               absoluteGeometry;
	glm::ivec2              zeroPoint;
	AbstractListener * listener;
	AbstractListener * sysListener;

	// On mouse pressed -> true; on mouse released -> false
	mutable bool active;
	mutable bool visible;

	bool         clickConsumed;
	mutable bool mouseOver;

public:
	Component(uint32_t inId, const Rect & inGeometry);

	virtual ~Component();

	void setListener(AbstractListener * inListener) { listener = inListener; }
	void setSysListener(AbstractListener * inListener) { sysListener = inListener; }

	void setPosition(const glm::ivec2 & inPos);

	const glm::ivec2 & getPosition() const { return geometry.getPos(); }

	void setSize(const Size & inSize);

	const Size & getSize() const { return geometry.getSize(); }

	void setRect(const Rect & inRect);

	const Rect & getRect() const { return geometry; }

	bool isClickConsumed() const { return clickConsumed; }
	void setClickConsumed(bool value) { clickConsumed = value; }

	bool isActive() const { return active; }
	void setActive(bool inActive) { active = inActive; }

	bool isVisible() const { return visible; }
	void setVisible(bool inVisible) { visible = inVisible; }

	const uint32_t getID() const { return id; }
	const std::string   getIDAsString() const;

	bool isLastMouseOver() const { return mouseOver; }

	virtual bool isMouseOver(const glm::ivec2 & inMousePos);
	virtual bool isMouseClick(const glm::ivec2 & inMousePos, int button, int state);
	virtual bool isMouseDragged(const glm::ivec2 & inMousePos);
	virtual bool isKeyType(unsigned char);

protected:
	const glm::ivec2 & getAbsolutePosition() const { return absoluteGeometry.getPos(); }
	void          setZeroPoint(const glm::ivec2 & inZero);
	const glm::ivec2 & getZeroPoint() { return zeroPoint; }

private: // intentionally not implemented
	Component(const Component &);
	Component & operator=(const Component &);

public:
	virtual void draw() = 0;
	virtual void show();
	virtual void hide();

	const Rect & getAbsoluteRect() const { return absoluteGeometry; }

	// Operators
	bool operator==(const Component & inRHS) const;
};

} // namespace mhgui
