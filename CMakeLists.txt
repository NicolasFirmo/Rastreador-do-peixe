cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
project( Rastreador )

set(OpenCV_STATIC ON)
set(BUILD_SHARED_LIBS OFF)
SET(OpenCV_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vendor/OpenCV/Linux/lib/cmake/opencv4")

find_package(OpenCV REQUIRED PATHS "${CMAKE_CURRENT_SOURCE_DIR}/vendor/OpenCV/Linux/lib/cmake/opencv4")

MESSAGE(STATUS "OpenCV_DIR ${OpenCV_DIR}")
MESSAGE(STATUS "OpenCV Include dirs ${OpenCV_INCLUDE_DIRS}")
MESSAGE(STATUS "OpenCV LINK LIBRARIES ${OpenCV_LIBS}")

include_directories( ${OpenCV_INCLUDE_DIRS} )

add_subdirectory(vendor)
add_subdirectory(src)
add_subdirectory(Detect)