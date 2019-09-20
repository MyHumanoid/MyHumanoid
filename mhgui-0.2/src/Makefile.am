## Process this file with automake to produce Makefile.in
## Created by Anjuta - will *NOT* be overwritten

INCLUDES =

AM_CXXFLAGS = @PACKAGE_CFLAGS@\
	      @GLUT_CFLAGS@\
	      @FREEGLUT_CFLAGS@\
	      @PNG_CFLAGS@\
	     -Wall

lib_LTLIBRARIES = libmhgui.la

libmhgui_la_SOURCES = \
    CGUtilities.cpp   \
    Console.cpp       \
    Image.cpp         \
    ImageSlider.cpp   \
    ImageSliderSysListener.cpp\
    ImageSysListener.cpp\
    Panel.cpp         \
    MultiPanel.cpp    \
    Window.cpp        \
    ImageData.cpp     \
    Component.cpp     \
    Widget.cpp        \
    Tooltip.cpp       \
    Size.cpp          \
    Point.cpp         \
    Camera.cpp	  \
    Rect.cpp	  \
    TextEntry.cpp	  \
    TextEntrySysListener.cpp\
    Label.cpp\
    Texture.cpp\
    Selector.cpp	\
    SelectorSysListener.cpp

libmhgui_la_LDFLAGS = 

libmhgui_la_LIBADD = @PACKAGE_LIBS@\
		     @GLUT_LIBS@\
		     @FREEGLUT_LIBS@\
		     @PNG_LIBS@
