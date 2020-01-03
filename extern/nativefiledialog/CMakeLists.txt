# SPDX-License-Identifier: GPL-3.0-or-later

project(nativefiledialog)

set(nfd_dir ${CMAKE_CURRENT_SOURCE_DIR}/nativefiledialog)

add_library(nativefiledialog STATIC)

target_sources(nativefiledialog
PRIVATE
	${nfd_dir}/src/common.h
	${nfd_dir}/src/nfd_common.c
	${nfd_dir}/src/nfd_common.h
)

target_include_directories(nativefiledialog
PUBLIC
	${nfd_dir}/include
)

if(UNIX)
	target_sources(nativefiledialog
	PRIVATE
		${nfd_dir}/src/nfd_gtk.c
		${nfd_dir}/src/nfd_zenity.c
		${nfd_dir}/src/simple_exec.h
	)
	
	find_package(PkgConfig REQUIRED)
	pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
	
	target_include_directories(nativefiledialog
	PRIVATE
		${GTK3_INCLUDE_DIRS}
	)
	target_link_libraries(nativefiledialog
	PRIVATE
		${GTK3_LIBRARIES}
	)
else()
	target_sources(nativefiledialog
	PRIVATE
		${nfd_dir}/src/nfd_win.cpp
	)
endif()
