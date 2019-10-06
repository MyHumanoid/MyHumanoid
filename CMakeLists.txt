cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

if(WIN32)
	# MSVC runtime library flags are selected by an abstraction.
	cmake_policy(SET CMP0091 NEW)
endif()

project(MyHumanoid
	VERSION 0.1.0
	DESCRIPTION MyHumanoid
	LANGUAGES CXX
)

#set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

add_subdirectory(extern)

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
	src/gui/Panel.h
	src/gui/Panel.cpp
	src/gui/Rect.h
	src/gui/Rect.cpp
	src/gui/Selector.h
	src/gui/Selector.cpp
	src/gui/Texture.h
	src/gui/Texture.cpp
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

set(myhumanoid_sources
	src/ComponentID.h
	src/CompositeMorph.h
	src/CompositeMorph.cpp
	src/GlInclude.h
	src/Global.cpp
	src/Global.h
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
	src/util.cpp
	src/util.h
	src/Vfs.h
	src/Vfs.cpp
	src/util/StringUtils.h
)

set(shader_sources
	data/shader/background.vert
	data/shader/background.frag
	data/shader/body.vert
	data/shader/body.frag
	data/shader/body_2.vert
	data/shader/body_2.frag
)


find_package(OpenGL     REQUIRED)
find_package(GLEW       REQUIRED)
find_package(GLUT       REQUIRED)
find_package(glm        REQUIRED)
find_package(PNG        REQUIRED)
find_package(PhysFS 2.1 REQUIRED)

add_executable(MyHumanoid
	${animorph_sources}
	${gui_sources}
	${log_sources}
	${render_sources}
	${myhumanoid_sources}
	${shader_sources}
)
set_target_properties(MyHumanoid PROPERTIES
	CXX_STANDARD 17
#	CXX_EXTENSIONS OFF
#	INTERPROCEDURAL_OPTIMIZATION TRUE
)
target_compile_features(MyHumanoid PRIVATE
	cxx_std_17
)

if(UNIX)
	#target_link_libraries(MyHumanoid -static)
else()
	#target_compile_options(MyHumanoid PUBLIC/PRIVATE /MT)
	#target_link_options(MyHumanoid PUBLIC/PRIVATE /INCREMENTAL:NO /NODEFAULTLIB:MSVCRT)
endif()

target_include_directories(MyHumanoid PRIVATE
	${OPENGL_INCLUDE_DIRS}
	${GLEW_INCLUDE_DIRS}
	${GLUT_INCLUDE_DIRS}
	${GLM_INCLUDE_DIRS}
	${PNG_INCLUDE_DIRS}
	${PHYSFS_INCLUDE_DIRS}
	${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(MyHumanoid PRIVATE
	${OPENGL_LIBRARIES}
	${GLEW_LIBRARIES}
	GLUT::GLUT
	${PNG_LIBRARY}
	${PHYSFS_LIBRARY}
	fmt
	imgui
	stb_image
	simpleini
	nativefiledialog
)

#file(CREATE_LINK
	#${CMAKE_CURRENT_SOURCE_DIR}/pixmaps
	#${CMAKE_CURRENT_BINARY_DIR}/pixmaps
	#SYMBOLIC
#)
macro(create_symlink filepath sympath)
	execute_process(COMMAND "${CMAKE_COMMAND}" -E create_symlink ${filepath} ${sympath})
endmacro(create_symlink)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	create_symlink(
		${CMAKE_CURRENT_SOURCE_DIR}/data
		${CMAKE_CURRENT_BINARY_DIR}/data
	)
endif()
