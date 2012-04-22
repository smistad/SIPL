project(SIPL-example)
cmake_minimum_required (VERSION 2.6)

find_package (PkgConfig REQUIRED)
pkg_check_modules (GTK2 REQUIRED gtk+-2.0 gthread-2.0)

include_directories (${GTK2_INCLUDE_DIRS})
link_directories (${GTK2_LIBRARY_DIRS})
add_executable (example Core.cpp example.cpp)
add_definitions (${GTK2_CFLAGS_OTHER})
target_link_libraries (example ${GTK2_LIBRARIES})
