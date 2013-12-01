cmake_minimum_required(VERSION 2.8)
project( DisplayImage )

# Add local cmake module path
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

# include stk
include_directories( ${CMAKE_SOURCE_DIR}/include/stk )
# link stk
link_directories( ${CMAKE_SOURCE_DIR}/include/stk )

#include core
include_directories( ${CMAKE_SOURCE_DIR}/core )
#link core
link_directories( ${CMAKE_SOURCE_DIR}/core )

#include x-api
include_directories( ${CMAKE_SOURCE_DIR}/include/x-api )
#link x-api
link_directories( ${CMAKE_SOURCE_DIR}/include/x-api )

#include y-api
include_directories( ${CMAKE_SOURCE_DIR}/include/y-api )
#link y-api
link_directories( ${CMAKE_SOURCE_DIR}/include/y-api )

# Set some defaults
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/" )
set(CMAKE_BUILD_TYPE Debug)

# Setup Sources for rtaudio
set(rtaudio_SOURCES
  ${CMAKE_SOURCE_DIR}/include/rtaudio/RtAudio.cpp
  ${CMAKE_SOURCE_DIR}/include/rtaudio/RtAudio.h
  ${CMAKE_SOURCE_DIR}/include/rtaudio/RtError.h
)

set(stk_SOURCES
  ${CMAKE_SOURCE_DIR}/include/stk/Stk.cpp
  ${CMAKE_SOURCE_DIR}/include/stk/Stk.h
  ${CMAKE_SOURCE_DIR}/include/stk/Filter.h
  ${CMAKE_SOURCE_DIR}/include/stk/Delay.cpp
  ${CMAKE_SOURCE_DIR}/include/stk/Delay.h
)

set(xyapi_SOURCES
  ${CMAKE_SOURCE_DIR}/include/x-api/x-def.h
  ${CMAKE_SOURCE_DIR}/include/y-api/y-fft.cpp
  ${CMAKE_SOURCE_DIR}/include/y-api/y-fft.h
  ${CMAKE_SOURCE_DIR}/include/x-api/x-audio.cpp
  ${CMAKE_SOURCE_DIR}/include/x-api/x-audio.h
  ${CMAKE_SOURCE_DIR}/include/x-api/x-vector3d.cpp
  ${CMAKE_SOURCE_DIR}/include/x-api/x-vector3d.h
  ${CMAKE_SOURCE_DIR}/include/y-api/y-entity.cpp
  ${CMAKE_SOURCE_DIR}/include/y-api/y-entity.h
  ${CMAKE_SOURCE_DIR}/include/x-api/x-gfx.cpp
  ${CMAKE_SOURCE_DIR}/include/x-api/x-gfx.h
  ${CMAKE_SOURCE_DIR}/include/x-api/x-thread.cpp
  ${CMAKE_SOURCE_DIR}/include/x-api/x-thread.h
  ${rtaudio_SOURCES}
  ${stk_SOURCES}
)

set(core_SOURCES 
  ${CMAKE_SOURCE_DIR}/core/theremax-globals.h
  ${CMAKE_SOURCE_DIR}/core/theremax-globals.cpp
  
  ${CMAKE_SOURCE_DIR}/core/theremax-audio.cpp
  ${CMAKE_SOURCE_DIR}/core/theremax-audio.h
  ${xyapi_SOURCES}
)

set(theremax_SOURCES
  src/theremax.cpp
  ${core_SOURCES}
)

add_executable( theremax ${theremax_SOURCES} )
target_link_libraries( theremax ${OpenCV_LIBS} ${GLUT_LIBRARY} ${OPENGL_LIBRARY} ${LIBS} )

