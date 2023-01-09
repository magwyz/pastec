cmake_minimum_required(VERSION 2.6)
project(Pastec)

include_directories(include include/orb)

set(SOURCES      src/main.cpp
                 src/imagereranker.cpp
                 src/imagererankerransac.cpp
                 src/imageloader.cpp
                 src/httpserver.cpp
                 src/jsoncpp.cpp
                 src/requesthandler.cpp
                 src/imagedownloader.cpp
                 src/orb/orbfeatureextractor.cpp
                 src/orb/orbindex.cpp
                 src/orb/orbsearcher.cpp
                 src/orb/orbwordindex.cpp)

set(HEADERS      include/thread.h
                 include/messages.h
                 include/hit.h
                 include/searchResult.h
                 include/imagereranker.h
                 include/backwardindexreaderaccess.h
                 include/imageloader.h
                 include/index.h
                 include/featureextractor.h
                 include/imagedownloader.h
                 include/json/json-forwards.h
                 include/json/json.h
                 include/orb/orbfeatureextractor.h
                 include/orb/orbindex.h
                 include/orb/orbsearcher.h
                 include/orb/orbwordindex.h
                 include/searcher.h
                 include/httpserver.h
                 include/requesthandler.h)


set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_INSTALL_RPATH "/tmp")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif(NOT CMAKE_BUILD_TYPE)

find_package(Threads REQUIRED)
find_package(OpenCV REQUIRED)
find_package(microhttpd REQUIRED)
find_package(CURL REQUIRED)

add_executable(pastec ${SOURCES} ${HEADERS})

target_link_libraries(pastec ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(pastec ${OpenCV_LIBS})
target_link_libraries(pastec ${LIBMICROHTTPD_LIBRARY})
target_link_libraries(pastec ${CURL_LIBRARIES})

#target_include_directories(pastec ${OpenCV_INCLUDE_DIRS})
#target_include_directories(pastec ${CMAKE_THREAD_INCLUDE_DIRS})

#add_library(pastec SHARED  ${CMAKE_THREAD_LIBS_INIT} ${OpenCV_LIBS} ${LIBMICROHTTPD_LIBRARY} ${CURL_LIBRARIES})


#add_dependencies(pastec ${CMAKE_THREAD_LIBS_INIT} )
#add_dependencies(pastec ${OpenCV_LIBS})
#add_dependencies(pastec ${LIBMICROHTTPD_LIBRARY})
#add_dependencies(pastec ${CURL_LIBRARIES} )
target_include_directories(pastec PRIVATE ${CMAKE_THREAD_INCLUDE_DIRS}  ${ZLIB_INCLUDE_DIRS}
${Boost_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS} ${LIBMICROHTTPD_INCLUDE_DIRS} ${CURL_INCLUDE_DIRS} )

set ( pastec_INCLUDE_DIRS "include" )

include_directories(${CMAKE_SOURCE_DIR}/include ${OpenCV_INCLUDE_DIRS})

#target_link_libraries(pastec ${CMAKE_THREAD_LIBS_INIT} ${OpenCV_LIBS} ${LIBMICROHTTPD_LIBRARY} ${CURL_LIBRARIES})
#target_link_libraries(pastec ${OpenCV_LIBS})
#target_link_libraries(pastec ${LIBMICROHTTPD_LIBRARY})
#target_link_libraries(pastec ${CURL_LIBRARIES})
#target_include_directories(pastec PUBLIC ${CMAKE_THREAD_LIBS_INIT} ${OpenCV_LIBS} ${LIBMICROHTTPD_LIBRARY} ${CURL_LIBRARIES})


set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

#add_library(opencv SHARED ${OpenCV_INCLUDE_DIRS})
#dd_executable(run ${RUN_FILES})
#target_link_libraries(pastec opencv)

#install(
#  TARGETS pastec opencv
#  RUNTIME DESTINATION bin
#  LIBRARY DESTINATION lib
#)
