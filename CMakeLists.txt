# SPDX-License-Identifier: MIT
# SPDX-FileCopyrightText: 2025 Eli2

cmake_minimum_required(VERSION 3.28)

set(CMAKE_POLICY_DEFAULT_CMP0069 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)


include(${CMAKE_CURRENT_LIST_DIR}/ProjectVersion.cmake)

project(
	MyHumanoid
	VERSION ${project_version_major}.${project_version_minor}.${project_version_patch}
	DESCRIPTION MyHumanoid
	LANGUAGES C CXX
)
message(STATUS "${PROJECT_NAME} version: ${PROJECT_VERSION}")

option(MH_USE_GLES           "Use OpenGL ES for rendering"        OFF)
option(MH_ENABLE_ASAN        "Enable address sanitizer"           OFF)
option(MH_ENABLE_TRACY       "Enable tracing using tracy"         OFF)
option(MH_USE_SYSTEM_FMT     "Use system provided fmt library"    ON)
option(MH_USE_SYSTEM_SDL     "Use system provided SDL library"    OFF)
option(MH_USE_SYSTEM_GLM     "Use system provided glm library"    ON)
option(MH_USE_SYSTEM_PHYSFS  "Use system provided physfs library" OFF)


if(DEFINED EMSCRIPTEN)
	message(STATUS "Overriding options for emscripten")
	set(MH_USE_GLES ON)
	set(MH_ENABLE_ASAN OFF)
	set(MH_ENABLE_TRACY OFF)
	set(MH_USE_SYSTEM_FMT OFF)
	set(MH_USE_SYSTEM_SDL OFF)
	set(MH_USE_SYSTEM_GLM OFF)
	set(MH_USE_SYSTEM_PHYSFS OFF)
endif()

message(STATUS "")
message(STATUS "MyHumanoid configured options:")
message(STATUS "MH_USE_GLES           Use OpenGL ES for rendering        ${MH_USE_GLES}")
message(STATUS "MH_ENABLE_ASAN        Enable address sanitizer           ${MH_ENABLE_ASAN}")
message(STATUS "MH_ENABLE_TRACY       Enable tracing using tracy         ${MH_ENABLE_TRACY}")
message(STATUS "MH_USE_SYSTEM_FMT     Use system provided fmt library    ${MH_USE_SYSTEM_FMT}")
message(STATUS "MH_USE_SYSTEM_SDL     Use system provided SDL library    ${MH_USE_SYSTEM_SDL}")
message(STATUS "MH_USE_SYSTEM_GLM     Use system provided glm library    ${MH_USE_SYSTEM_GLM}")
message(STATUS "MH_USE_SYSTEM_PHYSFS  Use system provided physfs library ${MH_USE_SYSTEM_PHYSFS}")
message(STATUS "")


message(STATUS "Resolving GIT Version")
set(project_git_commit_hash "unknown")
set(project_git_commit_date "unknown")
find_package(Git)
if(GIT_FOUND)
	set(git_show_output "unknown;unknown")
	execute_process(
		COMMAND ${GIT_EXECUTABLE} show -s --encoding=UTF8 "--format=%H;%cs" HEAD
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		OUTPUT_VARIABLE git_show_output
		ERROR_QUIET
		OUTPUT_STRIP_TRAILING_WHITESPACE
	)
	list(GET git_show_output 0 project_git_commit_hash)
	list(GET git_show_output 1 project_git_commit_date)
	message(STATUS "GIT hash=${project_git_commit_hash}")
	message(STATUS "GIT date=${project_git_commit_date}")
else()
	message(STATUS "GIT not found")
endif()


if(WIN32)
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

if(MH_ENABLE_ASAN)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-omit-frame-pointer -fsanitize=address")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -fsanitize=address")
	set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fno-omit-frame-pointer -fsanitize=address")
endif()

# Libraries

include(FetchContent)

if(MH_USE_SYSTEM_FMT)
	find_package(fmt REQUIRED)
else()
	FetchContent_Declare(
		fmt
		GIT_SHALLOW    ON
		GIT_REPOSITORY https://github.com/fmtlib/fmt
		GIT_TAG        123913715afeb8a437e6388b4473fcc4753e1c9a # 11.1.4
	)
	FetchContent_MakeAvailable(fmt)
endif()

if(MH_USE_SYSTEM_SDL)
	find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3)
else()
	set(SDL_SHARED OFF)
	set(SDL_STATIC ON)
	set(SDL_TEST_LIBRARY OFF)
	
	FetchContent_Declare(
		SDL3
		GIT_SHALLOW    ON
		GIT_REPOSITORY https://github.com/libsdl-org/SDL
		GIT_TAG        877399b2b2cf21e67554ed9046410f268ce1d1b2 # 3.2.10
	)
	FetchContent_MakeAvailable(SDL3)
endif()

if(MH_USE_SYSTEM_GLM)
	find_package(glm REQUIRED)
else()
	FetchContent_Declare(
		glm
		GIT_SHALLOW    ON
		GIT_REPOSITORY https://github.com/g-truc/glm
		GIT_TAG        0af55ccecd98d4e5a8d1fad7de25ba429d60e863 # 1.0.1
	)
	FetchContent_MakeAvailable(glm)
endif()

if(MH_USE_SYSTEM_PHYSFS)
	find_package(PhysFS 2.1 REQUIRED)
else()
	set(PHYSFS_ARCHIVE_GRP     OFF)
	set(PHYSFS_ARCHIVE_WAD     OFF)
	set(PHYSFS_ARCHIVE_CSM     OFF)
	set(PHYSFS_ARCHIVE_HOG     OFF)
	set(PHYSFS_ARCHIVE_MVL     OFF)
	set(PHYSFS_ARCHIVE_QPAK    OFF)
	set(PHYSFS_ARCHIVE_SLB     OFF)
	set(PHYSFS_ARCHIVE_ISO9660 OFF)
	set(PHYSFS_ARCHIVE_VDF     OFF)
	set(PHYSFS_BUILD_SHARED    OFF)
	set(PHYSFS_BUILD_TEST      OFF)
	set(PHYSFS_DISABLE_INSTALL ON)
	set(PHYSFS_BUILD_DOCS      OFF)
	
	FetchContent_Declare(
		physfs
		GIT_SHALLOW    ON
		GIT_REPOSITORY https://github.com/icculus/physfs
		GIT_TAG        adfdec6af14e4d12551446e7ad060415ca563950
	)
	FetchContent_MakeAvailable(physfs)
endif()

if(MH_ENABLE_TRACY)
	set(TRACY_LTO    ON)
	
	FetchContent_Declare(
		tracy
		GIT_SHALLOW    ON
		GIT_REPOSITORY https://github.com/wolfpld/tracy
		GIT_TAG        a03c7580b9101df3225937524e09a9a885d5464a
	)
	FetchContent_MakeAvailable(tracy)
endif()


add_subdirectory(extern)

# Sources

set(animorph_sources
	src/animorph/BodySettings.h
	src/animorph/BodySettings.cpp
	src/animorph/DirectoryList.h
	src/animorph/DirectoryList.cpp
	src/animorph/Face.h
	src/animorph/Face.cpp
	src/animorph/FileReader.h
	src/animorph/FileReader.cpp
	src/animorph/Target.h
	src/animorph/Target.cpp
	src/animorph/Vertex.h
	src/animorph/Vertex.cpp
	src/animorph/xmlParser.h
	src/animorph/xmlParser.cpp
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
	src/render/RenderUtils.h
	src/render/RenderUtils.cpp
)

set(myhumanoid_sources
	src/gui/Camera.h
	src/gui/Camera.cpp
	src/gui/CGUtilities.h
	src/gui/CGUtilities.cpp
	src/gui/Rect.h
	src/gui/Rect.cpp

	src/io/ColladaExporter.h
	src/io/ColladaExporter.cpp
	src/io/Gltf.h
	src/io/Gltf.cpp
	src/io/ObjExporter.h
	src/io/ObjExporter.cpp

	src/AbstractGl.h
	src/AbstractGl.cpp
	src/ComponentID.h
	src/CompositeMorph.h
	src/CompositeMorph.cpp
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
	
	data/shader/common_es.glsl
	data/shader/common_gl.glsl
)

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
	INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE
)

target_compile_features(MyHumanoid PRIVATE
	cxx_std_20
)

if(MSVC)
	target_compile_options(MyHumanoid PUBLIC /W4)
else()
	target_compile_options(MyHumanoid PUBLIC -Wall -Wextra -Wpedantic)
endif()

target_include_directories(MyHumanoid PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(MyHumanoid PRIVATE
	SDL3::SDL3
	glad::glad
	glm::glm
	PhysFS::PhysFS-static
	fmt::fmt
	imgui::imgui
	stb::stb
	tinygltf::tinygltf
	simpleini
)

if(MH_USE_GLES)
	target_compile_definitions(MyHumanoid PRIVATE
		AGL_USE_GLES=1
	)
endif()

if(MH_ENABLE_TRACY)
	target_compile_definitions(MyHumanoid PRIVATE
		MH_ENABLE_TRACY=1
	)
	target_link_libraries(MyHumanoid PRIVATE
		TracyClient
	)
endif()

if(DEFINED EMSCRIPTEN)
	set_target_properties(MyHumanoid PROPERTIES
		LINK_FLAGS
		"-O3 -sSTACK_SIZE=2mb -sINITIAL_MEMORY=256mb -sALLOW_MEMORY_GROWTH -sMIN_WEBGL_VERSION=2"
	)
endif()


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
		COMMAND ${CMAKE_COMMAND} -E tar "cf" MyHumanoid.zip
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
