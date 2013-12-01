cmake_minimum_required(VERSION 2.8)
project( DisplayImage )

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# Require OpenCV, OpenGl, and GLUT using cmake modules
find_package( OpenCV REQUIRED )
find_package( OpenGL REQUIRED )
find_package( GLUT REQUIRED )

# Get liblo in there
find_package( liblo REQUIRED )
include_directories(${liblo_INCLUDE_DIRS})
set(LIBS ${LIBS} ${liblo_LIBRARIES})

# include rtaudio
include_directories( ${CMAKE_SOURCE_DIR}/include/rtaudio )
# link rtaudio
link_directories( ${CMAKE_SOURCE_DIR}/include/rtaudio )

#include core
include_directories( ${CMAKE_SOURCE_DIR}/include/core )
#link core
link_directories( ${CMAKE_SOURCE_DIR}/include/core )

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/" )

# Setup Sources
set(rtaudio_SOURCES include/rtaudio/RtAudio.cpp include/rtaudio/RtAudio.h include/rtaudio/RtError.h)
set(core_SOURCES include/core/theremax-audio.cpp include/core/theremax-audio.h include/core/theremax-def.h ${rtaudio_SOURCES} )

set(theremax_SOURCES  src/theremax.cpp ${core_SOURCES} )

# add_subdirectory(src)
add_executable( theremax ${theremax_SOURCES} )
target_link_libraries( theremax ${OpenCV_LIBS} ${OpenGL_LIBS} ${LIBS} )
# 

# 
# set(CMAKE_BUILD_TYPE Debug)
