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
CMAKE_SOURCE_DIR = /home/jwu/Documents/Github/EECS405Project/flamingo-4.1/src/partenum

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jwu/Documents/Github/EECS405Project/flamingo-4.1/src/partenum

# Include any dependencies generated for this target.
include CMakeFiles/partenum-lib.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/partenum-lib.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/partenum-lib.dir/flags.make

CMakeFiles/partenum-lib.dir/src/partenum.cc.o: CMakeFiles/partenum-lib.dir/flags.make
CMakeFiles/partenum-lib.dir/src/partenum.cc.o: src/partenum.cc
	$(CMAKE_COMMAND) -E cmake_progress_report /home/jwu/Documents/Github/EECS405Project/flamingo-4.1/src/partenum/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/partenum-lib.dir/src/partenum.cc.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/partenum-lib.dir/src/partenum.cc.o -c /home/jwu/Documents/Github/EECS405Project/flamingo-4.1/src/partenum/src/partenum.cc

CMakeFiles/partenum-lib.dir/src/partenum.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/partenum-lib.dir/src/partenum.cc.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/jwu/Documents/Github/EECS405Project/flamingo-4.1/src/partenum/src/partenum.cc > CMakeFiles/partenum-lib.dir/src/partenum.cc.i

CMakeFiles/partenum-lib.dir/src/partenum.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/partenum-lib.dir/src/partenum.cc.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/jwu/Documents/Github/EECS405Project/flamingo-4.1/src/partenum/src/partenum.cc -o CMakeFiles/partenum-lib.dir/src/partenum.cc.s

CMakeFiles/partenum-lib.dir/src/partenum.cc.o.requires:
.PHONY : CMakeFiles/partenum-lib.dir/src/partenum.cc.o.requires

CMakeFiles/partenum-lib.dir/src/partenum.cc.o.provides: CMakeFiles/partenum-lib.dir/src/partenum.cc.o.requires
	$(MAKE) -f CMakeFiles/partenum-lib.dir/build.make CMakeFiles/partenum-lib.dir/src/partenum.cc.o.provides.build
.PHONY : CMakeFiles/partenum-lib.dir/src/partenum.cc.o.provides

CMakeFiles/partenum-lib.dir/src/partenum.cc.o.provides.build: CMakeFiles/partenum-lib.dir/src/partenum.cc.o

# Object files for target partenum-lib
partenum__lib_OBJECTS = \
"CMakeFiles/partenum-lib.dir/src/partenum.cc.o"

# External object files for target partenum-lib
partenum__lib_EXTERNAL_OBJECTS =

build/libpartenum-lib.so: CMakeFiles/partenum-lib.dir/src/partenum.cc.o
build/libpartenum-lib.so: CMakeFiles/partenum-lib.dir/build.make
build/libpartenum-lib.so: CMakeFiles/partenum-lib.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX shared library build/libpartenum-lib.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/partenum-lib.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/partenum-lib.dir/build: build/libpartenum-lib.so
.PHONY : CMakeFiles/partenum-lib.dir/build

CMakeFiles/partenum-lib.dir/requires: CMakeFiles/partenum-lib.dir/src/partenum.cc.o.requires
.PHONY : CMakeFiles/partenum-lib.dir/requires

CMakeFiles/partenum-lib.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/partenum-lib.dir/cmake_clean.cmake
.PHONY : CMakeFiles/partenum-lib.dir/clean

CMakeFiles/partenum-lib.dir/depend:
	cd /home/jwu/Documents/Github/EECS405Project/flamingo-4.1/src/partenum && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jwu/Documents/Github/EECS405Project/flamingo-4.1/src/partenum /home/jwu/Documents/Github/EECS405Project/flamingo-4.1/src/partenum /home/jwu/Documents/Github/EECS405Project/flamingo-4.1/src/partenum /home/jwu/Documents/Github/EECS405Project/flamingo-4.1/src/partenum /home/jwu/Documents/Github/EECS405Project/flamingo-4.1/src/partenum/CMakeFiles/partenum-lib.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/partenum-lib.dir/depend

