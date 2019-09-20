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
 *  File   : BsSelectionListener.cpp
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  App    : makehuman
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#include <animorph/Mesh.h>

#include <mhgui/Image.h>
#include <mhgui/CGUtilities.h>
#include <mhgui/Window.h>
#include <mhgui/Component.h>
#include <mhgui/GLUTWrapper.h>

#include <stdio.h>
#include <assert.h>

#include "BsCategoryPanel.h"
#include "BsSelectionListener.h"
#include "ComponentID.h"
#include "BsPanel.h"
#include "Global.h"

using namespace Animorph;
using namespace std;

BsSelectionListener::BsSelectionListener()
    : AbstractListener()
{
}

BsSelectionListener::~BsSelectionListener()
{
}

bool BsSelectionListener::mouseOver (const Point& inMousePos, Component *source)
{
  Image *imgSource = dynamic_cast<Image *>(source); // req. RTTI!
  assert(imgSource); // Check if this is really an Image object?

  imgSource->setOverlayRectangle (Color (1,0,0,0.5));

  return false;
}

bool BsSelectionListener::mouseOut (const Point& inMousePos, Component *source)
{
  Image *imgSource = dynamic_cast<Image *>(source); // req. RTTI!
  assert(imgSource); // Check if this is really an Image object?

  imgSource->setOverlayRectangle (false);

  return false;
}

bool BsSelectionListener::mouseDragged (const Point& inMousePos, Component *source)
{
  return true;
}

bool BsSelectionListener::mouseWheel    (const Point& inMousePos, int inButton, Component *source )
{
  return false;
}

bool BsSelectionListener::mousePressed(const Point& inMousePos, int button, Component *source)
{
  if (button == GLUT_LEFT_BUTTON)
  {
    Image *imgSource = dynamic_cast<Image *>(source); // req. RTTI!
    assert(imgSource); // Check if this is really an Image object?
    imgSource->setOverlayRectangle(Color (1,1,1,0.5));

    return true;
  }
  return false;
}

bool BsSelectionListener::mouseReleased (const Point& inMousePos, int button, Component *source)
{
  std::ostringstream out_stream;
  if(button == GLUT_LEFT_BUTTON)
  {
    Image *imgSource = dynamic_cast<Image *>(source); // req. RTTI!
    assert(imgSource); // Check if this is really an Image object?

    imgSource->setOverlayRectangle(false);

    if(!imgSource->getAbsoluteRect().isHitBy(inMousePos))
      return false;

    string character;

    if(imgSource->getID () == kComponentID_BsCategoryPanel_MyBS)
    {
      character = "lib";

      Global &global = Global::instance ();
      Mesh *mesh = global.getMesh ();
      CharactersMap &charactersmap = mesh->getCharactersMapRef ();
      list <string> toErase;

      for (CharactersMap::const_iterator charactersmap_it = charactersmap.begin ();
          charactersmap_it != charactersmap.end ();
          charactersmap_it++)
      {
        const string &character_name((*charactersmap_it).first);

        string::size_type loc = character_name.find ("/", 0 );
        if (loc == string::npos)
        {
          continue;
        }
        else
        {
          string sub = character_name.substr (0, loc);

          if (sub == character)
          {
            toErase.push_back(character_name);
          }
        }
      }

      for (list<string>::const_iterator toErase_it = toErase.begin ();
          toErase_it != toErase.end ();
          toErase_it++)
      {
        charactersmap.erase(*toErase_it);
      }

      mesh->loadCharactersFactory(getMyBodysettingsBasePath());
    }
    else
    {
      for(int i = 0; i < 17; i++)
      {
        if(imgSource->getID () == table_id[i])
        {
          out_stream << i + 1;
          character = "characters" + out_stream.str();
        }
      }
    }

    if(!character.empty())
    {
      Window &mainWindow = Window::instance ();
      BsPanel* bsPanel = dynamic_cast<BsPanel*>(mainWindow.getPanel (kComponentID_TargetPanel));

      if ((bsPanel == NULL) || (bsPanel->getCategory() != character))
      {
        mainWindow.removePanel(bsPanel);
        delete bsPanel;

        int x = mainWindow.getSize().getWidth() - 210;
        bsPanel = new BsPanel (character, Rect(x,40,210,517));
        mainWindow.addPanel (bsPanel);
        bsPanel->createWidgets ();
      }
      bsPanel->show();
      return true;
    }
    else
    {
      return false;
    }
  }

  return false;
}

bool BsSelectionListener::keyType (unsigned char key, Component *source)
{
  return false;
}
