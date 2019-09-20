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
 *  File   : MultiPanel.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  Library: MHGUI
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#ifndef MULTIPANEL_H
#define MULTIPANEL_H 1

#include "Panel.h"
#include "Image.h"

#include <list>
#include <cassert>
#include <animorph/Color.h>
#include "Window.h"

using std::vector;

namespace mhgui {

class MultiPanel : public Panel
{
protected:
  vector <Panel*> panelList;
  Window  *parentWindow;
  int numPages;
  Image  *nextPage;
  Image  *prevPage;
  void checkControlsVisibility();

public:
  MultiPanel (uint32_t    inMultiPanelId,
              const Rect& inGeometry);
  virtual ~MultiPanel ();

  virtual void draw ();

  bool addPanel    (Panel *p);
  void removePanel (Panel *p);

  bool hasNextPage();
  bool hasPrevPage();

  void pageNext();
  void pageBack();

  typedef vector<Panel*>::iterator PanelIterator;

  PanelIterator begin() {return panelList.begin();}
  PanelIterator end()   {return panelList.end();}

  virtual bool isMouseOverWidgets (const Point& inMousePos);
  virtual bool isMouseClickWidgets   (const Point& inMousePos, int button, int state);
  virtual bool isMouseDraggedWidgets (const Point& inMousePos);
  virtual bool isKeyTypeWidgets      (unsigned char key);

  virtual void calcWidgetPosition ();

private: // intentionally not implemeted
  MultiPanel(const MultiPanel&);
  MultiPanel& operator=(const MultiPanel&);

  int currentPage;

};

} // namespace mhgui

#endif //MULTIPANEL_H
