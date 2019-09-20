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
 *  File   : BsPanel.cpp
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  App    : makehuman
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

#include "BsPanel.h"
#include "ComponentID.h"
#include "Global.h"

#include <gui/Window.h>
#include <gui/Rect.h>
#include <gui/ImageSlider.h>
#include <gui/Tooltip.h>

using namespace std;
using namespace Animorph;

const static string kFilePrefixTarget (".bs");
const static string kFilePrefixPNG    (".png");

const static int kMaxTargetsNumber = 24;

BsPanel::BsPanel (const string& category, const Rect& rect)
    : MultiPanel (kComponentID_TargetPanel, rect),
    category (category)
{
  setAlignment(FREE);
  setRightAnchor (true);
}

BsPanel::~BsPanel ()
{
  for_each (imageButtonVector.begin (), imageButtonVector.end (), deleteFunctor <ImageButton*> ());
  for_each (imageVector.begin (), imageVector.end (), deleteFunctor <Image*> ());
  for_each (begin (), end (), deleteFunctor <Panel*> ());
}

PageBsPanel *BsPanel::newPagePanel()
{
  PageBsPanel *page;

  page = new PageBsPanel(category, getRect());

  return page;
}

void BsPanel::createWidgets ()
{
  int numTargetsInPage = 1;

  Global &global = Global::instance ();
  Window &mainWindow = Window::instance ();

  Panel *tooltipPanel = mainWindow.getPanel (kComponentID_TooltipPanel);
  assert(tooltipPanel);

  Mesh *mesh = global.getMesh ();
  assert (mesh);

  CharactersMap &charactersmap = mesh->getCharactersMapRef ();

  const Color c (1.0, 0.0, 0.0);
  const Point kTooltipPos (70, 12);

  ImageButton *image_button;
  PageBsPanel *page = newPagePanel();

  close_image = new Image(kComponentID_CloseTargetPanel,
                    searchPixmapFile ("ui/close_panel.png"),
                    Rect(0,0,64,64));
  close_image->setListener(&imgListener);
  close_image->setTooltip(Tooltip("Close panel", kTooltipPos, c, tooltipPanel));
  page->addWidget (close_image);
  imageVector.push_back(close_image);

  for (CharactersMap::const_iterator charactersmap_it = charactersmap.begin ();
      charactersmap_it != charactersmap.end ();
      charactersmap_it++)
  {
    const string &character_name((*charactersmap_it).first);

    string::size_type loc = character_name.find ("/", 0 );
    if (loc == string::npos)
     continue;
    else
    {
      string sub = character_name.substr (0, loc);

      if (sub == category)
      {
        // remove ".bs"
        string character_image (character_name);
        if(category == "lib") // user bs
        {
          character_image = "user_bs.png";
        }
        else
        {
          character_image.replace (character_image.length () - kFilePrefixTarget.length (),
                                   kFilePrefixTarget.length (), kFilePrefixPNG);
        }

        if(numTargetsInPage++ > kMaxTargetsNumber)
        {
          numTargetsInPage = 0;
          addPanel(page);
          page = newPagePanel();
        }

        image_button = new ImageButton(kComponentID_TargetPanel_Target,
                                       searchPixmapFile ("bsimg/" + character_image),
                                       character_name,
                                       Rect(0,0,64,64));
        image_button->setListener(&imgButtonListener);
        image_button->setTooltip(Tooltip(character_name, kTooltipPos, c, tooltipPanel));
        image_button->setOverlayTexture(searchPixmapFile ("bsimg/" + character_image));
        page->imageButtonVector.push_back (image_button);
        page->addWidget (image_button);
      }
    }
  }
  addPanel(page);

  if(numPages > 1)
  {

    int x = getSize().getWidth() ;
    int y = getSize().getHeight();
    prevPage = new Image(kComponentID_PrevTargetPanel,
                      searchPixmapFile ("ui/page_prev.png"),
                      Rect(x-70, y-32, 32, 32));
    prevPage->setListener(&imgListener);
    prevPage->setTooltip(Tooltip("Prev page", kTooltipPos, c, tooltipPanel));
    addWidget(prevPage);
    imageVector.push_back(prevPage);

    nextPage = new Image(kComponentID_NextTargetPanel,
                      searchPixmapFile ("ui/page_next.png"),
                      Rect(x-35, y-32, 32, 32));
    nextPage->setListener(&imgListener);
    nextPage->setTooltip(Tooltip("Next page", kTooltipPos, c, tooltipPanel));
    addWidget(nextPage);
    imageVector.push_back(nextPage);

    checkControlsVisibility();
  }

}
