cmake_minimum_required(VERSION 3.10)

set(gui_sources
	src/gui/AbstractListener.h
	src/gui/Camera.h
	src/gui/Camera.cpp
	src/gui/CGUtilities.h
	src/gui/CGUtilities.cpp
	src/gui/Component.h
	src/gui/Component.cpp
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
	src/gui/Texture.h
	src/gui/Texture.cpp
	src/gui/Tooltip.h
	src/gui/Tooltip.cpp
	src/gui/Widget.h
	src/gui/Widget.cpp
	src/gui/Window.h
	src/gui/Window.cpp
)

set(render_sources
	src/render/DebugGl.h
	src/render/DebugGl.cpp
	src/render/Shader.h
	src/render/Shader.cpp
)

set(log_sources
	src/log/log.h
	src/log/log.cpp
)

set(makehuman_sources
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
	src/ComponentID.h
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
)

set(shader_sources
	shader/body.vert
	shader/body.frag
)


find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLUT REQUIRED)
find_package(PNG REQUIRED)
find_package(fmt REQUIRED)

add_executable(makehuman
	${gui_sources}
	${log_sources}
	${render_sources}
	${makehuman_sources}
	${shader_sources}
)

set_target_properties(makehuman PROPERTIES
	CXX_STANDARD 17
#	CXX_EXTENSIONS OFF
	INTERPROCEDURAL_OPTIMIZATION TRUE
)

#target_compile_options(makehuman PRIVATE -Wall -Wextra -pedantic)

target_include_directories(makehuman PRIVATE
	${OPENGL_INCLUDE_DIRS}
	${GLEW_INCLUDE_DIRS}
	${GLUT_INCLUDE_DIRS}
	${PNG_INCLUDE_DIRS}
	${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(makehuman PRIVATE
	stdc++fs
	fmt
	animorph
	imgui
	nlohmann-json
	stb_image
	OpenGL::OpenGL
	GLEW::GLEW
	GLUT::GLUT
	${PNG_LIBRARY}
)

#file(CREATE_LINK
	#${CMAKE_CURRENT_SOURCE_DIR}/pixmaps
	#${CMAKE_CURRENT_BINARY_DIR}/pixmaps
	#SYMBOLIC
#)
macro(create_symlink filepath sympath)
	execute_process(COMMAND "${CMAKE_COMMAND}" -E create_symlink ${filepath} ${sympath})
endmacro(create_symlink)

create_symlink(
	${CMAKE_CURRENT_SOURCE_DIR}/data
	${CMAKE_CURRENT_BINARY_DIR}/data
)

create_symlink(
	${CMAKE_CURRENT_SOURCE_DIR}/pixmaps
	${CMAKE_CURRENT_BINARY_DIR}/pixmaps
	
)

create_symlink(
	${CMAKE_CURRENT_SOURCE_DIR}/shader
	${CMAKE_CURRENT_BINARY_DIR}/shader
	
)
