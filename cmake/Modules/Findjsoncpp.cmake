# - Try to find libjsoncpp
# Once done this will define
#  LIBJSONCPP_FOUND - System has libjsoncpp
#  LIBJSONCPP_INCLUDE_DIRS - The libjsoncpp include directories
#  LIBJSONCPP_LIBRARIES - The libraries needed to use libjsoncpp
#  LIBJSONCPP_DEFINITIONS - Compiler switches required for using libjsoncpp

find_package(PkgConfig)
pkg_check_modules(PC_LIBJSONCPP QUIET libjsoncpp)
set(LIBJSONCPP_DEFINITIONS ${PC_LIBJSONCPP_CFLAGS_OTHER})

find_path(LIBJSONCPP_INCLUDE_DIR json.h
          HINTS ${PC_LIBJSONCPP_INCLUDEDIR} ${PC_LIBJSONCPP_INCLUDE_DIRS}
          PATH_SUFFIXES libjsoncpp)

find_library(LIBJSONCPP_LIBRARY jsoncpp
             HINTS ${PC_LIBJSONCPP_LIBDIR} ${PC_LIBJSONCPP_LIBRARY_DIRS} )

set(LIBJSONCPP_LIBRARIES ${LIBJSONCPP_LIBRARY})
set(LIBJSONCPP_INCLUDE_DIRS ${LIBJSONCPP_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBJSONCPP_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(libjsoncpp  DEFAULT_MSG
                                  LIBJSONCPP_LIBRARY LIBJSONCPP_INCLUDE_DIR)

mark_as_advanced(LIBJSONCPP_INCLUDE_DIR LIBJSONCPP_LIBRARY)
