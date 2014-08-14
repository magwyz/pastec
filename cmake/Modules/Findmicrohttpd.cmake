# - Try to find libmicrohttpd
# Once done this will define
#  LIBMICROHTTPD_FOUND - System has libmicrohttpd
#  LIBMICROHTTPD_INCLUDE_DIRS - The libmicrohttpd include directories
#  LIBMICROHTTPD_LIBRARIES - The libraries needed to use libmicrohttpd
#  LIBMICROHTTPD_DEFINITIONS - Compiler switches required for using libmicrohttp

find_package(PkgConfig)
pkg_check_modules(PC_LIBMICROHTTPD QUIET libmicrohttpd)
set(LIBMICROHTTPD_DEFINITIONS ${PC_LIBMICROHTTPD_CFLAGS_OTHER})

find_path(LIBMICROHTTPD_INCLUDE_DIR microhttpd.h
          HINTS ${PC_LIBMICROHTTPD_INCLUDEDIR} ${PC_LIBMICROHTTPD_INCLUDE_DIRS}
          PATH_SUFFIXES libmicrohttpd)

find_library(LIBMICROHTTPD_LIBRARY microhttpd
             HINTS ${PC_LIBMICROHTTPD_LIBDIR} ${PC_LIBMICROHTTPD_LIBRARY_DIRS} )

set(LIBMICROHTTPD_LIBRARIES ${LIBMICROHTTPD_LIBRARY} )
set(LIBMICROHTTPD_INCLUDE_DIRS ${LIBMICROHTTPD_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBMICROHTTPD_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(libmicrohttpd  DEFAULT_MSG
                                  LIBMICROHTTPD_LIBRARY LIBMICROHTTPD_INCLUDE_DIR)

mark_as_advanced(LIBMICROHTTPD_INCLUDE_DIR LIBMICROHTTPD_LIBRARY)
