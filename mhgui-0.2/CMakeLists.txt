cmake_minimum_required(VERSION 3.5)

set(mhgui_headers
	include/mhgui/AbstractListener.h
	include/mhgui/Camera.h
	include/mhgui/CGUtilities.h
	include/mhgui/Component.h
	include/mhgui/Console.h
	include/mhgui/GLUTWrapper.h
	include/mhgui/ImageData.h
	include/mhgui/Image.h
	include/mhgui/ImageSlider.h
	include/mhgui/ImageSliderSysListener.h
	include/mhgui/ImageSysListener.h
	include/mhgui/Label.h
	include/mhgui/Makefile.am
	include/mhgui/Makefile.in
	include/mhgui/MultiPanel.h
	include/mhgui/Panel.h
	include/mhgui/Point.h
	include/mhgui/Rect.h
	include/mhgui/Selector.h
	include/mhgui/SelectorSysListener.h
	include/mhgui/Size.h
	include/mhgui/TextEntry.h
	include/mhgui/TextEntrySysListener.h
	include/mhgui/Texture.h
	include/mhgui/Tooltip.h
	include/mhgui/Widget.h
	include/mhgui/Window.h
)

set(mhgui_sources
	src/Camera.cpp
	src/CGUtilities.cpp
	src/Component.cpp
	src/Console.cpp
	src/Image.cpp
	src/ImageData.cpp
	src/ImageSlider.cpp
	src/ImageSliderSysListener.cpp
	src/ImageSysListener.cpp
	src/Label.cpp
	src/MultiPanel.cpp
	src/Panel.cpp
	src/Point.cpp
	src/Rect.cpp
	src/Selector.cpp
	src/SelectorSysListener.cpp
	src/Size.cpp
	src/TextEntry.cpp
	src/TextEntrySysListener.cpp
	src/Texture.cpp
	src/Tooltip.cpp
	src/Widget.cpp
	src/Window.cpp
)

set(SOURCES
	${mhgui_headers}
	${mhgui_sources}
)

find_package(PNG REQUIRED)

add_library(mhgui "${SOURCES}")
include_directories(mhgui ${PNG_INCLUDE_DIRS})
target_link_libraries(mhgui PRIVATE animorph ${PNG_LIBRARY})
target_include_directories(mhgui PUBLIC include)
