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
 *  File   : Component.cpp
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: MHGUI
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#include "gui/Component.h"
#include "gui/AbstractListener.h"
#include "gui/GLUTWrapper.h"

namespace mhgui
{

bool Component::operator==(const Component & inRHS) const
{
	if(this == &inRHS) // object identity?
		return true;
	return (id == inRHS.id);
}

/* ========================================================================== *
 * ctor & dtors
 * ========================================================================== */

/* ========================================================================== */
/** ctor.
 */
/* ========================================================================== */
Component::Component(uint32_t inId, const Rect & inGeometry)
        : id(inId)
        , geometry(inGeometry)
        , absoluteGeometry(inGeometry)
        , zeroPoint(0, 0)
        , listener(NULL)
        , // No listener per default (use setListener())
        sysListener(NULL)
        , // No sysListener per default (use setSysListener())
        active(false)
        , visible(false)
        , clickConsumed(false)
        , mouseOver(false)
{
}

/* ========================================================================== */
/** dtor.
 */
/* ========================================================================== */
Component::~Component() {}

void Component::setPosition(const Point & inPos)
{
	geometry.moveTo(inPos);

	absoluteGeometry = geometry;
	absoluteGeometry.moveBy(zeroPoint);
}

void Component::setSize(const Size & inSize)
{
	absoluteGeometry.resizeTo(inSize);
	geometry.resizeTo(inSize);
}

void Component::setRect(const Rect & inRect)
{
	absoluteGeometry = inRect;
	absoluteGeometry.moveBy(zeroPoint);

	geometry = inRect;
}

/* ========================================================================== */
/**
 */
/* ========================================================================== */
void Component::show() {}

/* ========================================================================== */
/**
 */
/* ========================================================================== */
void Component::hide() {}

void Component::setZeroPoint(const Point & inZero)
{
	zeroPoint = inZero;

	absoluteGeometry = geometry;
	absoluteGeometry.moveBy(inZero);
}

/* ========================================================================== */
/** Check if mouse over, and use the listener mouseover function.
 */
/* ========================================================================== */
bool Component::isMouseOver(const Point & inMousePos)
{
	bool isOver = false;

	if(absoluteGeometry.isHitBy(inMousePos)) {
		if(sysListener)
			sysListener->mouseOver(inMousePos, this);

		if(listener)
			isOver = listener->mouseOver(inMousePos, this);

		mouseOver = true;
	} else if(mouseOver) {
		if(sysListener)
			sysListener->mouseOut(inMousePos, this);

		if(listener)
			listener->mouseOut(inMousePos, this);

		mouseOver = false;
	}
	return isOver;
};

/* ========================================================================== */
/** Check if mouse click, and use the listener mousepressed or mousereleased
 * function.
 */
/* ========================================================================== */
bool Component::isMouseClick(const Point & inMousePos, int button, int state)
{
	bool isClick = false;
	bool isHit   = absoluteGeometry.isHitBy(inMousePos);

	//  if (absoluteGeometry.isHitBy(inMousePos))
	//  {
	if((button == GLUT_LEFT_BUTTON || button == GLUT_RIGHT_BUTTON) && state == GLUT_DOWN &&
	   isHit) {
		if(sysListener)
			sysListener->mousePressed(inMousePos, button, this);

		if(listener)
			isClick = listener->mousePressed(inMousePos, button, this);
	} else if((button == GLUT_LEFT_BUTTON || button == GLUT_RIGHT_BUTTON) && state == GLUT_UP &&
	          isActive()) {
		if(sysListener)
			sysListener->mouseReleased(inMousePos, button, this);

		if(listener)
			isClick = listener->mouseReleased(inMousePos, button, this);
	} else if((button == GLUT_WHEEL_DOWN || button == GLUT_WHEEL_UP ||
	           button == GLUT_WHEEL_RIGHT || button == GLUT_WHEEL_LEFT) &&
	          isHit) {
		if(listener)
			isClick = listener->mouseWheel(inMousePos, button, this);

		if(sysListener)
			sysListener->mouseWheel(inMousePos, button, this);
	}

	//  }
	else if(active == true)
		active = false;

	return isClick;
};

/* ========================================================================== */
/** Check keyboard events.
 */
/* ========================================================================== */
bool Component::isKeyType(unsigned char key)
{
	bool isType = false;
	if(isActive()) {
		if(sysListener)
			sysListener->keyType(key, this);

		if(listener)
			isType = listener->keyType(key, this);
	}
	return isType;
}

/* ========================================================================== */
/**
 */
/* ========================================================================== */
bool Component::isMouseDragged(const Point & inMousePos)
{
	bool dragged = false;
	if(isActive()) {
		if(sysListener)
			dragged = sysListener->mouseDragged(inMousePos, this);

		if(listener)
			dragged = listener->mouseDragged(inMousePos, this);
	}
	return dragged;
}

/* ========================================================================== */
/**
 */
/* ========================================================================== */
const string Component::getIDAsString() const
{
	char tmpBuffer[8];
	tmpBuffer[4] = '\0';

#if BYTE_ORDER == BIG_ENDIAN
	tmpBuffer[0] = (uint8_t)(id >> 24);
	tmpBuffer[1] = (uint8_t)(id >> 16);
	tmpBuffer[2] = (uint8_t)(id >> 8);
	tmpBuffer[3] = (uint8_t)(id >> 0);
#elif BYTE_ORDER == LITTLE_ENDIAN
	tmpBuffer[3] = (uint8_t)(id >> 24);
	tmpBuffer[2] = (uint8_t)(id >> 16);
	tmpBuffer[1] = (uint8_t)(id >> 8);
	tmpBuffer[0] = (uint8_t)(id >> 0);
#else
#error "Unknown endianess!"
#endif
	return string(tmpBuffer);
}

} // namespace mhgui
