# SPDX-License-Identifier: GPL-3.0-or-later

project(imgui)

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
	${imgui_dir}/examples/imgui_impl_opengl3.h
	${imgui_dir}/examples/imgui_impl_opengl3.cpp
	${imgui_dir}/examples/imgui_impl_sdl.h
	${imgui_dir}/examples/imgui_impl_sdl.cpp
)

find_package(GLEW REQUIRED)
find_package(SDL2 REQUIRED)

add_library(imgui STATIC
	${imgui_src}
)
target_link_libraries(imgui
PRIVATE
	GLEW::GLEW
	${SDL2_LIBRARIES}
)
target_include_directories(imgui
PRIVATE
	${GLEW_INCLUDE_DIRS}
	${SDL2_INCLUDE_DIRS}
PUBLIC
	${imgui_dir}
)

# ============================================================

add_library(stb_image INTERFACE)
target_sources(stb_image INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_image.h
)

target_include_directories(stb_image INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}/stb
)

# ============================================================

add_library(simpleini STATIC
	${CMAKE_CURRENT_SOURCE_DIR}/simpleini/ConvertUTF.c
	${CMAKE_CURRENT_SOURCE_DIR}/simpleini/ConvertUTF.h
	${CMAKE_CURRENT_SOURCE_DIR}/simpleini/SimpleIni.h
)
target_include_directories(simpleini PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/simpleini
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
	PUBLIC
		${nfd_dir}/include
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
	target_include_directories(nativefiledialog
	PUBLIC
		${nfd_dir}/include
	)
endif()

# ============================================================

add_library(fmt STATIC
	${CMAKE_CURRENT_SOURCE_DIR}/fmt/src/format.cc
	${CMAKE_CURRENT_SOURCE_DIR}/fmt/src/posix.cc
)
target_include_directories(fmt PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/fmt/include
)
