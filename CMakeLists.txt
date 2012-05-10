cmake_minimum_required(VERSION 2.8)
project(SIPL)

find_package (PkgConfig REQUIRED)
pkg_check_modules (GTK2 REQUIRED gtk+-2.0 gthread-2.0)

include_directories (${GTK2_INCLUDE_DIRS})
set(SIPL_INCLUDE_DIRS ${GTK2_INCLUDE_DIRS} PARENT_SCOPE)
link_directories (${GTK2_LIBRARY_DIRS})
add_library (SIPL Core.cpp )
add_definitions (${GTK2_CFLAGS_OTHER})
target_link_libraries (SIPL ${GTK2_LIBRARIES})
