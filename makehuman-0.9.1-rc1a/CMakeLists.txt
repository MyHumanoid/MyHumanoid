cmake_minimum_required(VERSION 3.5)

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

add_executable(makehuman "${makehuman_sources}")
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS})
target_link_libraries(makehuman PRIVATE animorph mhgui ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})

file(COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY pixmaps DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
