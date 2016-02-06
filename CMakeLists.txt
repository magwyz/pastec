cmake_minimum_required(VERSION 2.6)
project(Pastec)

include_directories(include include/orb)

set(SOURCES      src/main.cpp
                 src/imagereranker.cpp
                 src/imagererankerransac.cpp
                 src/imageloader.cpp
                 src/httpserver.cpp
                 src/requesthandler.cpp
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
                 include/orb/orbfeatureextractor.h
                 include/orb/orbindex.h
                 include/orb/orbsearcher.h
                 include/orb/orbwordindex.h
                 include/searcher.h
                 include/httpserver.h
                 include/requesthandler.h)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(Threads REQUIRED)
find_package(OpenCV REQUIRED)
find_package(microhttpd REQUIRED)
find_package(jsoncpp REQUIRED)

add_executable(pastec ${SOURCES} ${HEADERS})

target_link_libraries(pastec ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(pastec ${OpenCV_LIBS})
target_link_libraries(pastec ${LIBMICROHTTPD_LIBRARY})
target_link_libraries(pastec ${LIBJSONCPP_LIBRARY})

