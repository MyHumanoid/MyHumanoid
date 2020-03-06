# SPDX-License-Identifier: GPL-3.0-or-later

project(tinygltf)

add_library(tinygltf STATIC
	${CMAKE_CURRENT_SOURCE_DIR}/tinygltf/json.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/tinygltf/tiny_gltf.h
	${CMAKE_CURRENT_SOURCE_DIR}/tinygltf.cpp
)
target_include_directories(tinygltf PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/tinygltf
)
target_compile_definitions(tinygltf PUBLIC
	TINYGLTF_USE_CPP14
	#TINYGLTF_NO_FS
)
target_link_libraries(tinygltf PUBLIC
	stb::stb
)
add_library(tinygltf::tinygltf ALIAS tinygltf)
