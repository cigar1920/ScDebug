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
include CMakeFiles/partion.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/partion.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/partion.dir/flags.make

CMakeFiles/partion.dir/src/partion/PartionStmt.cpp.o: CMakeFiles/partion.dir/flags.make
CMakeFiles/partion.dir/src/partion/PartionStmt.cpp.o: src/partion/PartionStmt.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/shijia/Public/ScDebug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/partion.dir/src/partion/PartionStmt.cpp.o"
	/opt/compilers/gcc/6.5.0/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/partion.dir/src/partion/PartionStmt.cpp.o -c /home/shijia/Public/ScDebug/src/partion/PartionStmt.cpp

CMakeFiles/partion.dir/src/partion/PartionStmt.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/partion.dir/src/partion/PartionStmt.cpp.i"
	/opt/compilers/gcc/6.5.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/shijia/Public/ScDebug/src/partion/PartionStmt.cpp > CMakeFiles/partion.dir/src/partion/PartionStmt.cpp.i

CMakeFiles/partion.dir/src/partion/PartionStmt.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/partion.dir/src/partion/PartionStmt.cpp.s"
	/opt/compilers/gcc/6.5.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/shijia/Public/ScDebug/src/partion/PartionStmt.cpp -o CMakeFiles/partion.dir/src/partion/PartionStmt.cpp.s

CMakeFiles/partion.dir/src/partion/PartionStmt.cpp.o.requires:

.PHONY : CMakeFiles/partion.dir/src/partion/PartionStmt.cpp.o.requires

CMakeFiles/partion.dir/src/partion/PartionStmt.cpp.o.provides: CMakeFiles/partion.dir/src/partion/PartionStmt.cpp.o.requires
	$(MAKE) -f CMakeFiles/partion.dir/build.make CMakeFiles/partion.dir/src/partion/PartionStmt.cpp.o.provides.build
.PHONY : CMakeFiles/partion.dir/src/partion/PartionStmt.cpp.o.provides

CMakeFiles/partion.dir/src/partion/PartionStmt.cpp.o.provides.build: CMakeFiles/partion.dir/src/partion/PartionStmt.cpp.o


# Object files for target partion
partion_OBJECTS = \
"CMakeFiles/partion.dir/src/partion/PartionStmt.cpp.o"

# External object files for target partion
partion_EXTERNAL_OBJECTS =

bin/partion: CMakeFiles/partion.dir/src/partion/PartionStmt.cpp.o
bin/partion: CMakeFiles/partion.dir/build.make
bin/partion: CMakeFiles/partion.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/shijia/Public/ScDebug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable bin/partion"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/partion.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/partion.dir/build: bin/partion

.PHONY : CMakeFiles/partion.dir/build

CMakeFiles/partion.dir/requires: CMakeFiles/partion.dir/src/partion/PartionStmt.cpp.o.requires

.PHONY : CMakeFiles/partion.dir/requires

CMakeFiles/partion.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/partion.dir/cmake_clean.cmake
.PHONY : CMakeFiles/partion.dir/clean

CMakeFiles/partion.dir/depend:
	cd /home/shijia/Public/ScDebug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/shijia/Public/ScDebug /home/shijia/Public/ScDebug /home/shijia/Public/ScDebug /home/shijia/Public/ScDebug /home/shijia/Public/ScDebug/CMakeFiles/partion.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/partion.dir/depend

