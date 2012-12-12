###########################################################
##          SIPL project
###########################################################
project(SIPL)

cmake_minimum_required(VERSION 2.8)

#------------------------------------------------------------------------------
# External libraries
#------------------------------------------------------------------------------

# GTK
###########
find_package (PkgConfig REQUIRED)
pkg_check_modules (GTK2 REQUIRED gtk+-2.0 gthread-2.0)

# OpenMP
###########
find_package(OpenMP)
if(OPENMP_FOUND)
    message("-- OpenMP was detected. Using OpenMP to speed up MIP calculations.")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}" PARENT_SCOPE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}" PARENT_SCOPE)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}" PARENT_SCOPE)
endif()


#------------------------------------------------------------------------------
# Where to look for includes and libraries
#------------------------------------------------------------------------------
include_directories (${GTK2_INCLUDE_DIRS})
set(SIPL_INCLUDE_DIRS ${GTK2_INCLUDE_DIRS})
link_directories (${GTK2_LIBRARY_DIRS})


#------------------------------------------------------------------------------
# Targets
#------------------------------------------------------------------------------

# SIPL library
###########
add_library (SIPL Core.hpp Core.cpp Types.hpp Types.cpp )
target_link_libraries (SIPL ${GTK2_LIBRARIES})


#------------------------------------------------------------------------------
# Compiler options
#------------------------------------------------------------------------------
add_definitions (${GTK2_CFLAGS_OTHER})


#------------------------------------------------------------------------------
# Configure file for find_package module 
#------------------------------------------------------------------------------
set( SIPL_INCLUDE_DIRS
    ${SIPL_SOURCE_DIR}
    ) 

set(SIPL_LIBRARY_DIRS
    ${SIPL_BINARY_DIR}
    )    

configure_file (
    "${PROJECT_SOURCE_DIR}/CMake/SIPLConfig.cmake.in"
    "${PROJECT_BINARY_DIR}/SIPLConfig.cmake"
    )
