cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
project( Rastreador )

set(OpenCV_STATIC ON)
set(BUILD_SHARED_LIBS OFF)
set(OpenCV_DIR "/usr/local/lib")

find_package(OpenCV REQUIRED)

MESSAGE(STATUS "Include dirs ${OpenCV_INCLUDE_DIRS}")
MESSAGE(STATUS "LINK LIBRARIES ${OpenCV_LIBS}")

include_directories( ${OpenCV_INCLUDE_DIRS} )

add_subdirectory(vendor)
add_subdirectory(src)
add_subdirectory(Detect)