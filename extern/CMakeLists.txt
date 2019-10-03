#    CMakeLists.txt for somewhat nice imgui loading
#
#    Copyright (C) 2018  Malte Kießling
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    any later version.

#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.

#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <https://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.1.3 FATAL_ERROR)
project(imgui)

#cpp 11, we need find_sdl2 for the includes and also disallow in-source builds 
set (CMAKE_CXX_STANDARD 11)
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

# gl3w
#set(gl3wGen ${CMAKE_CURRENT_SOURCE_DIR}/gl3w/gl3w_gen.py)
#set(gl3wTargetDir ${CMAKE_BINARY_DIR}/gl3w)
#set(gl3wIncludeDir ${gl3wTargetDir}/include)
#set(gl3wSource ${gl3wTargetDir}/src/gl3w.c)


#add_library(gl3w STATIC ${gl3wSource})
#target_include_directories(gl3w PUBLIC ${gl3wIncludeDir})
#if(NOT WIN32)
#    target_link_libraries(gl3w dl)
#endif()
#set_property(TARGET gl3w PROPERTY POSITION_INDEPENDENT_CODE ON)

# imgui 
#find_package(SDL2 REQUIRED)
# these sources are quite api specific, but i dont care much

set(imgui_dir ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
set(imgui_src
	${imgui_dir}/imgui.cpp
	${imgui_dir}/imgui.h
	${imgui_dir}/imgui_demo.cpp
	${imgui_dir}/imgui_draw.cpp
	${imgui_dir}/imgui_internal.h
	${imgui_dir}/imgui_widgets.cpp
	${imgui_dir}/imstb_rectpack.h
	${imgui_dir}/imstb_textedit.h
	${imgui_dir}/imstb_truetype.h
	${imgui_dir}/misc/cpp/imgui_stdlib.cpp
	${imgui_dir}/misc/cpp/imgui_stdlib.h
	${imgui_dir}/examples/imgui_impl_glut.h
	${imgui_dir}/examples/imgui_impl_glut.cpp
	${imgui_dir}/examples/imgui_impl_opengl3.h
	${imgui_dir}/examples/imgui_impl_opengl3.cpp
)

find_package(GLEW REQUIRED)
find_package(GLUT REQUIRED)

add_library(imgui STATIC
	${imgui_src}
)
target_link_libraries(imgui
PRIVATE
	GLEW::GLEW
	GLUT::GLUT
)
target_include_directories(imgui
PRIVATE
	${GLEW_INCLUDE_DIRS}
	${GLUT_INCLUDE_DIRS}
PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/imgui/
)

#target_link_libraries(imgui gl3w)
#target_compile_definitions(imgui PUBLIC IMGUI_IMPL_OPENGL_LOADER_GL3W)
#target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/imgui/)
#target_include_directories(imgui PRIVATE ${SDL2_INCLUDE_DIR})
set_property(TARGET imgui PROPERTY POSITION_INDEPENDENT_CODE ON)

# ============================================================

add_library(stb_image INTERFACE)
target_sources(stb_image INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_image.h
)

target_include_directories(stb_image INTERFACE
	"${PROJECT_SOURCE_DIR}/stb"
)

# ============================================================

add_library(simpleini STATIC
	${CMAKE_CURRENT_SOURCE_DIR}/simpleini/ConvertUTF.c
	${CMAKE_CURRENT_SOURCE_DIR}/simpleini/ConvertUTF.h
	${CMAKE_CURRENT_SOURCE_DIR}/simpleini/SimpleIni.h
)
target_include_directories(simpleini PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/simpleini/
)

# ============================================================

set(nfd_dir ${CMAKE_CURRENT_SOURCE_DIR}/nativefiledialog)

if(UNIX)
	find_package(PkgConfig REQUIRED)
	pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
	
	add_library(nativefiledialog STATIC
		${nfd_dir}/src/common.h
		${nfd_dir}/src/nfd_common.c
		${nfd_dir}/src/nfd_common.h
		${nfd_dir}/src/nfd_gtk.c
		${nfd_dir}/src/nfd_zenity.c
		${nfd_dir}/src/simple_exec.h
	)
	target_include_directories(nativefiledialog
	PRIVATE
		${GTK3_INCLUDE_DIRS}
	)
	target_link_libraries(nativefiledialog
	PRIVATE
		${GTK3_LIBRARIES}
	)
else()
	add_library(nativefiledialog STATIC
		${nfd_dir}/src/common.h
		${nfd_dir}/src/nfd_common.c
		${nfd_dir}/src/nfd_common.h
		${nfd_dir}/src/nfd_win.cpp
	)
endif()

target_include_directories(nativefiledialog PUBLIC
	${nfd_dir}/include
)

# ============================================================

add_library(fmt STATIC
       ${CMAKE_CURRENT_SOURCE_DIR}/fmt/src/format.cc
       ${CMAKE_CURRENT_SOURCE_DIR}/fmt/src/posix.cc
)
target_include_directories(fmt PUBLIC
       ${CMAKE_CURRENT_SOURCE_DIR}/fmt/include
)
