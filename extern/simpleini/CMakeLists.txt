# SPDX-License-Identifier: GPL-3.0-or-later

project(simpleini)

add_library(simpleini STATIC
	${CMAKE_CURRENT_SOURCE_DIR}/simpleini/ConvertUTF.c
	${CMAKE_CURRENT_SOURCE_DIR}/simpleini/ConvertUTF.h
	${CMAKE_CURRENT_SOURCE_DIR}/simpleini/SimpleIni.h
)
target_include_directories(simpleini PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/simpleini
)
