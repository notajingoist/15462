# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canoncical targets will work.
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

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/build

# Include any dependencies generated for this target.
include tinyxml/CMakeFiles/tinyxml.dir/depend.make

# Include the progress variables for this target.
include tinyxml/CMakeFiles/tinyxml.dir/progress.make

# Include the compile flags for this target's objects.
include tinyxml/CMakeFiles/tinyxml.dir/flags.make

tinyxml/CMakeFiles/tinyxml.dir/tinyxml.cpp.o: tinyxml/CMakeFiles/tinyxml.dir/flags.make
tinyxml/CMakeFiles/tinyxml.dir/tinyxml.cpp.o: /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/src/tinyxml/tinyxml.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object tinyxml/CMakeFiles/tinyxml.dir/tinyxml.cpp.o"
	cd /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/build/tinyxml && /usr/local/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/tinyxml.dir/tinyxml.cpp.o -c /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/src/tinyxml/tinyxml.cpp

tinyxml/CMakeFiles/tinyxml.dir/tinyxml.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tinyxml.dir/tinyxml.cpp.i"
	cd /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/build/tinyxml && /usr/local/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/src/tinyxml/tinyxml.cpp > CMakeFiles/tinyxml.dir/tinyxml.cpp.i

tinyxml/CMakeFiles/tinyxml.dir/tinyxml.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tinyxml.dir/tinyxml.cpp.s"
	cd /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/build/tinyxml && /usr/local/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/src/tinyxml/tinyxml.cpp -o CMakeFiles/tinyxml.dir/tinyxml.cpp.s

tinyxml/CMakeFiles/tinyxml.dir/tinyxml.cpp.o.requires:
.PHONY : tinyxml/CMakeFiles/tinyxml.dir/tinyxml.cpp.o.requires

tinyxml/CMakeFiles/tinyxml.dir/tinyxml.cpp.o.provides: tinyxml/CMakeFiles/tinyxml.dir/tinyxml.cpp.o.requires
	$(MAKE) -f tinyxml/CMakeFiles/tinyxml.dir/build.make tinyxml/CMakeFiles/tinyxml.dir/tinyxml.cpp.o.provides.build
.PHONY : tinyxml/CMakeFiles/tinyxml.dir/tinyxml.cpp.o.provides

tinyxml/CMakeFiles/tinyxml.dir/tinyxml.cpp.o.provides.build: tinyxml/CMakeFiles/tinyxml.dir/tinyxml.cpp.o

tinyxml/CMakeFiles/tinyxml.dir/tinyxmlerror.cpp.o: tinyxml/CMakeFiles/tinyxml.dir/flags.make
tinyxml/CMakeFiles/tinyxml.dir/tinyxmlerror.cpp.o: /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/src/tinyxml/tinyxmlerror.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object tinyxml/CMakeFiles/tinyxml.dir/tinyxmlerror.cpp.o"
	cd /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/build/tinyxml && /usr/local/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/tinyxml.dir/tinyxmlerror.cpp.o -c /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/src/tinyxml/tinyxmlerror.cpp

tinyxml/CMakeFiles/tinyxml.dir/tinyxmlerror.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tinyxml.dir/tinyxmlerror.cpp.i"
	cd /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/build/tinyxml && /usr/local/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/src/tinyxml/tinyxmlerror.cpp > CMakeFiles/tinyxml.dir/tinyxmlerror.cpp.i

tinyxml/CMakeFiles/tinyxml.dir/tinyxmlerror.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tinyxml.dir/tinyxmlerror.cpp.s"
	cd /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/build/tinyxml && /usr/local/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/src/tinyxml/tinyxmlerror.cpp -o CMakeFiles/tinyxml.dir/tinyxmlerror.cpp.s

tinyxml/CMakeFiles/tinyxml.dir/tinyxmlerror.cpp.o.requires:
.PHONY : tinyxml/CMakeFiles/tinyxml.dir/tinyxmlerror.cpp.o.requires

tinyxml/CMakeFiles/tinyxml.dir/tinyxmlerror.cpp.o.provides: tinyxml/CMakeFiles/tinyxml.dir/tinyxmlerror.cpp.o.requires
	$(MAKE) -f tinyxml/CMakeFiles/tinyxml.dir/build.make tinyxml/CMakeFiles/tinyxml.dir/tinyxmlerror.cpp.o.provides.build
.PHONY : tinyxml/CMakeFiles/tinyxml.dir/tinyxmlerror.cpp.o.provides

tinyxml/CMakeFiles/tinyxml.dir/tinyxmlerror.cpp.o.provides.build: tinyxml/CMakeFiles/tinyxml.dir/tinyxmlerror.cpp.o

tinyxml/CMakeFiles/tinyxml.dir/tinyxmlparser.cpp.o: tinyxml/CMakeFiles/tinyxml.dir/flags.make
tinyxml/CMakeFiles/tinyxml.dir/tinyxmlparser.cpp.o: /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/src/tinyxml/tinyxmlparser.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/build/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object tinyxml/CMakeFiles/tinyxml.dir/tinyxmlparser.cpp.o"
	cd /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/build/tinyxml && /usr/local/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/tinyxml.dir/tinyxmlparser.cpp.o -c /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/src/tinyxml/tinyxmlparser.cpp

tinyxml/CMakeFiles/tinyxml.dir/tinyxmlparser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tinyxml.dir/tinyxmlparser.cpp.i"
	cd /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/build/tinyxml && /usr/local/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/src/tinyxml/tinyxmlparser.cpp > CMakeFiles/tinyxml.dir/tinyxmlparser.cpp.i

tinyxml/CMakeFiles/tinyxml.dir/tinyxmlparser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tinyxml.dir/tinyxmlparser.cpp.s"
	cd /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/build/tinyxml && /usr/local/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/src/tinyxml/tinyxmlparser.cpp -o CMakeFiles/tinyxml.dir/tinyxmlparser.cpp.s

tinyxml/CMakeFiles/tinyxml.dir/tinyxmlparser.cpp.o.requires:
.PHONY : tinyxml/CMakeFiles/tinyxml.dir/tinyxmlparser.cpp.o.requires

tinyxml/CMakeFiles/tinyxml.dir/tinyxmlparser.cpp.o.provides: tinyxml/CMakeFiles/tinyxml.dir/tinyxmlparser.cpp.o.requires
	$(MAKE) -f tinyxml/CMakeFiles/tinyxml.dir/build.make tinyxml/CMakeFiles/tinyxml.dir/tinyxmlparser.cpp.o.provides.build
.PHONY : tinyxml/CMakeFiles/tinyxml.dir/tinyxmlparser.cpp.o.provides

tinyxml/CMakeFiles/tinyxml.dir/tinyxmlparser.cpp.o.provides.build: tinyxml/CMakeFiles/tinyxml.dir/tinyxmlparser.cpp.o

# Object files for target tinyxml
tinyxml_OBJECTS = \
"CMakeFiles/tinyxml.dir/tinyxml.cpp.o" \
"CMakeFiles/tinyxml.dir/tinyxmlerror.cpp.o" \
"CMakeFiles/tinyxml.dir/tinyxmlparser.cpp.o"

# External object files for target tinyxml
tinyxml_EXTERNAL_OBJECTS =

tinyxml/libtinyxml.a: tinyxml/CMakeFiles/tinyxml.dir/tinyxml.cpp.o
tinyxml/libtinyxml.a: tinyxml/CMakeFiles/tinyxml.dir/tinyxmlerror.cpp.o
tinyxml/libtinyxml.a: tinyxml/CMakeFiles/tinyxml.dir/tinyxmlparser.cpp.o
tinyxml/libtinyxml.a: tinyxml/CMakeFiles/tinyxml.dir/build.make
tinyxml/libtinyxml.a: tinyxml/CMakeFiles/tinyxml.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX static library libtinyxml.a"
	cd /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/build/tinyxml && $(CMAKE_COMMAND) -P CMakeFiles/tinyxml.dir/cmake_clean_target.cmake
	cd /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/build/tinyxml && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tinyxml.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tinyxml/CMakeFiles/tinyxml.dir/build: tinyxml/libtinyxml.a
.PHONY : tinyxml/CMakeFiles/tinyxml.dir/build

tinyxml/CMakeFiles/tinyxml.dir/requires: tinyxml/CMakeFiles/tinyxml.dir/tinyxml.cpp.o.requires
tinyxml/CMakeFiles/tinyxml.dir/requires: tinyxml/CMakeFiles/tinyxml.dir/tinyxmlerror.cpp.o.requires
tinyxml/CMakeFiles/tinyxml.dir/requires: tinyxml/CMakeFiles/tinyxml.dir/tinyxmlparser.cpp.o.requires
.PHONY : tinyxml/CMakeFiles/tinyxml.dir/requires

tinyxml/CMakeFiles/tinyxml.dir/clean:
	cd /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/build/tinyxml && $(CMAKE_COMMAND) -P CMakeFiles/tinyxml.dir/cmake_clean.cmake
.PHONY : tinyxml/CMakeFiles/tinyxml.dir/clean

tinyxml/CMakeFiles/tinyxml.dir/depend:
	cd /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/src /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/src/tinyxml /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/build /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/build/tinyxml /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/build/tinyxml/CMakeFiles/tinyxml.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tinyxml/CMakeFiles/tinyxml.dir/depend
