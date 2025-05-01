# SPDX-License-Identifier: MIT

project(glad)

if(MH_USE_GLES)
	set(GLAD_SOURCES
		${CMAKE_CURRENT_SOURCE_DIR}/include/glad/gles2.h
		${CMAKE_CURRENT_SOURCE_DIR}/src/gles2.c
	)
else()
	set(GLAD_SOURCES
		${CMAKE_CURRENT_SOURCE_DIR}/include/glad/gl.h
		${CMAKE_CURRENT_SOURCE_DIR}/src/gl.c
	)
endif()

add_library(glad OBJECT
	${CMAKE_CURRENT_SOURCE_DIR}/include/KHR/khrplatform.h
	${GLAD_SOURCES}
)
add_library(glad::glad ALIAS glad)

target_include_directories(glad PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
)
