# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.8

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


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
CMAKE_COMMAND = /opt/tools/cmake/3.8.0/bin/cmake

# The command to remove a file.
RM = /opt/tools/cmake/3.8.0/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/shijia/Public/ScDebug

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/shijia/Public/ScDebug

# Include any dependencies generated for this target.
include CMakeFiles/transblock.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/transblock.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/transblock.dir/flags.make

CMakeFiles/transblock.dir/src/transblock/TACTransBlockform.cpp.o: CMakeFiles/transblock.dir/flags.make
CMakeFiles/transblock.dir/src/transblock/TACTransBlockform.cpp.o: src/transblock/TACTransBlockform.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/shijia/Public/ScDebug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/transblock.dir/src/transblock/TACTransBlockform.cpp.o"
	/opt/compilers/gcc/6.5.0/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/transblock.dir/src/transblock/TACTransBlockform.cpp.o -c /home/shijia/Public/ScDebug/src/transblock/TACTransBlockform.cpp

CMakeFiles/transblock.dir/src/transblock/TACTransBlockform.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/transblock.dir/src/transblock/TACTransBlockform.cpp.i"
	/opt/compilers/gcc/6.5.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/shijia/Public/ScDebug/src/transblock/TACTransBlockform.cpp > CMakeFiles/transblock.dir/src/transblock/TACTransBlockform.cpp.i

CMakeFiles/transblock.dir/src/transblock/TACTransBlockform.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/transblock.dir/src/transblock/TACTransBlockform.cpp.s"
	/opt/compilers/gcc/6.5.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/shijia/Public/ScDebug/src/transblock/TACTransBlockform.cpp -o CMakeFiles/transblock.dir/src/transblock/TACTransBlockform.cpp.s

CMakeFiles/transblock.dir/src/transblock/TACTransBlockform.cpp.o.requires:

.PHONY : CMakeFiles/transblock.dir/src/transblock/TACTransBlockform.cpp.o.requires

CMakeFiles/transblock.dir/src/transblock/TACTransBlockform.cpp.o.provides: CMakeFiles/transblock.dir/src/transblock/TACTransBlockform.cpp.o.requires
	$(MAKE) -f CMakeFiles/transblock.dir/build.make CMakeFiles/transblock.dir/src/transblock/TACTransBlockform.cpp.o.provides.build
.PHONY : CMakeFiles/transblock.dir/src/transblock/TACTransBlockform.cpp.o.provides

CMakeFiles/transblock.dir/src/transblock/TACTransBlockform.cpp.o.provides.build: CMakeFiles/transblock.dir/src/transblock/TACTransBlockform.cpp.o


# Object files for target transblock
transblock_OBJECTS = \
"CMakeFiles/transblock.dir/src/transblock/TACTransBlockform.cpp.o"

# External object files for target transblock
transblock_EXTERNAL_OBJECTS =

bin/transblock: CMakeFiles/transblock.dir/src/transblock/TACTransBlockform.cpp.o
bin/transblock: CMakeFiles/transblock.dir/build.make
bin/transblock: CMakeFiles/transblock.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/shijia/Public/ScDebug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable bin/transblock"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/transblock.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/transblock.dir/build: bin/transblock

.PHONY : CMakeFiles/transblock.dir/build

CMakeFiles/transblock.dir/requires: CMakeFiles/transblock.dir/src/transblock/TACTransBlockform.cpp.o.requires

.PHONY : CMakeFiles/transblock.dir/requires

CMakeFiles/transblock.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/transblock.dir/cmake_clean.cmake
.PHONY : CMakeFiles/transblock.dir/clean

CMakeFiles/transblock.dir/depend:
	cd /home/shijia/Public/ScDebug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/shijia/Public/ScDebug /home/shijia/Public/ScDebug /home/shijia/Public/ScDebug /home/shijia/Public/ScDebug /home/shijia/Public/ScDebug/CMakeFiles/transblock.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/transblock.dir/depend

