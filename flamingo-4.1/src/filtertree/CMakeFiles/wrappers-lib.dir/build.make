# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.0

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/jwu/Documents/Github/EECS405Project/test/src/filtertree

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jwu/Documents/Github/EECS405Project/test/src/filtertree

# Include any dependencies generated for this target.
include CMakeFiles/wrappers-lib.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/wrappers-lib.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/wrappers-lib.dir/flags.make

CMakeFiles/wrappers-lib.dir/src/wrappers/wrappers.cc.o: CMakeFiles/wrappers-lib.dir/flags.make
CMakeFiles/wrappers-lib.dir/src/wrappers/wrappers.cc.o: src/wrappers/wrappers.cc
	$(CMAKE_COMMAND) -E cmake_progress_report /home/jwu/Documents/Github/EECS405Project/test/src/filtertree/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/wrappers-lib.dir/src/wrappers/wrappers.cc.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/wrappers-lib.dir/src/wrappers/wrappers.cc.o -c /home/jwu/Documents/Github/EECS405Project/test/src/filtertree/src/wrappers/wrappers.cc

CMakeFiles/wrappers-lib.dir/src/wrappers/wrappers.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/wrappers-lib.dir/src/wrappers/wrappers.cc.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/jwu/Documents/Github/EECS405Project/test/src/filtertree/src/wrappers/wrappers.cc > CMakeFiles/wrappers-lib.dir/src/wrappers/wrappers.cc.i

CMakeFiles/wrappers-lib.dir/src/wrappers/wrappers.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/wrappers-lib.dir/src/wrappers/wrappers.cc.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/jwu/Documents/Github/EECS405Project/test/src/filtertree/src/wrappers/wrappers.cc -o CMakeFiles/wrappers-lib.dir/src/wrappers/wrappers.cc.s

CMakeFiles/wrappers-lib.dir/src/wrappers/wrappers.cc.o.requires:
.PHONY : CMakeFiles/wrappers-lib.dir/src/wrappers/wrappers.cc.o.requires

CMakeFiles/wrappers-lib.dir/src/wrappers/wrappers.cc.o.provides: CMakeFiles/wrappers-lib.dir/src/wrappers/wrappers.cc.o.requires
	$(MAKE) -f CMakeFiles/wrappers-lib.dir/build.make CMakeFiles/wrappers-lib.dir/src/wrappers/wrappers.cc.o.provides.build
.PHONY : CMakeFiles/wrappers-lib.dir/src/wrappers/wrappers.cc.o.provides

CMakeFiles/wrappers-lib.dir/src/wrappers/wrappers.cc.o.provides.build: CMakeFiles/wrappers-lib.dir/src/wrappers/wrappers.cc.o

# Object files for target wrappers-lib
wrappers__lib_OBJECTS = \
"CMakeFiles/wrappers-lib.dir/src/wrappers/wrappers.cc.o"

# External object files for target wrappers-lib
wrappers__lib_EXTERNAL_OBJECTS =

build/libwrappers-lib.so: CMakeFiles/wrappers-lib.dir/src/wrappers/wrappers.cc.o
build/libwrappers-lib.so: CMakeFiles/wrappers-lib.dir/build.make
build/libwrappers-lib.so: CMakeFiles/wrappers-lib.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX shared library build/libwrappers-lib.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/wrappers-lib.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/wrappers-lib.dir/build: build/libwrappers-lib.so
.PHONY : CMakeFiles/wrappers-lib.dir/build

CMakeFiles/wrappers-lib.dir/requires: CMakeFiles/wrappers-lib.dir/src/wrappers/wrappers.cc.o.requires
.PHONY : CMakeFiles/wrappers-lib.dir/requires

CMakeFiles/wrappers-lib.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/wrappers-lib.dir/cmake_clean.cmake
.PHONY : CMakeFiles/wrappers-lib.dir/clean

CMakeFiles/wrappers-lib.dir/depend:
	cd /home/jwu/Documents/Github/EECS405Project/test/src/filtertree && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jwu/Documents/Github/EECS405Project/test/src/filtertree /home/jwu/Documents/Github/EECS405Project/test/src/filtertree /home/jwu/Documents/Github/EECS405Project/test/src/filtertree /home/jwu/Documents/Github/EECS405Project/test/src/filtertree /home/jwu/Documents/Github/EECS405Project/test/src/filtertree/CMakeFiles/wrappers-lib.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/wrappers-lib.dir/depend
