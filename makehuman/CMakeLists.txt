cmake_minimum_required(VERSION 3.5)

set(gui_sources
	src/gui/AbstractListener.h
	src/gui/Camera.h
	src/gui/Camera.cpp
	src/gui/CGUtilities.h
	src/gui/CGUtilities.cpp
	src/gui/Component.h
	src/gui/Component.cpp
	src/gui/Console.h
	src/gui/Console.cpp
	src/gui/GLUTWrapper.h
	src/gui/Image.h
	src/gui/Image.cpp
	src/gui/ImageData.h
	src/gui/ImageData.cpp
	src/gui/ImageSlider.h
	src/gui/ImageSlider.cpp
	src/gui/ImageSliderSysListener.h
	src/gui/ImageSliderSysListener.cpp
	src/gui/ImageSysListener.h
	src/gui/ImageSysListener.cpp
	src/gui/Label.h
	src/gui/Label.cpp
	src/gui/MultiPanel.h
	src/gui/MultiPanel.cpp
	src/gui/Panel.h
	src/gui/Panel.cpp
	src/gui/Point.h
	src/gui/Point.cpp
	src/gui/Rect.h
	src/gui/Rect.cpp
	src/gui/Selector.h
	src/gui/Selector.cpp
	src/gui/SelectorSysListener.h
	src/gui/SelectorSysListener.cpp
	src/gui/Size.h
	src/gui/Size.cpp
	src/gui/TextEntry.h
	src/gui/TextEntry.cpp
	src/gui/TextEntrySysListener.h
	src/gui/TextEntrySysListener.cpp
	src/gui/Texture.h
	src/gui/Texture.cpp
	src/gui/Tooltip.h
	src/gui/Tooltip.cpp
	src/gui/Widget.h
	src/gui/Widget.cpp
	src/gui/Window.h
	src/gui/Window.cpp
)

set(makehuman_sources
	src/Animation.cpp
	src/Animation.h
	src/BodyPanel.cpp
	src/BodyPanel.h
	src/BottomPanel.cpp
	src/BottomPanel.h
	src/BsCategoryPanel.cpp
	src/BsCategoryPanel.h
	src/BsPanel.cpp
	src/BsPanel.h
	src/BsPanelSelectionListener.cpp
	src/BsPanelSelectionListener.h
	src/BsSelectionListener.cpp
	src/BsSelectionListener.h
	src/CharacterSettingPanel.cpp
	src/CharacterSettingPanel.h
	src/ClothesPanel.cpp
	src/ClothesPanel.h
	src/ComponentID.h
	src/ConsoleCommands.h
	src/ConsoleListener.cpp
	src/ConsoleListener.h
	src/FacePanel.cpp
	src/FacePanel.h
	src/FileTools.cpp
	src/FileTools.h
	src/FooterPanel.cpp
	src/FooterPanel.h
	src/Global.cpp
	src/Global.h
	src/HandsPanel.cpp
	src/HandsPanel.h
	src/ImageButton.h
	src/ImageListener.cpp
	src/ImageListener.h
	src/makehuman.cpp
	src/PageBsPanel.cpp
	src/PageBsPanel.h
	src/PagePosesBsPanel.cpp
	src/PagePosesBsPanel.h
	src/PageTargetPanel.cpp
	src/PageTargetPanel.h
	src/PosesBodyPanel.cpp
	src/PosesBodyPanel.h
	src/PosesBsCategoryPanel.cpp
	src/PosesBsCategoryPanel.h
	src/PosesBsPanel.cpp
	src/PosesBsPanel.h
	src/PosesBsPanelSelectionListener.cpp
	src/PosesBsPanelSelectionListener.h
	src/PosesBsSelectionListener.cpp
	src/PosesBsSelectionListener.h
	src/PoseTargetPanel.cpp
	src/PoseTargetPanel.h
	src/PoseTargetSelectionListener.cpp
	src/PoseTargetSelectionListener.h
	src/PoseTargetSliderListener.cpp
	src/PoseTargetSliderListener.h
	src/SelectorListener.cpp
	src/SelectorListener.h
	src/SplashPanel.cpp
	src/SplashPanel.h
	src/TargetPanel.cpp
	src/TargetPanel.h
	src/TargetSelectionListener.cpp
	src/TargetSelectionListener.h
	src/TargetSlider.h
	src/TargetSliderListener.cpp
	src/TargetSliderListener.h
	src/TeethPanel.cpp
	src/TeethPanel.h
	src/ToolbarPanel.cpp
	src/ToolbarPanel.h
	src/TooltipPanel.cpp
	src/TooltipPanel.h
	src/util.cpp
	src/util.h
	src/UtilitiesPanel.cpp
	src/UtilitiesPanel.h
	src/UtilitiesSliderListener.cpp
	src/UtilitiesSliderListener.h
	src/UtilitybarPanel.cpp
	src/UtilitybarPanel.h
	src/ViewPanel.cpp
	src/ViewPanel.h
)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(PNG REQUIRED)

add_executable(makehuman ${gui_sources} ${makehuman_sources})

target_include_directories(makehuman PRIVATE src)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${PNG_INCLUDE_DIRS})
target_link_libraries(makehuman PRIVATE animorph ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${PNG_LIBRARY})

file(COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY pixmaps DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
