# SPDX-License-Identifier: GPL-3.0-or-later
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

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
find_package(SDL2 CONFIG REQUIRED)

add_library(imgui STATIC ${imgui_src})

target_link_libraries(imgui PUBLIC GLEW::GLEW)

if(TARGET SDL2::SDL2)
	target_link_libraries(imgui PUBLIC SDL2::SDL2)
else()
	target_link_libraries(imgui PUBLIC ${SDL2_LIBRARIES})
	target_include_directories(imgui PUBLIC ${SDL2_INCLUDE_DIRS})
endif()

target_include_directories(imgui PUBLIC ${imgui_dir})
