cmake_minimum_required(VERSION 2.8)
project( DisplayImage )

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package( OpenCV REQUIRED )
find_package( OpenGL REQUIRED )

find_package( liblo REQUIRED )
include_directories(${liblo_INCLUDE_DIRS})
set(LIBS ${LIBS} ${liblo_LIBRARIES})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/" )

add_subdirectory(src)
# add_executable( DisplayImage src/DisplayImage.cpp )
# 

# 
# set(CMAKE_BUILD_TYPE Debug)
