cmake_minimum_required(VERSION 3.5)
project( theremax )

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

message("Configuring for your deps")

# Make somewhere to store audio lib stuffs
set(AUDIO_LIBRARIES "")

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  find_package (Threads)
  message("You are on linux, so lets setup jack")
  find_package(ALSA)
  find_package(Jack)
  if(JACK_FOUND)
    add_definitions(-D__UNIX_JACK__)
    include_directories( ${JACK_INCLUDE_DIRS} )
    set( LIBS 
      ${LIBS}
      ${JACK_LIBRARY}
      ${CMAKE_THREAD_LIBS_INIT}
    )
  endif(JACK_FOUND)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  message("You are on osx, so lets setup coreaudio")

  # setting up foundation can be a pain
  add_definitions(-D__MACOSX_CORE__)

  # Setup core audio
  find_path(COREAUDIO_INCLUDE_PATH CoreAudio.h)
  find_library(COREAUDIO_LIBRARY CoreAudio)

  # setup core midi
  find_path(COREMIDI_INCLUDE_PATH CoreMidi.h)
  find_library(COREMIDI_LIBRARY CoreMidi)
  find_library(COREFOUNDATION_LIBRARY CoreFoundation)

  # setup carbon
  find_library(CARBON_LIBRARY Carbon)
  # setup IOKit
  find_library(IOKIT_LIBRARY IOKit)
  # setup Foundation
  find_library(FOUNDATION_LIBRARY Foundation)
  # setup AppKit
  find_library(APPKIT_LIBRARY AppKit)
  
  
  # get includes updated
  include_directories(${COREAUDIO_INCLUDE_PATH})
  include_directories(${COREMIDI_INCLUDE_PATH})
  
  # libraries will be linked later
  set( LIBS
    ${LIBS}
    ${COREAUDIO_LIBRARY}
    ${COREMIDI_LIBRARY}
    ${CARBON_LIBRARY}
    ${IOKIT_LIBRARY}
    ${APPKIT_LIBRARY}
    ${FOUNDATION_LIBRARY}
  )
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

# Require OpenCV, OpenGl, and GLUT using cmake modules
find_package( OpenCV REQUIRED )
find_package( OpenGL REQUIRED )
find_package( GLUT REQUIRED )

# Properly link GLUT
include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})

# Get liblo in there
find_package( liblo REQUIRED )
include_directories(${liblo_INCLUDE_DIRS})
set(LIBS ${LIBS} ${liblo_LIBRARIES})

#include audio
include_directories( ${CMAKE_SOURCE_DIR}/src/audio )
#link audio
link_directories( ${CMAKE_SOURCE_DIR}/src/audio )

#include cv
include_directories( ${CMAKE_SOURCE_DIR}/src/cv )
#link cv
link_directories( ${CMAKE_SOURCE_DIR}/src/cv )

#include globals
include_directories( ${CMAKE_SOURCE_DIR}/src/globals )
#link globals
link_directories( ${CMAKE_SOURCE_DIR}/src/globals )

#include graphics
include_directories( ${CMAKE_SOURCE_DIR}/src/graphics )
#link graphics
link_directories( ${CMAKE_SOURCE_DIR}/src/graphics )

# include rtaudio
include_directories( ${CMAKE_SOURCE_DIR}/include/rtaudio )
# link rtaudio
link_directories( ${CMAKE_SOURCE_DIR}/include/rtaudio )

# include stk
include_directories( ${CMAKE_SOURCE_DIR}/include/stk )
# link stk
link_directories( ${CMAKE_SOURCE_DIR}/include/stk )

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
)
# Setup Sources for stk
set(stk_SOURCES
  ${CMAKE_SOURCE_DIR}/include/stk/Stk.cpp
  ${CMAKE_SOURCE_DIR}/include/stk/Stk.h
  ${CMAKE_SOURCE_DIR}/include/stk/Filter.h
  ${CMAKE_SOURCE_DIR}/include/stk/Delay.cpp
  ${CMAKE_SOURCE_DIR}/include/stk/Delay.h
  ${CMAKE_SOURCE_DIR}/include/stk/BiQuad.cpp
  ${CMAKE_SOURCE_DIR}/include/stk/BiQuad.h
)
# Setup Sources for x-api
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
  ${CMAKE_SOURCE_DIR}/include/x-api/x-fun.cpp
  ${CMAKE_SOURCE_DIR}/include/x-api/x-fun.h
  ${rtaudio_SOURCES}
  ${stk_SOURCES}
)
# Setup Sources for core
set(core_SOURCES 
  # Globals
  ${CMAKE_SOURCE_DIR}/src/globals/theremax-globals.h
  ${CMAKE_SOURCE_DIR}/src/globals/theremax-globals.cpp
  # Audio Engine
  ${CMAKE_SOURCE_DIR}/src/audio/theremax-audio.cpp
  ${CMAKE_SOURCE_DIR}/src/audio/theremax-audio.h
  ${CMAKE_SOURCE_DIR}/src/audio/Reverb.h
  ${CMAKE_SOURCE_DIR}/src/audio/Reverb.cpp
  # Computer Vision shiz
  ${CMAKE_SOURCE_DIR}/src/cv/theremax-cv.cpp
  ${CMAKE_SOURCE_DIR}/src/cv/theremax-cv.h
  ${CMAKE_SOURCE_DIR}/src/cv/theremax-cv-thread.cpp
  ${CMAKE_SOURCE_DIR}/src/cv/theremax-cv-thread.h
  # Gfx
  ${CMAKE_SOURCE_DIR}/src/graphics/theremax-entity.cpp
  ${CMAKE_SOURCE_DIR}/src/graphics/theremax-entity.h
  ${CMAKE_SOURCE_DIR}/src/graphics/theremax-sim.cpp
  ${CMAKE_SOURCE_DIR}/src/graphics/theremax-sim.h
  ${CMAKE_SOURCE_DIR}/src/graphics/theremax-gfx.cpp
  ${CMAKE_SOURCE_DIR}/src/graphics/theremax-gfx.h
  ${CMAKE_SOURCE_DIR}/src/graphics/theremax-flocking.cpp
  ${CMAKE_SOURCE_DIR}/src/graphics/theremax-flocking.h
  
  ${xyapi_SOURCES}
)

set(theremax_SOURCES
  src/theremax.cpp
  ${core_SOURCES}
)

add_executable( theremax ${theremax_SOURCES} )
target_link_libraries( theremax 
  ${OpenCV_LIBS}
  ${GLUT_LIBRARIES}
  ${OPENGL_LIBRARIES}
  # ${Boost_LIBRARIES}
  ${LIBS}
)

# set( CMAKE_VERBOSE_MAKEFILE on )