# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/Cellar/cmake/2.8.12.1/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/2.8.12.1/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/local/Cellar/cmake/2.8.12.1/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/thealphanerd/github/newOpencv

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/thealphanerd/github/newOpencv

# Include any dependencies generated for this target.
include CMakeFiles/bin/DisplayImage.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/bin/DisplayImage.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/bin/DisplayImage.dir/flags.make

CMakeFiles/bin/DisplayImage.dir/src/DisplayImage.cpp.o: CMakeFiles/bin/DisplayImage.dir/flags.make
CMakeFiles/bin/DisplayImage.dir/src/DisplayImage.cpp.o: src/DisplayImage.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/thealphanerd/github/newOpencv/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/bin/DisplayImage.dir/src/DisplayImage.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/bin/DisplayImage.dir/src/DisplayImage.cpp.o -c /Users/thealphanerd/github/newOpencv/src/DisplayImage.cpp

CMakeFiles/bin/DisplayImage.dir/src/DisplayImage.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/bin/DisplayImage.dir/src/DisplayImage.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/thealphanerd/github/newOpencv/src/DisplayImage.cpp > CMakeFiles/bin/DisplayImage.dir/src/DisplayImage.cpp.i

CMakeFiles/bin/DisplayImage.dir/src/DisplayImage.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/bin/DisplayImage.dir/src/DisplayImage.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/thealphanerd/github/newOpencv/src/DisplayImage.cpp -o CMakeFiles/bin/DisplayImage.dir/src/DisplayImage.cpp.s

CMakeFiles/bin/DisplayImage.dir/src/DisplayImage.cpp.o.requires:
.PHONY : CMakeFiles/bin/DisplayImage.dir/src/DisplayImage.cpp.o.requires

CMakeFiles/bin/DisplayImage.dir/src/DisplayImage.cpp.o.provides: CMakeFiles/bin/DisplayImage.dir/src/DisplayImage.cpp.o.requires
	$(MAKE) -f CMakeFiles/bin/DisplayImage.dir/build.make CMakeFiles/bin/DisplayImage.dir/src/DisplayImage.cpp.o.provides.build
.PHONY : CMakeFiles/bin/DisplayImage.dir/src/DisplayImage.cpp.o.provides

CMakeFiles/bin/DisplayImage.dir/src/DisplayImage.cpp.o.provides.build: CMakeFiles/bin/DisplayImage.dir/src/DisplayImage.cpp.o

# Object files for target bin/DisplayImage
bin/DisplayImage_OBJECTS = \
"CMakeFiles/bin/DisplayImage.dir/src/DisplayImage.cpp.o"

# External object files for target bin/DisplayImage
bin/DisplayImage_EXTERNAL_OBJECTS =

bin/DisplayImage: CMakeFiles/bin/DisplayImage.dir/src/DisplayImage.cpp.o
bin/DisplayImage: CMakeFiles/bin/DisplayImage.dir/build.make
bin/DisplayImage: /usr/local/lib/libopencv_calib3d.dylib
bin/DisplayImage: /usr/local/lib/libopencv_contrib.dylib
bin/DisplayImage: /usr/local/lib/libopencv_core.dylib
bin/DisplayImage: /usr/local/lib/libopencv_features2d.dylib
bin/DisplayImage: /usr/local/lib/libopencv_flann.dylib
bin/DisplayImage: /usr/local/lib/libopencv_gpu.dylib
bin/DisplayImage: /usr/local/lib/libopencv_highgui.dylib
bin/DisplayImage: /usr/local/lib/libopencv_imgproc.dylib
bin/DisplayImage: /usr/local/lib/libopencv_legacy.dylib
bin/DisplayImage: /usr/local/lib/libopencv_ml.dylib
bin/DisplayImage: /usr/local/lib/libopencv_nonfree.dylib
bin/DisplayImage: /usr/local/lib/libopencv_objdetect.dylib
bin/DisplayImage: /usr/local/lib/libopencv_ocl.dylib
bin/DisplayImage: /usr/local/lib/libopencv_photo.dylib
bin/DisplayImage: /usr/local/lib/libopencv_stitching.dylib
bin/DisplayImage: /usr/local/lib/libopencv_superres.dylib
bin/DisplayImage: /usr/local/lib/libopencv_ts.dylib
bin/DisplayImage: /usr/local/lib/libopencv_video.dylib
bin/DisplayImage: /usr/local/lib/libopencv_videostab.dylib
bin/DisplayImage: CMakeFiles/bin/DisplayImage.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable bin/DisplayImage"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/bin/DisplayImage.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/bin/DisplayImage.dir/build: bin/DisplayImage
.PHONY : CMakeFiles/bin/DisplayImage.dir/build

CMakeFiles/bin/DisplayImage.dir/requires: CMakeFiles/bin/DisplayImage.dir/src/DisplayImage.cpp.o.requires
.PHONY : CMakeFiles/bin/DisplayImage.dir/requires

CMakeFiles/bin/DisplayImage.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/bin/DisplayImage.dir/cmake_clean.cmake
.PHONY : CMakeFiles/bin/DisplayImage.dir/clean

CMakeFiles/bin/DisplayImage.dir/depend:
	cd /Users/thealphanerd/github/newOpencv && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/thealphanerd/github/newOpencv /Users/thealphanerd/github/newOpencv /Users/thealphanerd/github/newOpencv /Users/thealphanerd/github/newOpencv /Users/thealphanerd/github/newOpencv/CMakeFiles/bin/DisplayImage.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/bin/DisplayImage.dir/depend
