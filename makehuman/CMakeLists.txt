cmake_minimum_required(VERSION 3.10)

set(animorph_sources
	src/animorph/BodySettings.h
	src/animorph/BodySettings.cpp
	src/animorph/DirectoryList.h
	src/animorph/DirectoryList.cpp
	src/animorph/Face.h
	src/animorph/Face.cpp
	src/animorph/FileReader.h
	src/animorph/FileReader.cpp
	src/animorph/ObjExporter.h
	src/animorph/ObjExporter.cpp
	src/animorph/PoseSemiTarget.h
	src/animorph/PoseSemiTarget.cpp
	src/animorph/Target.h
	src/animorph/Target.cpp
	src/animorph/Vertex.h
	src/animorph/Vertex.cpp
	src/animorph/xmlParser.h
	src/animorph/xmlParser.cpp
	src/animorph/ColladaExporter.h
	src/animorph/ColladaExporter.cpp
	src/animorph/FaceGroup.h
	src/animorph/FaceGroup.cpp
	src/animorph/FileWriter.h
	src/animorph/FileWriter.cpp
	src/animorph/Matrix.h
	src/animorph/Matrix.cpp
	src/animorph/PoseTarget.h
	src/animorph/PoseTarget.cpp
	src/animorph/RawMesh.h
	src/animorph/RawMesh.cpp
	src/animorph/TextureVector.h
	src/animorph/TextureVector.cpp
	src/animorph/VertexGroup.h
	src/animorph/VertexGroup.cpp
	src/animorph/Color.h
	src/animorph/Color.cpp
	src/animorph/Euler.h
	src/animorph/FaceVector.h
	src/animorph/FaceVector.cpp
	src/animorph/Material.h
	src/animorph/Material.cpp
	src/animorph/Mesh.h
	src/animorph/Mesh.cpp
	src/animorph/PoseRotation.h
	src/animorph/PoseRotation.cpp
	src/animorph/PoseTranslation.h
	src/animorph/PoseTranslation.cpp
	src/animorph/SmoothVertex.h
	src/animorph/SmoothVertex.cpp
	src/animorph/util.h
	src/animorph/util.cpp
	src/animorph/VertexVector.h
	src/animorph/VertexVector.cpp
)

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
	src/render/GlTypes.h
	src/render/RenderUtils.h
	src/render/RenderUtils.cpp
	src/render/Shader.h
	src/render/Shader.cpp
)

set(log_sources
	src/log/log.h
	src/log/log.cpp
	src/log/Profiler.h
	src/log/Profiler.cpp
)

set(makehuman_sources
	src/BodyPanel.cpp
	src/BodyPanel.h
	src/CharacterSettingPanel.cpp
	src/CharacterSettingPanel.h
	src/ComponentID.h
	src/FacePanel.cpp
	src/FacePanel.h
	src/Global.cpp
	src/Global.h
	src/HandsPanel.cpp
	src/HandsPanel.h
	src/ImageButton.h
	src/ImageListener.cpp
	src/ImageListener.h
	src/MhConfig.h
	src/MhConfig.cpp
	src/MhMain.cpp
	src/MhRender.h
	src/MhRender.cpp
	src/MhUi.h
	src/MhUiCommon.h
	src/MhUiMorph.h
	src/MhUiMorph.cpp
	src/MhUiPose.h
	src/MhUiPose.cpp
	src/PageTargetPanel.cpp
	src/PageTargetPanel.h
	src/SelectorListener.cpp
	src/SelectorListener.h
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
	src/Vfs.h
	src/Vfs.cpp
	src/util/StringUtils.h
)

set(shader_sources
	shader/background.vert
	shader/background.frag
	shader/body.vert
	shader/body.frag
	shader/body_2.vert
	shader/body_2.frag
)


find_package(OpenGL     REQUIRED)
find_package(GLEW       REQUIRED)
find_package(GLUT       REQUIRED)
find_package(glm        REQUIRED)
find_package(PNG        REQUIRED)
find_package(fmt    4.0 REQUIRED)
find_package(PhysFS 2.1 REQUIRED)

add_executable(makehuman
	${animorph_sources}
	${gui_sources}
	${log_sources}
	${render_sources}
	${makehuman_sources}
	${shader_sources}
)
set_target_properties(makehuman PROPERTIES
	CXX_STANDARD 17
#	CXX_EXTENSIONS OFF
#	INTERPROCEDURAL_OPTIMIZATION TRUE
)
target_compile_features(makehuman PRIVATE
	cxx_std_17
)



#target_compile_options(makehuman PRIVATE -Wall -Wextra -pedantic)

target_include_directories(makehuman PRIVATE
	${OPENGL_INCLUDE_DIRS}
	${GLEW_INCLUDE_DIRS}
	${GLUT_INCLUDE_DIRS}
	${GLM_INCLUDE_DIRS}
	${PNG_INCLUDE_DIRS}
	${PHYSFS_INCLUDE_DIRS}
	${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(makehuman PRIVATE
	fmt::fmt
	imgui
	stb_image
	simpleini
	nativefiledialog
	OpenGL::OpenGL
	GLEW::GLEW
	GLUT::GLUT
	${PHYSFS_LIBRARY}
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
