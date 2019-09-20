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
 *  File   : makehuman.cpp
 *  Project: MakeHuman <info@makehuman.org>, http://www.makehuman.org/
 *  App    : makehuman
 *
 *  For individual developers look into the AUTHORS file.
 *
 */

// TODO: create makehuman.h
#ifdef HAVE_CONFIG_H
  #include <config.h>
#endif

#include <iostream>

#include <GL/freeglut.h>
#include <GL/gl.h>

//mhh...maybe some of these includes are retundant
#include <time.h>
#include <gui/Panel.h>
#include <gui/Widget.h>
#include <gui/Image.h>
#include <gui/ImageSlider.h>
#include <gui/Window.h>
#include <gui/Console.h>
#include <gui/Size.h>
#include <gui/Point.h>
#include <gui/CGUtilities.h>
#include <gui/Camera.h>
#include <gui/GLUTWrapper.h>
#include <animorph/Mesh.h>
#include <animorph/util.h>
#include <animorph/Vector3.h>
#ifdef DEBUG
#include <StopClock/StopClock.h>
#endif

#include "CharacterSettingPanel.h"
#include "Global.h"
#include "ToolbarPanel.h"
#include "UtilitybarPanel.h"
#include "FooterPanel.h"
#include "ViewPanel.h"
#include "TooltipPanel.h"
#include "TargetPanel.h"
#include "BottomPanel.h"
#include "SplashPanel.h"
#include "ConsoleListener.h"
#include "ComponentID.h"
#include "util.h"
#include "ConsoleCommands.h"
#include "Animation.h"

#define kTimerRendering 1000

using namespace std;
using namespace Animorph;

static void renderMesh ();
static void renderMeshEdges();
static bool loadTextures();
static void toggleVisiblePart(const std::string& name);
static void toggleSubdivision();
static void renderSubsurf ();

// console listener
static ConsoleListener       *consoleListener;
static TooltipPanel          *tooltipPanel;
static ToolbarPanel          *toolbarPanel;
static UtilitybarPanel       *utilitybarPanel;
static FooterPanel           *footerPanel;
static ViewPanel             *viewPanel;
static BottomPanel           *bottomPanel;
static SplashPanel           *splashPanel;
static Console               *console;
static Mesh                  *mesh;
static Camera                *camera;
//static Texture               *headTexture;
//static Texture               *bodyTexture;
static Animation             *animation;
static Autozoom              *autozoom;

bool init;  // shows the init status
int splashMotionCount;
float twopoints[6];
int average = 0;
int n_display = 0;
bool right_button_down = false;
int tickCount = 0;
float kTimePerRaster (0.03f);
bool waitDisplay = false;
bool oldCameraTimerTrigger = false;

static Vector3f cameraPos;

const Color border_color   (1.0, 0.55, 0.0, 0.8);
const Color grid_color (0.35, 0.50, 0.30, 0.50);
const Color edges_color (0.4, 0.3, 0.3, 0.5);

//Display function
static void display()
{
  camera->applyMatrix ();

  //cgutils::drawAxis ();
  if(Global::instance().getSubdivision() && !Global::instance().getLightMesh())
  {
    renderSubsurf ();
  }
  else
  {
    renderMesh ();
  }

/*
  if(Global::instance().getFlatShading())
    renderMeshEdges();
*/

  if(Global::instance().getQuotedBox())
    cgutils::mhWireCube(twopoints);

  Window &mainWindow(Window::instance());
  mainWindow.draw ();

  if (Global::instance().getDrawGrid ())
  {
    cgutils::drawGrid ( mainWindow.getSize(), 220, 70, grid_color, border_color, 50);
  }
  else
  {
    cgutils::drawSquare(Rect(0,0,1,1), border_color);
  }

  cgutils::swapBuffers ();

  // TODO: put this in a own function
  if (!init)
  {
    //mesh->loadTargetsFactory (searchDataDir ("targets"));
    init = true;
  }

  if(Global::instance().isRendering())
  {
    waitDisplay = true;
  }
};

//Glut callbacks
static void reshape     (int w, int h)
{
  Window &mainWindow(Window::instance());
  mainWindow.reshape            (Size(w,h), *camera);
  camera->reshape (w, h);
}

static void timerTrigger(int val)
{
    bool tmp;

	Window &mainWindow(Window::instance());

    tmp = camera->timerTrigger();
    if(Global::instance().getSubdivision())
    {
      if(!tmp && oldCameraTimerTrigger)
      {
        Global::instance().setLightMesh(false);
        cgutils::redisplay();
      }
      else if(tmp && !oldCameraTimerTrigger)
      {
        Global::instance().setLightMesh(true);
      }
      oldCameraTimerTrigger = tmp;
    }

	if (!camera->isPerspective())
	{
		reshape(mainWindow.getSize().getWidth(),mainWindow.getSize().getHeight());
	}
	mainWindow.setTimerCallback(50, timerTrigger, 1);
}

static void motion      (int x, int y)
{
  Window &mainWindow(Window::instance());
  splashPanel = (SplashPanel *)mainWindow.getPanel(kComponentID_SplashPanel);
  if(splashPanel != NULL && splashMotionCount++ >= 5)
  {
    mainWindow.removePanel(splashPanel);
    delete splashPanel;
    splashPanel = NULL;

    splashMotionCount = 0;
    cgutils::redisplay();
  }

  mainWindow.isMouseOverPanel   (Point(x,y));
}

static void timerRendering(int value)
{
  Window &mainWindow(Window::instance());

  if(Global::instance().isRendering() && waitDisplay)
  {
    waitDisplay = false;
    renderingStep();
    mainWindow.setTimerCallback(kTimerRendering, timerRendering, 0); // Rendering
  }
  else
  {
    mainWindow.setTimerCallback(kTimerRendering, timerRendering, 0); // Rendering
  }
}

static void timer (int value)
{
  Window &mainWindow(Window::instance());

  if(animation->isStarted())
  {
    if(tickCount++ >= (int)((1000 / kTimerCallback) / animation->getFrameRate()))
    {
      animation->nextFrame();
      tickCount = 0;
      cgutils::redisplay();
    }
  }
  mainWindow.setTimerCallback(kTimerCallback, timer, value);
}

static void special     (int key, int x, int y)
{
  Window &mainWindow(Window::instance());
  if (!mainWindow.isKeyTypePanel (key))
  {
    switch(key)
    {
      case GLUT_KEY_UP:
        camera->move(0, 1, 0);
        break;
      case GLUT_KEY_DOWN:
        camera->move(0, -1, 0);
        break;
      case GLUT_KEY_LEFT:
        camera->move(-1, 0, 0);
        break;
      case GLUT_KEY_RIGHT:
        camera->move(1, 0, 0);
        break;
    }
  }
  cgutils::redisplay ();
}

static void keyboard    (unsigned char key,     int x, int y)
{
  Window &mainWindow(Window::instance());
  if (!mainWindow.isKeyTypePanel (key))
  {
    switch (toupper (key)) {
    case '+':
      camera->move (0,0,1);
      break;
    case '-':
      camera->move (0,0,-1);
      break;
    case '8':
      camera->rotate (-M_PI/12, X_AXIS);
      break;
    case '2':
      camera->rotate (M_PI/12, X_AXIS);
      break;
    case '1':
      camera->resetRotation();
      //camera->resetPosition();
      //camera->rotate (-M_PI/2, X_AXIS);
      //camera->move (0,0,-75);
      break;
    case '7':
      camera->resetRotation();
      camera->rotate (M_PI/2, X_AXIS);
      break;
    case '6':
      camera->rotate (-M_PI/12, Y_AXIS);
      break;
/*
    // Disabled to toggle between perspective and orthogonal mode, because
    // Perspective mode causes problems on OS-X OpenGL and MESA...
    case '5':
	    camera->setPerspective(!camera->isPerspective());
	    reshape(mainWindow.getSize().getWidth(),mainWindow.getSize().getHeight());
      break;
*/

    case '4':
      camera->rotate (M_PI/12, Y_AXIS);
      break;
    case '3':
      camera->resetRotation();
      camera->rotate (-M_PI/2, Y_AXIS);
      break;
    case '.':
      camera->resetPosition();
      camera->move (0,0,-125);
      break;
    case 'O':
      console->open();
      break;
    case 'S':
      toggleSubdivision();
      break;
    case 'T':
      rendering(mainWindow, TOON);
      break;
    case 'H':
      toggleVisiblePart("head");
      break;
    case 'B':
      toggleVisiblePart("body");
      break;
    case 'E':
      toggleVisiblePart("zeyebrows");
      break;
    case 'K':
    {
      int ret;
      if((ret = KillProcessList() )== 0){
        mainWindow.getConsole()->open();
        mainWindow.getConsole()->printMessage("No process killed");
        cgutils::redisplay();
      }else{
        std::stringstream kk;
        kk<<ret;
        mainWindow.getConsole()->open();
        mainWindow.getConsole()->printMessage("Killed  " + kk.str() + " process " );
        cgutils::redisplay();
      }
    }
      break;
    default:
      break;
    }
  }
  cgutils::redisplay ();
}

static void mouse (int button, int state, int x, int y)
{
  Window &mainWindow(Window::instance());
  splashPanel = (SplashPanel *)mainWindow.getPanel(kComponentID_SplashPanel);
  if(splashPanel != NULL)
  {
    mainWindow.removePanel(splashPanel);
    delete splashPanel;
    splashPanel = NULL;

    splashMotionCount = 0;
    //cgutils::redisplay();
  }

  if(console->isActive())
  {
    console->isMouseClick(Point(x,y), button, state);
    return;
  }

  //cout << "mouse: " << button << endl;
  camera->mouseRotateStart (x, y);

  if(!mainWindow.isMouseClickPanel  (Point(x, y), button, state))
  {
    switch(button)
    {
    case GLUT_LEFT_BUTTON :
        if (state != GLUT_DOWN)
        {
          if(Global::instance().getSubdivision())
          {
            Global::instance().setLightMesh(false);
          }
        }
        break;

    case GLUT_MIDDLE_BUTTON :
        break;

    case GLUT_RIGHT_BUTTON :
        if (state == GLUT_DOWN)
        {
          right_button_down = true;
        }
        else
        {
          right_button_down = false;
          if(Global::instance().getSubdivision())
          {
            Global::instance().setLightMesh(false);
          }
        }
        break;

#ifdef GLUT_WHEEL_UP
    case GLUT_WHEEL_UP :   // 3
        camera->move (0,0,1);
	if (!camera->isPerspective())
	{
	reshape(mainWindow.getSize().getWidth(),mainWindow.getSize().getHeight());
	}
        break;
#endif // GLUT_WHEEL_UP

#ifdef GLUT_WHEEL_DOWN
    case GLUT_WHEEL_DOWN : // 4
        camera->move (0,0,-1);
	if (!camera->isPerspective())
	{
	reshape(mainWindow.getSize().getWidth(),mainWindow.getSize().getHeight());
	}
        break;
#endif // GLUT_WHEEL_DOWN

#ifdef GLUT_WHEEL_RIGHT
    case GLUT_WHEEL_RIGHT : // 5
        break;
#endif // GLUT_WHEEL_RIGHT

#ifdef GLUT_WHEEL_LEFT
    case GLUT_WHEEL_LEFT :  // 6
        break;
#endif // GLUT_WHEEL_LEFT
    }
  }
  //mainWindow.isMouseClickPanel  (Point(x, y), button, state);

  cgutils::redisplay ();
}

static void closeWindow()
{
  Window &mainWindow(Window::instance());
  if(console->isActive())
  {
    console->close();
  }
  console->openWithCommand(kConsoleCommand_Exit, kConsoleMessage_ConfirmExit, "");
  mainWindow.mainLoop();
}

static void activeMotion (int x, int y)
{
  if(console->isActive())
  {
    console->isMouseDragged(Point(x,y));
    return;
  }

  Window &mainWindow(Window::instance());
  if (!mainWindow.isMouseDraggedPanel(Point(x,y)))
  {
    if(right_button_down)
    {
      camera->moveMouse(x, y);
    }
    else
    {
      camera->rotateMouse (x, y);
    }

    if(Global::instance().getSubdivision())
    {
      Global::instance().setLightMesh(true);
    }

    cgutils::redisplay ();
  }
}

#if defined(__APPLE__) && defined(__MACH__)
int main_wrapper (int argc, char** argv)
#else
int main (int argc, char** argv)
#endif
{
  // On OS X this is already done!
#if !(defined(__APPLE__) && defined(__MACH__))
  //Glut init
  glutInit (&argc, argv);
#endif

  int screenWidth = cgutils::getScreenWidth();
  int screenHeight = cgutils::getScreenHeight();

  mhgui::Window::createSingelton(screenWidth > 0 ? screenWidth - 10 : 800,
                                 screenHeight > 0 ? screenHeight - 55 : 600,
                                "MakeHuman 0.9.1 RC1",
                                Color (0,0,0));
  Window &mainWindow(Window::instance());

  splashMotionCount = 0;
  createWorkingDirs();

  ParseConfigurationXML();
  ExportConfigurationXML();
  tooltipPanel          = new TooltipPanel(mainWindow.getSize().getHeight());
  toolbarPanel          = new ToolbarPanel();
  utilitybarPanel       = new UtilitybarPanel();
  footerPanel           = new FooterPanel(mainWindow.getSize().getHeight());
  viewPanel             = new ViewPanel(mainWindow.getSize().getHeight());
  bottomPanel           = new BottomPanel();
  splashPanel           = new SplashPanel(mainWindow.getSize());
  mesh                  = new Mesh();
  camera                = new Camera();
  //headTexture           = new Texture();
  //bodyTexture           = new Texture();
  animation             = new Animation();
  autozoom		        = new Autozoom();

  CharacterSettingPanel *characterSettingPanel = new CharacterSettingPanel();
  mainWindow.initWindow ();

  // TODO: put animorph mesh loading stuff in extra function

  // load mesh with factory function
  bool mesh_loaded = mesh->loadMeshFactory (searchDataFile ("base.vertices"),
                     searchDataFile ("base.faces"));
  if (!mesh_loaded)
  {
    cerr << "couldn't load mesh geometry" << endl;
    return 1;
  }

  // load material for mesh
  bool material_loaded = mesh->loadMaterialFactory (searchDataFile ("base.materials"),
                         searchDataFile ("base.colors"));
  if (!material_loaded)
  {
    cerr << "couldn't load mesh material informations" << endl;
    return 1;
  }

  // load UV texture data, currently no Factory function
  TextureVector &texturevector = mesh->getTextureVectorRef ();
  texturevector.load (searchDataFile ("base.uv"));

  // load face groups with factory function
  bool groups_loaded = mesh->loadGroupsFactory (searchDataFile ("base.parts"));
  if (!groups_loaded)
  {
    cerr << "couldn't load face groups" << endl;
    return 1;
  }

  // load subdivided face groups with factory function
  bool subd_groups_loaded = mesh->loadSubdGroupsFactory (searchDataFile ("subd.parts"));
  if (!subd_groups_loaded)
  {
    cerr << "couldn't load subdivided face groups" << endl;
    return 1;
  }

  // load skin info with factory function
  bool skin_loaded = mesh->loadSkinFactory (searchDataFile ("base.skin"));
  if (!skin_loaded)
  {
    cerr << "couldn't load skin info" << endl;
    return 1;
  }

  // load skin info with factory function
  bool clothes_loaded = mesh->loadClothesFactory (searchDataFile ("base.clothes"));
  if (!clothes_loaded)
  {
    cerr << "couldn't load clothes info" << endl;
    return 1;
  }

  // load skin info with factory function
  bool edges_loaded = mesh->loadEdgeStripFactory (searchDataFile ("base.strips"));
  if (!edges_loaded)
  {
    cerr << "couldn't load edges info" << endl;
    return 1;
  }

  // load smooth info with factory function
  bool smooth_loaded = mesh->loadSmoothVertexFactory (searchDataFile ("base.smooth"));
  if (!smooth_loaded)
  {
    cerr << "couldn't load smooth info" << endl;
    return 1;
  }

  // load subdivision surfaces info with factory function
  bool subd_loaded = mesh->loadSubdFactory (searchDataFile ("base.subde"),searchDataFile ("base.subdo"),searchDataFile ("base.subdf"));
  if (!subd_loaded)
  {
    cerr << "couldn't load subsurf info" << endl;
    return 1;
  }

  // put mesh container into the Global Singleton
  Global::instance().setMesh (mesh);

  // put camera into the Global Singleton
  Global::instance().setCamera (camera);

  // put meshTexture into the Global Singleton
  //Global::instance().setBodyTexture (bodyTexture);
  //Global::instance().setHeadTexture (headTexture);

  // put animation into the Global Singleton
  Global::instance().setAnimation (animation);

  // put autozoom into the Global Singleton
  Global::instance().setAutozoom (autozoom);

  mesh->loadTargetsFactory (searchDataDir ("targets"));
  mesh->loadTargetsFactory (searchDataDir ("selectors"), 1, true, false);
  mesh->loadPoseTargetsFactory(searchDataDir ("rotations"));
  mesh->loadCharactersFactory(searchDataDir ("bs_data"));
  //mesh->loadCharactersFactory(searchDataDir ("my"));

  init = false;

  consoleListener = new ConsoleListener();
  //Add console
  console = new Console (FOUR_CHAR_CONST('C','O','N','1'));
  console->setListener (consoleListener);

  console->loadPNG (searchPixmapFile ("ui/console.png"));
  console->addSplashLine ("MakeHuman operating console");
  console->addSplashLine ("version 0.1");
  console->addSplashLine ("2005-2007");
  console->addSplashLine ("(press ESC to exit)");
  console->addSplashLine ("- - - - - - - - - - - - - -");
  console->setActive(false);
  mainWindow.setConsole (console);
  mainWindow.setCamera (camera);

  //Add panels to mainwindow
  mainWindow.addPanel (tooltipPanel);
  mainWindow.addPanel (toolbarPanel);
  mainWindow.addPanel (utilitybarPanel);
  mainWindow.addPanel (footerPanel);
  mainWindow.addPanel (viewPanel);
  mainWindow.addPanel (bottomPanel);
  mainWindow.addPanel (characterSettingPanel);
  mainWindow.addPanel (splashPanel);

  // set initial camera position
  //camera->rotate (-M_PI/2, X_AXIS);
  camera->move (0,0,-125.0f);

  // create after adding
  tooltipPanel->createWidgets ();
  toolbarPanel->createWidgets ();
  utilitybarPanel->createWidgets ();
  footerPanel->createWidgets ();
  viewPanel->createWidgets ();
  bottomPanel->createWidgets();
  characterSettingPanel->createWidgets();
  splashPanel->createWidgets();

  mainWindow.loadPNG (searchPixmapFile ("ui/background.png"));

  //Activate the images textures
  //Note it's after context creation
  toolbarPanel->show_all ();
  utilitybarPanel->show_all ();
  footerPanel->show_all ();
  viewPanel->show_all();
  tooltipPanel->show_all();
  bottomPanel->show_all();
  characterSettingPanel->show_all();
  splashPanel->show_all();

  if(loadTextures())
  {
    Global::instance().setTexture (true);
    Global::instance().setCanTexture (true);
  }

  //Glut callbacks
  mainWindow.setDisplayCallback(display);
  mainWindow.setReshapeCallback(reshape);
  mainWindow.setPassiveMotionCallback(motion);
  mainWindow.setKeyboardCallback(keyboard);
  mainWindow.setMouseCallback(mouse);
  mainWindow.setMotionCallback(activeMotion);
  mainWindow.setSpecialCallback(special);
  mainWindow.setTimerCallback(kTimerCallback, timer, 0); // Animation
  mainWindow.setTimerCallback(1000,timerTrigger,1); // Autozoom
  mainWindow.setTimerCallback(kTimerRendering, timerRendering, 0); // Rendering
#ifdef USE_FREEGLUT
  mainWindow.setCloseCallback(closeWindow);
#endif

  console->show ();
  mainWindow.show ();

#ifdef USE_FREEGLUT
  glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
#endif

  ::glPolygonOffset( 1.0, 1.0 );

  loadDefaultBodySettings();
  mesh->calcSubsurf();

  mainWindow.mainLoop();

  // we never reach this point.
  return 0;
}

//function to find the xyz values of the bounding box
void calcMinMax(const Vector3f &coords)
{
  if (coords.x < twopoints[0])
  {
    twopoints[0] = coords.x;
  }
  else if (coords.x > twopoints[3])
  {
    twopoints[3] = coords.x;
  }

  if (coords.y < twopoints[1])
  {
    twopoints[1] = coords.y;
  }
  else if (coords.y > twopoints[4])
  {
    twopoints[4] = coords.y;
  }

  if (coords.z < twopoints[2])
  {
    twopoints[2] = coords.z;
  }
  else if (coords.z > twopoints[5])
  {
    twopoints[5] = coords.z;
  }
}

void renderMeshEdges()
{
  const EdgeStrip     &edgestrip    (mesh->getEdgeStripRef ());
  const VertexVector   &vertexvector  (animation->isLoaded() ? animation->getMesh().getVertexVectorRef() :
                                                               mesh->getVertexVectorRef ());

  EdgeStrip::const_iterator i;

  glColor4f(edges_color.red (), edges_color.green (), edges_color.blue (), edges_color.alpha ());
  // the same as above, but ~5fps faster:
	//GLfloat edgescolor[]={0.75, 0.75, 0.75, 1.0};
	//glColor4fv(edgescolor);
	// not used for "consistency of the code"

  cgutils::enableBlend ();
  cgutils::enableLineSmoothing ();
  glDisable (GL_LIGHTING);

  ::glEnable(GL_POLYGON_OFFSET_FILL);

  for (i = edgestrip.begin(); i != edgestrip.end(); ++i)
  {
    const StripData &stripdata(*i);

    StripData::const_iterator j;

    ::glBegin (GL_LINE_STRIP);
    for (j = stripdata.begin(); j != stripdata.end(); ++j)
    {
      const Vertex &vertex = vertexvector[*j];
      ::glVertex3fv (vertex.co.getAsOpenGLVector());
    }
    ::glEnd ();
  } //   for (i = facevector.begin(); i != facevector.end(); ++i)

  ::glDisable(GL_POLYGON_OFFSET_FILL);
  glEnable (GL_LIGHTING);
  cgutils::disableLineSmoothing ();
  cgutils::disableBlend();
}

void renderFace(const Face &face,
                const MaterialVector &materialvector,
                const VertexVector &vertexvector,
                const TextureFace &texture_face)
{
  unsigned int facesize = face.getSize();

  size_t j;

  int material_index = face.getMaterialIndex ();
  if (material_index != -1)
  {
    const Material &material(materialvector[material_index]);
    const Color    &color   (material.getRGBCol ());

    // Set the color for these vertices
    ::glColor4fv  (color.getAsOpenGLVector());
  }

  for (j = 0; j != facesize; ++j)
  {
    const Vertex &vertex = vertexvector[face.getVertexAtIndex(j)];
    const Vector2f &uv = texture_face[j];
/*
    if(Global::instance().getFlatShading())
    {
      ::glNormal3fv (face.no.getAsOpenGLVector());
    }
    else
    {
*/
      ::glNormal3fv (vertex.no.getAsOpenGLVector());
//    }

    if(Global::instance().getTexture() && !Global::instance().getLightMesh())
    {
      ::glTexCoord2f (uv.x, uv.y);
    }
    ::glVertex3fv (vertex.co.getAsOpenGLVector());

    if(Global::instance().getQuotedBox())
    {
      calcMinMax(vertex.co);
    }
  } // for (j = 0; j != facesize; ++j)
}

void renderClothes()
{
  const MaterialVector &materialvector(mesh->getMaterialVectorRef ());
  const TextureVector  &texturevector (mesh->getTextureVectorRef ());

  const FaceVector     &facevector    (animation->isLoaded() ? animation->getMesh().getFaceVectorRef() :
                                                               mesh->getFaceVectorRef ());

  const VertexVector   &vertexvector  (animation->isLoaded() ? animation->getMesh().getVertexVectorRef() :
                                                               mesh->getVertexVectorRef ());

  FaceGroup            &clothesgroup     (animation->isLoaded() ? animation->getMesh().getClothesGroupRef() :
                                                                  mesh->getClothesGroupRef());

  clothesgroup.calcVertexes(facevector);

  int facesize;
  int istri = -1; //to remember which type was the latest drawn geometry and avoid too many glBegin

  for (FaceGroup::const_iterator clothesgroup_it = clothesgroup.begin ();
       clothesgroup_it != clothesgroup.end ();
       clothesgroup_it++)
  {
    FGroup clothes = (*clothesgroup_it).second;
    if(!clothes.visible) continue;

    FGroupData &groupdata = clothes.facesIndexes;
    for (unsigned int i = 0; i < groupdata.size (); i++)
    {
      const Face &face(facevector[clothes.facesIndexes[i]]);
      const TextureFace &texture_face = texturevector[clothes.facesIndexes[i]];

      facesize = face.getSize();
      if(istri != facesize)
      {
        if(istri != -1)
        {
          ::glEnd();
        }

        ::glBegin(facesize == 4 ? GL_QUADS : GL_TRIANGLES);
        istri = facesize;
      }
      renderFace(face, materialvector, vertexvector, texture_face);
    } //   for (i = facevector.begin(); i != facevector.end(); ++i)
  ::glEnd();
  }
}

void toggleVisiblePart(const std::string& name)
{
  FaceGroup      &facegroup           (mesh->getFaceGroupRef ());
  facegroup.toggleVisible(name);

  FaceGroup      &subd_facegroup           (mesh->getSubdFaceGroupRef ());
  subd_facegroup.toggleVisible(name);
}

void toggleSubdivision()
{
  Global::instance().setSubdivision(!Global::instance().getSubdivision());

  if(Global::instance().getSubdivision())
  {
    mesh->calcSubsurf();
  }
}

void renderMesh ()
{
  const MaterialVector &materialvector(mesh->getMaterialVectorRef ());
  const TextureVector  &texturevector (mesh->getTextureVectorRef ());

  const FaceVector     &facevector    (animation->isLoaded() ? animation->getMesh().getFaceVectorRef() :
                                                               mesh->getFaceVectorRef ());

  const VertexVector   &vertexvector  (animation->isLoaded() ? animation->getMesh().getVertexVectorRef() :
                                                               mesh->getVertexVectorRef ());

  FaceGroup      &facegroup           (mesh->getFaceGroupRef ());

  int facesize;
  int istri = -1; //to remember which type was the latest drawn geometry and avoid too many glBegin

  if(Global::instance().getQuotedBox())
  {
    for(int i = 0; i < 6; i++)
    {
      twopoints[i] = 0;
    }
  }

  if(Global::instance().getTexture() && !Global::instance().getLightMesh())
  {
    ::glEnable(GL_TEXTURE_2D);
  }

  cgutils::enableBlend ();

  for (FaceGroup::iterator facegroup_it = facegroup.begin ();
       facegroup_it != facegroup.end ();
       facegroup_it++)
  {

    if( (*facegroup_it).second.visible == false)
      continue;

    if(Global::instance().getTexture() && !Global::instance().getLightMesh())
    {
      ::glBindTexture(GL_TEXTURE_2D, Global::instance().getTexture((*facegroup_it).first)->getTextureIdOfXY(0, 0));
    }

    FGroupData &groupdata = (*facegroup_it).second.facesIndexes;

    for (unsigned int i = 0; i < groupdata.size (); i++)
    {
      const Face &face(facevector[groupdata[i]]);
      const TextureFace &texture_face = texturevector[groupdata[i]];

      facesize = face.getSize();

      if(istri != facesize)
      {
        if(istri != -1)
        {
          ::glEnd();
        }

        ::glBegin(facesize == 4 ? GL_QUADS : GL_TRIANGLES);
        istri = facesize;
      }
      renderFace(face, materialvector, vertexvector, texture_face);
    }

    ::glEnd();
    istri = -1;
  }

  renderClothes();

  if(Global::instance().getTexture() && !Global::instance().getLightMesh())
  {
    ::glDisable(GL_TEXTURE_2D);
  }
  cgutils::disableBlend ();
}

bool loadTextures()
{
  FaceGroup      &facegroup           (mesh->getFaceGroupRef ());

  for (FaceGroup::iterator facegroup_it = facegroup.begin ();
       facegroup_it != facegroup.end ();
       facegroup_it++)
  {
    string name((*facegroup_it).first);
    Global::instance().setTexture(name, new Texture());
    if(!Global::instance().getTexture(name)->load(searchPixmapFile ("ui/" + name + "_color.png")))
    {
      cerr << "couldn't load base skin_color Texture Data " << name << "_color.png" << endl;
      return false;
    }
  }

  return true;
}

void renderSubsurf()
{
  const FaceVector     &facevector_subd    (mesh->getFaceVectorSubdRef ());
  const subdVertexVector     &vertexvector_subd_f    (mesh->getVertexVectorSubdFRef ());
  const subdVertexVector     &vertexvector_subd_e    (mesh->getVertexVectorSubdERef ());
  const origVertexVector     &vertexvector_subd_o    (mesh->getVertexVectorSubdORef ());
  const MaterialVector &materialvector(mesh->getMaterialVectorRef ());

  FaceGroup      &facegroup           (mesh->getSubdFaceGroupRef ());

  if(Global::instance().getQuotedBox())
  {
    for(int i = 0; i < 6; i++)
    {
      twopoints[i] = 0;
    }
  }

  ::glBegin(Global::instance().getLightMesh() ? GL_POINTS : GL_QUADS);

  for (FaceGroup::iterator facegroup_it = facegroup.begin ();
       facegroup_it != facegroup.end ();
       facegroup_it++)
  {

    if( (*facegroup_it).second.visible == false)
      continue;

    FGroupData &groupdata = (*facegroup_it).second.facesIndexes;

    for (unsigned int i = 0; i < groupdata.size (); i++)
    {
      //if(Global::instance().getLightMesh() && i % 10 != 0) continue;
      const Face &face(facevector_subd[groupdata[i]]);

      int material_index = face.getMaterialIndex ();
      if (material_index != -1)
      {
        const Material &material(materialvector[material_index]);
        const Color    &color   (material.getRGBCol ());

        // Set the color for these vertices
        ::glColor4fv  (color.getAsOpenGLVector());
      }

      //::glNormal3fv ((*f_it).no.getAsOpenGLVector());

      ::glNormal3fv (vertexvector_subd_o[face.getVertexAtIndex(0)].no.getAsOpenGLVector());
      ::glVertex3fv (vertexvector_subd_o[face.getVertexAtIndex(0)].co.getAsOpenGLVector());

      ::glNormal3fv (vertexvector_subd_e[face.getVertexAtIndex(1)].no.getAsOpenGLVector());
      ::glVertex3fv (vertexvector_subd_e[face.getVertexAtIndex(1)].co.getAsOpenGLVector());

      ::glNormal3fv (vertexvector_subd_f[face.getVertexAtIndex(2)].no.getAsOpenGLVector());
      ::glVertex3fv (vertexvector_subd_f[face.getVertexAtIndex(2)].co.getAsOpenGLVector());

      ::glNormal3fv (vertexvector_subd_e[face.getVertexAtIndex(3)].no.getAsOpenGLVector());
      ::glVertex3fv (vertexvector_subd_e[face.getVertexAtIndex(3)].co.getAsOpenGLVector());

      if(Global::instance().getQuotedBox())
      {
        calcMinMax(vertexvector_subd_o[face.getVertexAtIndex(0)].co);
      }
    }
  }

  ::glEnd();

}
