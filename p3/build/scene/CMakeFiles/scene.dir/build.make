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
include scene/CMakeFiles/scene.dir/depend.make

# Include the progress variables for this target.
include scene/CMakeFiles/scene.dir/progress.make

# Include the compile flags for this target's objects.
include scene/CMakeFiles/scene.dir/flags.make

scene/CMakeFiles/scene.dir/material.cpp.o: scene/CMakeFiles/scene.dir/flags.make
scene/CMakeFiles/scene.dir/material.cpp.o: /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/src/scene/material.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object scene/CMakeFiles/scene.dir/material.cpp.o"
	cd /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/build/scene && /usr/local/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/scene.dir/material.cpp.o -c /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/src/scene/material.cpp

scene/CMakeFiles/scene.dir/material.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/scene.dir/material.cpp.i"
	cd /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/build/scene && /usr/local/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/src/scene/material.cpp > CMakeFiles/scene.dir/material.cpp.i

scene/CMakeFiles/scene.dir/material.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/scene.dir/material.cpp.s"
	cd /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/build/scene && /usr/local/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/src/scene/material.cpp -o CMakeFiles/scene.dir/material.cpp.s

scene/CMakeFiles/scene.dir/material.cpp.o.requires:
.PHONY : scene/CMakeFiles/scene.dir/material.cpp.o.requires

scene/CMakeFiles/scene.dir/material.cpp.o.provides: scene/CMakeFiles/scene.dir/material.cpp.o.requires
	$(MAKE) -f scene/CMakeFiles/scene.dir/build.make scene/CMakeFiles/scene.dir/material.cpp.o.provides.build
.PHONY : scene/CMakeFiles/scene.dir/material.cpp.o.provides

scene/CMakeFiles/scene.dir/material.cpp.o.provides.build: scene/CMakeFiles/scene.dir/material.cpp.o

scene/CMakeFiles/scene.dir/mesh.cpp.o: scene/CMakeFiles/scene.dir/flags.make
scene/CMakeFiles/scene.dir/mesh.cpp.o: /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/src/scene/mesh.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object scene/CMakeFiles/scene.dir/mesh.cpp.o"
	cd /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/build/scene && /usr/local/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/scene.dir/mesh.cpp.o -c /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/src/scene/mesh.cpp

scene/CMakeFiles/scene.dir/mesh.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/scene.dir/mesh.cpp.i"
	cd /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/build/scene && /usr/local/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/src/scene/mesh.cpp > CMakeFiles/scene.dir/mesh.cpp.i

scene/CMakeFiles/scene.dir/mesh.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/scene.dir/mesh.cpp.s"
	cd /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/build/scene && /usr/local/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/src/scene/mesh.cpp -o CMakeFiles/scene.dir/mesh.cpp.s

scene/CMakeFiles/scene.dir/mesh.cpp.o.requires:
.PHONY : scene/CMakeFiles/scene.dir/mesh.cpp.o.requires

scene/CMakeFiles/scene.dir/mesh.cpp.o.provides: scene/CMakeFiles/scene.dir/mesh.cpp.o.requires
	$(MAKE) -f scene/CMakeFiles/scene.dir/build.make scene/CMakeFiles/scene.dir/mesh.cpp.o.provides.build
.PHONY : scene/CMakeFiles/scene.dir/mesh.cpp.o.provides

scene/CMakeFiles/scene.dir/mesh.cpp.o.provides.build: scene/CMakeFiles/scene.dir/mesh.cpp.o

scene/CMakeFiles/scene.dir/model.cpp.o: scene/CMakeFiles/scene.dir/flags.make
scene/CMakeFiles/scene.dir/model.cpp.o: /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/src/scene/model.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/build/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object scene/CMakeFiles/scene.dir/model.cpp.o"
	cd /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/build/scene && /usr/local/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/scene.dir/model.cpp.o -c /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/src/scene/model.cpp

scene/CMakeFiles/scene.dir/model.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/scene.dir/model.cpp.i"
	cd /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/build/scene && /usr/local/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/src/scene/model.cpp > CMakeFiles/scene.dir/model.cpp.i

scene/CMakeFiles/scene.dir/model.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/scene.dir/model.cpp.s"
	cd /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/build/scene && /usr/local/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/src/scene/model.cpp -o CMakeFiles/scene.dir/model.cpp.s

scene/CMakeFiles/scene.dir/model.cpp.o.requires:
.PHONY : scene/CMakeFiles/scene.dir/model.cpp.o.requires

scene/CMakeFiles/scene.dir/model.cpp.o.provides: scene/CMakeFiles/scene.dir/model.cpp.o.requires
	$(MAKE) -f scene/CMakeFiles/scene.dir/build.make scene/CMakeFiles/scene.dir/model.cpp.o.provides.build
.PHONY : scene/CMakeFiles/scene.dir/model.cpp.o.provides

scene/CMakeFiles/scene.dir/model.cpp.o.provides.build: scene/CMakeFiles/scene.dir/model.cpp.o

scene/CMakeFiles/scene.dir/scene.cpp.o: scene/CMakeFiles/scene.dir/flags.make
scene/CMakeFiles/scene.dir/scene.cpp.o: /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/src/scene/scene.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/build/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object scene/CMakeFiles/scene.dir/scene.cpp.o"
	cd /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/build/scene && /usr/local/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/scene.dir/scene.cpp.o -c /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/src/scene/scene.cpp

scene/CMakeFiles/scene.dir/scene.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/scene.dir/scene.cpp.i"
	cd /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/build/scene && /usr/local/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/src/scene/scene.cpp > CMakeFiles/scene.dir/scene.cpp.i

scene/CMakeFiles/scene.dir/scene.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/scene.dir/scene.cpp.s"
	cd /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/build/scene && /usr/local/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/src/scene/scene.cpp -o CMakeFiles/scene.dir/scene.cpp.s

scene/CMakeFiles/scene.dir/scene.cpp.o.requires:
.PHONY : scene/CMakeFiles/scene.dir/scene.cpp.o.requires

scene/CMakeFiles/scene.dir/scene.cpp.o.provides: scene/CMakeFiles/scene.dir/scene.cpp.o.requires
	$(MAKE) -f scene/CMakeFiles/scene.dir/build.make scene/CMakeFiles/scene.dir/scene.cpp.o.provides.build
.PHONY : scene/CMakeFiles/scene.dir/scene.cpp.o.provides

scene/CMakeFiles/scene.dir/scene.cpp.o.provides.build: scene/CMakeFiles/scene.dir/scene.cpp.o

scene/CMakeFiles/scene.dir/sphere.cpp.o: scene/CMakeFiles/scene.dir/flags.make
scene/CMakeFiles/scene.dir/sphere.cpp.o: /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/src/scene/sphere.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/build/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object scene/CMakeFiles/scene.dir/sphere.cpp.o"
	cd /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/build/scene && /usr/local/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/scene.dir/sphere.cpp.o -c /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/src/scene/sphere.cpp

scene/CMakeFiles/scene.dir/sphere.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/scene.dir/sphere.cpp.i"
	cd /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/build/scene && /usr/local/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/src/scene/sphere.cpp > CMakeFiles/scene.dir/sphere.cpp.i

scene/CMakeFiles/scene.dir/sphere.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/scene.dir/sphere.cpp.s"
	cd /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/build/scene && /usr/local/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/src/scene/sphere.cpp -o CMakeFiles/scene.dir/sphere.cpp.s

scene/CMakeFiles/scene.dir/sphere.cpp.o.requires:
.PHONY : scene/CMakeFiles/scene.dir/sphere.cpp.o.requires

scene/CMakeFiles/scene.dir/sphere.cpp.o.provides: scene/CMakeFiles/scene.dir/sphere.cpp.o.requires
	$(MAKE) -f scene/CMakeFiles/scene.dir/build.make scene/CMakeFiles/scene.dir/sphere.cpp.o.provides.build
.PHONY : scene/CMakeFiles/scene.dir/sphere.cpp.o.provides

scene/CMakeFiles/scene.dir/sphere.cpp.o.provides.build: scene/CMakeFiles/scene.dir/sphere.cpp.o

scene/CMakeFiles/scene.dir/triangle.cpp.o: scene/CMakeFiles/scene.dir/flags.make
scene/CMakeFiles/scene.dir/triangle.cpp.o: /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/src/scene/triangle.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/build/CMakeFiles $(CMAKE_PROGRESS_6)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object scene/CMakeFiles/scene.dir/triangle.cpp.o"
	cd /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/build/scene && /usr/local/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/scene.dir/triangle.cpp.o -c /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/src/scene/triangle.cpp

scene/CMakeFiles/scene.dir/triangle.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/scene.dir/triangle.cpp.i"
	cd /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/build/scene && /usr/local/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/src/scene/triangle.cpp > CMakeFiles/scene.dir/triangle.cpp.i

scene/CMakeFiles/scene.dir/triangle.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/scene.dir/triangle.cpp.s"
	cd /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/build/scene && /usr/local/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/src/scene/triangle.cpp -o CMakeFiles/scene.dir/triangle.cpp.s

scene/CMakeFiles/scene.dir/triangle.cpp.o.requires:
.PHONY : scene/CMakeFiles/scene.dir/triangle.cpp.o.requires

scene/CMakeFiles/scene.dir/triangle.cpp.o.provides: scene/CMakeFiles/scene.dir/triangle.cpp.o.requires
	$(MAKE) -f scene/CMakeFiles/scene.dir/build.make scene/CMakeFiles/scene.dir/triangle.cpp.o.provides.build
.PHONY : scene/CMakeFiles/scene.dir/triangle.cpp.o.provides

scene/CMakeFiles/scene.dir/triangle.cpp.o.provides.build: scene/CMakeFiles/scene.dir/triangle.cpp.o

scene/CMakeFiles/scene.dir/ray.cpp.o: scene/CMakeFiles/scene.dir/flags.make
scene/CMakeFiles/scene.dir/ray.cpp.o: /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/src/scene/ray.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/build/CMakeFiles $(CMAKE_PROGRESS_7)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object scene/CMakeFiles/scene.dir/ray.cpp.o"
	cd /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/build/scene && /usr/local/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/scene.dir/ray.cpp.o -c /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/src/scene/ray.cpp

scene/CMakeFiles/scene.dir/ray.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/scene.dir/ray.cpp.i"
	cd /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/build/scene && /usr/local/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/src/scene/ray.cpp > CMakeFiles/scene.dir/ray.cpp.i

scene/CMakeFiles/scene.dir/ray.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/scene.dir/ray.cpp.s"
	cd /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/build/scene && /usr/local/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/src/scene/ray.cpp -o CMakeFiles/scene.dir/ray.cpp.s

scene/CMakeFiles/scene.dir/ray.cpp.o.requires:
.PHONY : scene/CMakeFiles/scene.dir/ray.cpp.o.requires

scene/CMakeFiles/scene.dir/ray.cpp.o.provides: scene/CMakeFiles/scene.dir/ray.cpp.o.requires
	$(MAKE) -f scene/CMakeFiles/scene.dir/build.make scene/CMakeFiles/scene.dir/ray.cpp.o.provides.build
.PHONY : scene/CMakeFiles/scene.dir/ray.cpp.o.provides

scene/CMakeFiles/scene.dir/ray.cpp.o.provides.build: scene/CMakeFiles/scene.dir/ray.cpp.o

# Object files for target scene
scene_OBJECTS = \
"CMakeFiles/scene.dir/material.cpp.o" \
"CMakeFiles/scene.dir/mesh.cpp.o" \
"CMakeFiles/scene.dir/model.cpp.o" \
"CMakeFiles/scene.dir/scene.cpp.o" \
"CMakeFiles/scene.dir/sphere.cpp.o" \
"CMakeFiles/scene.dir/triangle.cpp.o" \
"CMakeFiles/scene.dir/ray.cpp.o"

# External object files for target scene
scene_EXTERNAL_OBJECTS =

scene/libscene.a: scene/CMakeFiles/scene.dir/material.cpp.o
scene/libscene.a: scene/CMakeFiles/scene.dir/mesh.cpp.o
scene/libscene.a: scene/CMakeFiles/scene.dir/model.cpp.o
scene/libscene.a: scene/CMakeFiles/scene.dir/scene.cpp.o
scene/libscene.a: scene/CMakeFiles/scene.dir/sphere.cpp.o
scene/libscene.a: scene/CMakeFiles/scene.dir/triangle.cpp.o
scene/libscene.a: scene/CMakeFiles/scene.dir/ray.cpp.o
scene/libscene.a: scene/CMakeFiles/scene.dir/build.make
scene/libscene.a: scene/CMakeFiles/scene.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX static library libscene.a"
	cd /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/build/scene && $(CMAKE_COMMAND) -P CMakeFiles/scene.dir/cmake_clean_target.cmake
	cd /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/build/scene && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/scene.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
scene/CMakeFiles/scene.dir/build: scene/libscene.a
.PHONY : scene/CMakeFiles/scene.dir/build

scene/CMakeFiles/scene.dir/requires: scene/CMakeFiles/scene.dir/material.cpp.o.requires
scene/CMakeFiles/scene.dir/requires: scene/CMakeFiles/scene.dir/mesh.cpp.o.requires
scene/CMakeFiles/scene.dir/requires: scene/CMakeFiles/scene.dir/model.cpp.o.requires
scene/CMakeFiles/scene.dir/requires: scene/CMakeFiles/scene.dir/scene.cpp.o.requires
scene/CMakeFiles/scene.dir/requires: scene/CMakeFiles/scene.dir/sphere.cpp.o.requires
scene/CMakeFiles/scene.dir/requires: scene/CMakeFiles/scene.dir/triangle.cpp.o.requires
scene/CMakeFiles/scene.dir/requires: scene/CMakeFiles/scene.dir/ray.cpp.o.requires
.PHONY : scene/CMakeFiles/scene.dir/requires

scene/CMakeFiles/scene.dir/clean:
	cd /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/build/scene && $(CMAKE_COMMAND) -P CMakeFiles/scene.dir/cmake_clean.cmake
.PHONY : scene/CMakeFiles/scene.dir/clean

scene/CMakeFiles/scene.dir/depend:
	cd /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/src /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/src/scene /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/build /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/build/scene /afs/andrew.cmu.edu/usr13/jingxiao/private/15462/p3/build/scene/CMakeFiles/scene.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : scene/CMakeFiles/scene.dir/depend

