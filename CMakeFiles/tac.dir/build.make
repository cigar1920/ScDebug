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
include CMakeFiles/tac.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/tac.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/tac.dir/flags.make

CMakeFiles/tac.dir/src/trans/TACTransform.cpp.o: CMakeFiles/tac.dir/flags.make
CMakeFiles/tac.dir/src/trans/TACTransform.cpp.o: src/trans/TACTransform.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/shijia/Public/ScDebug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/tac.dir/src/trans/TACTransform.cpp.o"
	/opt/compilers/gcc/6.5.0/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tac.dir/src/trans/TACTransform.cpp.o -c /home/shijia/Public/ScDebug/src/trans/TACTransform.cpp

CMakeFiles/tac.dir/src/trans/TACTransform.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tac.dir/src/trans/TACTransform.cpp.i"
	/opt/compilers/gcc/6.5.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/shijia/Public/ScDebug/src/trans/TACTransform.cpp > CMakeFiles/tac.dir/src/trans/TACTransform.cpp.i

CMakeFiles/tac.dir/src/trans/TACTransform.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tac.dir/src/trans/TACTransform.cpp.s"
	/opt/compilers/gcc/6.5.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/shijia/Public/ScDebug/src/trans/TACTransform.cpp -o CMakeFiles/tac.dir/src/trans/TACTransform.cpp.s

CMakeFiles/tac.dir/src/trans/TACTransform.cpp.o.requires:

.PHONY : CMakeFiles/tac.dir/src/trans/TACTransform.cpp.o.requires

CMakeFiles/tac.dir/src/trans/TACTransform.cpp.o.provides: CMakeFiles/tac.dir/src/trans/TACTransform.cpp.o.requires
	$(MAKE) -f CMakeFiles/tac.dir/build.make CMakeFiles/tac.dir/src/trans/TACTransform.cpp.o.provides.build
.PHONY : CMakeFiles/tac.dir/src/trans/TACTransform.cpp.o.provides

CMakeFiles/tac.dir/src/trans/TACTransform.cpp.o.provides.build: CMakeFiles/tac.dir/src/trans/TACTransform.cpp.o


# Object files for target tac
tac_OBJECTS = \
"CMakeFiles/tac.dir/src/trans/TACTransform.cpp.o"

# External object files for target tac
tac_EXTERNAL_OBJECTS =

bin/tac: CMakeFiles/tac.dir/src/trans/TACTransform.cpp.o
bin/tac: CMakeFiles/tac.dir/build.make
bin/tac: CMakeFiles/tac.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/shijia/Public/ScDebug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable bin/tac"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tac.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/tac.dir/build: bin/tac

.PHONY : CMakeFiles/tac.dir/build

CMakeFiles/tac.dir/requires: CMakeFiles/tac.dir/src/trans/TACTransform.cpp.o.requires

.PHONY : CMakeFiles/tac.dir/requires

CMakeFiles/tac.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/tac.dir/cmake_clean.cmake
.PHONY : CMakeFiles/tac.dir/clean

CMakeFiles/tac.dir/depend:
	cd /home/shijia/Public/ScDebug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/shijia/Public/ScDebug /home/shijia/Public/ScDebug /home/shijia/Public/ScDebug /home/shijia/Public/ScDebug /home/shijia/Public/ScDebug/CMakeFiles/tac.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/tac.dir/depend

