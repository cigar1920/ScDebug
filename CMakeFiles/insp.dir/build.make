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
include CMakeFiles/insp.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/insp.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/insp.dir/flags.make

CMakeFiles/insp.dir/src/insert/InsertParen.cpp.o: CMakeFiles/insp.dir/flags.make
CMakeFiles/insp.dir/src/insert/InsertParen.cpp.o: src/insert/InsertParen.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/shijia/Public/ScDebug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/insp.dir/src/insert/InsertParen.cpp.o"
	/opt/compilers/gcc/6.5.0/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/insp.dir/src/insert/InsertParen.cpp.o -c /home/shijia/Public/ScDebug/src/insert/InsertParen.cpp

CMakeFiles/insp.dir/src/insert/InsertParen.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/insp.dir/src/insert/InsertParen.cpp.i"
	/opt/compilers/gcc/6.5.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/shijia/Public/ScDebug/src/insert/InsertParen.cpp > CMakeFiles/insp.dir/src/insert/InsertParen.cpp.i

CMakeFiles/insp.dir/src/insert/InsertParen.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/insp.dir/src/insert/InsertParen.cpp.s"
	/opt/compilers/gcc/6.5.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/shijia/Public/ScDebug/src/insert/InsertParen.cpp -o CMakeFiles/insp.dir/src/insert/InsertParen.cpp.s

CMakeFiles/insp.dir/src/insert/InsertParen.cpp.o.requires:

.PHONY : CMakeFiles/insp.dir/src/insert/InsertParen.cpp.o.requires

CMakeFiles/insp.dir/src/insert/InsertParen.cpp.o.provides: CMakeFiles/insp.dir/src/insert/InsertParen.cpp.o.requires
	$(MAKE) -f CMakeFiles/insp.dir/build.make CMakeFiles/insp.dir/src/insert/InsertParen.cpp.o.provides.build
.PHONY : CMakeFiles/insp.dir/src/insert/InsertParen.cpp.o.provides

CMakeFiles/insp.dir/src/insert/InsertParen.cpp.o.provides.build: CMakeFiles/insp.dir/src/insert/InsertParen.cpp.o


# Object files for target insp
insp_OBJECTS = \
"CMakeFiles/insp.dir/src/insert/InsertParen.cpp.o"

# External object files for target insp
insp_EXTERNAL_OBJECTS =

bin/insp: CMakeFiles/insp.dir/src/insert/InsertParen.cpp.o
bin/insp: CMakeFiles/insp.dir/build.make
bin/insp: CMakeFiles/insp.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/shijia/Public/ScDebug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable bin/insp"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/insp.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/insp.dir/build: bin/insp

.PHONY : CMakeFiles/insp.dir/build

CMakeFiles/insp.dir/requires: CMakeFiles/insp.dir/src/insert/InsertParen.cpp.o.requires

.PHONY : CMakeFiles/insp.dir/requires

CMakeFiles/insp.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/insp.dir/cmake_clean.cmake
.PHONY : CMakeFiles/insp.dir/clean

CMakeFiles/insp.dir/depend:
	cd /home/shijia/Public/ScDebug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/shijia/Public/ScDebug /home/shijia/Public/ScDebug /home/shijia/Public/ScDebug /home/shijia/Public/ScDebug /home/shijia/Public/ScDebug/CMakeFiles/insp.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/insp.dir/depend
