# Include the directory itself as a path to include directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
 
# Create a variable called theremax_SOURCES containing all .cpp files:
set(theremax_SOURCES theremax.cpp )
 
# For a large number of source files you can create it in a simpler way
# using file() function:
# file(GLOB hellworld_SOURCES *.cpp)
 
# Create an executable file called theremax from sources:
add_executable(theremax ${theremax_SOURCES})
target_link_libraries( theremax ${OpenCV_LIBS} ${OpenGL_LIBS} ${LIBS} )