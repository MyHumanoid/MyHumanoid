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
