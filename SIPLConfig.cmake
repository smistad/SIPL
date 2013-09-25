# Find the SIPL includes and library
#
# SIPL_INCLUDE_DIR - where to find include files.
# SIPL_LIBRARIES - List of fully qualified libraries to link against.
# SIPL_FOUND - set to 1 if found.
#
# Usage:
# SIPL (SIPL)
#----------
# option(CX_USE_SIPL "use SIPL (SIPL)" OFF)
# if (CX_USE_SIPL)
#    ADD_DEFINITIONS(-DCX_USE_SIPL)
#    find_package(SIPL REQUIRED)
#    include()
# endif()
#

set (SIPL_FOUND 1)

set (SIPL_INCLUDE_DIRS "/home/smistad/Dropbox/Programmering/GPU-Narrow-Band-Level-Set-Segmentation/SIPL")
set (SIPL_LIBRARY_DIRS "/home/smistad/Dropbox/Programmering/GPU-Narrow-Band-Level-Set-Segmentation/SIPL")

set (SIPL_USE_FILE "/home/smistad/Dropbox/Programmering/GPU-Narrow-Band-Level-Set-Segmentation/SIPL/CMake/SIPLUse.cmake")

