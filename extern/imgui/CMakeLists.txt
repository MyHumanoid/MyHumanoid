# SPDX-License-Identifier: GPL-3.0-or-later

project(imgui)

set(imgui_dir ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
set(imgui_src
	${imgui_dir}/imgui.cpp
	${imgui_dir}/imgui.h
	${imgui_dir}/imgui_demo.cpp
	${imgui_dir}/imgui_draw.cpp
	${imgui_dir}/imgui_internal.h
	${imgui_dir}/imgui_tables.cpp
	${imgui_dir}/imgui_widgets.cpp
	${imgui_dir}/imstb_textedit.h
	${imgui_dir}/misc/cpp/imgui_stdlib.cpp
	${imgui_dir}/misc/cpp/imgui_stdlib.h
	${imgui_dir}/backends/imgui_impl_opengl3.h
	${imgui_dir}/backends/imgui_impl_opengl3.cpp
	${imgui_dir}/backends/imgui_impl_opengl3_loader.h
	${imgui_dir}/backends/imgui_impl_sdl3.h
	${imgui_dir}/backends/imgui_impl_sdl3.cpp
)

add_library(imgui STATIC ${imgui_src})
add_library(imgui::imgui ALIAS imgui)

target_include_directories(imgui PUBLIC ${imgui_dir})

target_link_libraries(imgui PUBLIC
	stb::stb
	glad::glad
	SDL3::SDL3
)

target_compile_definitions(imgui PUBLIC
	IMGUI_DISABLE_STB_RECT_PACK_IMPLEMENTATION
	IMGUI_STB_RECT_PACK_FILENAME="stb_rect_pack.h"
	IMGUI_DISABLE_STB_TRUETYPE_IMPLEMENTATION
	IMGUI_STB_TRUETYPE_FILENAME="stb_truetype.h"
)

if(MH_USE_GLES)
	target_compile_definitions(imgui PUBLIC
		IMGUI_IMPL_OPENGL_ES3=1
	)
endif()
