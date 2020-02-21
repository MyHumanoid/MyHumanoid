# SPDX-License-Identifier: GPL-3.0-or-later
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

include(${CMAKE_CURRENT_LIST_DIR}/ProjectVersion.cmake)

set(project_version ${project_version_major}.${project_version_minor}.${project_version_patch})

if(WIN32)
	# MSVC runtime library flags are selected by an abstraction.
	cmake_policy(SET CMP0091 NEW)
endif()

project(MyHumanoid
	VERSION ${project_version}
	DESCRIPTION MyHumanoid
	LANGUAGES CXX
)

option(MyHumanoid_BUILD_IPO "Build with Interprocedural optimizations" ON)

message(STATUS "Resolving GIT Version")
set(project_git_commit_hash "unknown")
set(project_git_commit_date "unknown")
find_package(Git)
if(GIT_FOUND)
	set(git_show_output "unknown;unknown")
	execute_process(
		COMMAND ${GIT_EXECUTABLE} show -s --encoding=UTF8 "--format=%H;%ci" HEAD
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		OUTPUT_VARIABLE git_show_output
		ERROR_QUIET
		OUTPUT_STRIP_TRAILING_WHITESPACE
	)
	list(GET git_show_output 0 project_git_commit_hash)
	list(GET git_show_output 1 project_git_commit_date)
	message(STATUS "GIT:\nhash=${project_git_commit_hash}\ndate=${project_git_commit_date}")
else()
	message(STATUS "GIT not found")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MyHumanoid_BUILD_IPO)
	include(CheckIPOSupported)
	check_ipo_supported(RESULT HAVE_IPO OUTPUT HAVE_IPO_OUTPUT)
	if(HAVE_IPO)
		message(STATUS "IPO enabled")
		set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
	else()
		message(STATUS "IPO disabled: ${HAVE_IPO_OUTPUT}")
	endif()
endif()


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

set(render_sources
	src/render/DebugGl.h
	src/render/DebugGl.cpp
	src/render/GlTypes.h
	src/render/RenderUtils.h
	src/render/RenderUtils.cpp
	src/render/Shader.h
	src/render/Shader.cpp
)

set(myhumanoid_sources
	src/gui/Camera.h
	src/gui/Camera.cpp
	src/gui/CGUtilities.h
	src/gui/CGUtilities.cpp
	src/gui/Rect.h
	src/gui/Rect.cpp
	src/ComponentID.h
	src/CompositeMorph.h
	src/CompositeMorph.cpp
	src/GlInclude.h
	src/Global.cpp
	src/Global.h
	src/Logger.h
	src/Logger.cpp
	src/MhConfig.h
	src/MhConfig.cpp
	src/MhMain.cpp
	src/MhRender.h
	src/MhRender.cpp
	src/MhUi.h
	src/MhUiCommon.h
	src/MhUiMain.h
	src/MhUiMain.cpp
	src/MhUiMorph.h
	src/MhUiMorph.cpp
	src/MhUiPose.h
	src/MhUiPose.cpp
	src/Profiler.h
	src/Profiler.cpp
	src/StringUtils.h
	src/util.cpp
	src/util.h
	src/Version.h
	src/Version.cpp
	src/Vfs.h
	src/Vfs.cpp
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
find_package(SDL2       CONFIG REQUIRED)
find_package(glm        REQUIRED)
find_package(PhysFS 2.1 REQUIRED)


add_executable(MyHumanoid
	${animorph_sources}
	${render_sources}
	${myhumanoid_sources}
	${shader_sources}
)

configure_file(src/VersionConfig.h.in src/VersionConfig.h)
target_sources(MyHumanoid PRIVATE
	src/VersionConfig.h.in
	${CMAKE_CURRENT_BINARY_DIR}/src/VersionConfig.h
)
target_include_directories(MyHumanoid PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/src)

set_target_properties(MyHumanoid PROPERTIES
	CXX_STANDARD 17
	INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE
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
	${GLM_INCLUDE_DIRS}
	${PHYSFS_INCLUDE_DIRS}
	${CMAKE_CURRENT_SOURCE_DIR}/src
)

if(TARGET SDL2::SDL2)
	target_link_libraries(MyHumanoid PRIVATE SDL2::SDL2 SDL2::SDL2main)
else()
	target_link_libraries(MyHumanoid PRIVATE ${SDL2_LIBRARIES})
	target_include_directories(MyHumanoid PRIVATE ${SDL2_INCLUDE_DIRS})
endif()

target_link_libraries(MyHumanoid PRIVATE
	${OPENGL_LIBRARIES}
	GLEW::GLEW
	${PHYSFS_LIBRARY}
	fmt
	imgui::imgui
	stb::stb
	simpleini
	nativefiledialog
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	execute_process(COMMAND "${CMAKE_COMMAND}" -E create_symlink
		${CMAKE_CURRENT_SOURCE_DIR}/data
		${CMAKE_CURRENT_BINARY_DIR}/data
	)
else()
	file(GLOB_RECURSE data_files RELATIVE ${CMAKE_SOURCE_DIR} "data/*")
    
	foreach(loop_var ${data_files})
	    set(files_str "${files_str}\n${CMAKE_CURRENT_SOURCE_DIR}/${loop_var}")
	endforeach(loop_var)
	
	file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/dataFilesList.txt
	     "${files_str}"
	)
	
	#message(${files_str})
	
	add_custom_command(TARGET MyHumanoid POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E tar "cfv" MyHumanoid.zip
		--files-from="${CMAKE_CURRENT_BINARY_DIR}/dataFilesList.txt"
		--format=zip "${CMAKE_CURRENT_SOURCE_DIR}/data/"
		WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/data"
		COMMENT "Packing data "
	)
	add_custom_command(TARGET MyHumanoid POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E rename
		"${CMAKE_CURRENT_SOURCE_DIR}/data/MyHumanoid.zip"
		"${CMAKE_CURRENT_BINARY_DIR}/MyHumanoid.data"
	)
endif()
