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
 *  File   : CharacterSettingPanel.h
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  App    : makehuman
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#ifndef CHARACTERSETTINGPANEL_H
#define CHARACTERSETTINGPANEL_H 1

#ifdef HAVE_CONFIG_H
  #include <config.h>
#endif

#include <vector>
#include <mhgui/Panel.h>
#include <mhgui/ImageSlider.h>
#include <mhgui/Image.h>
#include <mhgui/Selector.h>
#include "util.h"
#include "ImageListener.h"
#include "BsSelectionListener.h"
#include "TargetSlider.h"
#include "SelectorListener.h"

using std::vector;

class CharacterSettingPanel : public Panel
{
public:
  CharacterSettingPanel ();
  virtual ~CharacterSettingPanel ();

  void createWidgets ();

  //void resetTargetValues(const string currentTarget = "");

  void resetSlidersValues() ;
  void calcSelectorValues(uint32_t index);
private: // intentionally not implemented
  CharacterSettingPanel             (const CharacterSettingPanel&);
  CharacterSettingPanel& operator = (const CharacterSettingPanel&);

private:
  vector <Image*> imageVector;
  vector <Selector*> selectorVector;
  Selector *selector;
  SelectorListener selectorListener;
};

#endif // CHARACTERSETTINGBODYPANEL_H
