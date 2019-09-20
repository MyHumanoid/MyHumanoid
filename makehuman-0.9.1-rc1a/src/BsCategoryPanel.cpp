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
 *  File   : BsCategoryPanel.cpp
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  App    : makehuman
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#include "BsCategoryPanel.h"
#include "Global.h"

#include <mhgui/Window.h>
#include <mhgui/Rect.h>
#include <mhgui/Image.h>
#include <mhgui/Tooltip.h>
#include "Global.h"


using namespace std;
using namespace Animorph;

BsCategoryPanel::BsCategoryPanel ()
    : Panel (kComponentID_BsCategoryPanel, Rect(0,16,192,384))
{

}

BsCategoryPanel::~BsCategoryPanel ()
{
  for_each (imageVector.begin (), imageVector.end (), deleteFunctor <Image*> ());
}


void BsCategoryPanel::createWidgets ()
{
  Window &mainWindow = Window::instance ();

  Panel *tooltipPanel = mainWindow.getPanel(kComponentID_TooltipPanel);
  assert(tooltipPanel);

  const Color color_red (1.0, 0.0, 0.0);
  const Point kTooltipPos(70, 12);
  const float alpha = 1.0;
  Image *cat;
  bool ret;

  // -------------------------------------------------------------------------

   for (int i = 0;i<17;i++){
    std::ostringstream out, out2, out3;
    out  << "ui/characters" << i+1 << ".png";
    out2 << (i + 1);
    out3 << "characters" << i+1  <<"";

    cat = new Image(table_id[i],
                   searchPixmapFile (out.str().c_str()),
                   Rect (0,0,64,64));
    cat->setListener(&selectionListener);
    cat->setTooltip(Tooltip("Characters Library " + out2.str(), kTooltipPos, color_red, tooltipPanel));
    cat->setAlpha (alpha);
    ret = checkEnabled(out3.str().c_str());
    cat->setEnabled(ret);
    cat->setActive(ret);
    imageVector.push_back (cat);
    addWidget(cat);

  }
  // -------------------------------------------------------------------------

  cat = new Image(kComponentID_BsCategoryPanel_MyBS,
                   searchPixmapFile ("ui/mycharacters.png"),
                   Rect (0,0,64,64));
  cat->setListener(&selectionListener);
  cat->setTooltip(Tooltip("My Characters Library", kTooltipPos, color_red, tooltipPanel));
  cat->setAlpha (alpha);
  imageVector.push_back (cat);
  addWidget(cat);
}


bool BsCategoryPanel::checkEnabled(string category)
{
  Global &global = Global::instance ();

  Mesh *mesh = global.getMesh ();
  assert (mesh);

  CharactersMap &charactersmap = mesh->getCharactersMapRef ();

  for (CharactersMap::const_iterator charactersmap_it = charactersmap.begin ();
      charactersmap_it != charactersmap.end ();
      charactersmap_it++){

    const string &character_name((*charactersmap_it).first);

    string::size_type loc = character_name.find ("/", 0 );
    if (loc == string::npos)
     continue;
    else{
      string sub = character_name.substr (0, loc);

      if (sub == category){
        return true;
      }
    }
  }
  return false;
}
