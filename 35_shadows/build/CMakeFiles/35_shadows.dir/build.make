# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Produce verbose output by default.
VERBOSE = 1

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
CMAKE_SOURCE_DIR = /home/ram/workarea/opengl/advanced_opengl/35_shadows

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ram/workarea/opengl/advanced_opengl/35_shadows/build

# Include any dependencies generated for this target.
include CMakeFiles/35_shadows.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/35_shadows.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/35_shadows.dir/flags.make

CMakeFiles/35_shadows.dir/src/main.cpp.o: CMakeFiles/35_shadows.dir/flags.make
CMakeFiles/35_shadows.dir/src/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ram/workarea/opengl/advanced_opengl/35_shadows/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/35_shadows.dir/src/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/35_shadows.dir/src/main.cpp.o -c /home/ram/workarea/opengl/advanced_opengl/35_shadows/src/main.cpp

CMakeFiles/35_shadows.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/35_shadows.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ram/workarea/opengl/advanced_opengl/35_shadows/src/main.cpp > CMakeFiles/35_shadows.dir/src/main.cpp.i

CMakeFiles/35_shadows.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/35_shadows.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ram/workarea/opengl/advanced_opengl/35_shadows/src/main.cpp -o CMakeFiles/35_shadows.dir/src/main.cpp.s

CMakeFiles/35_shadows.dir/src/main.cpp.o.requires:

.PHONY : CMakeFiles/35_shadows.dir/src/main.cpp.o.requires

CMakeFiles/35_shadows.dir/src/main.cpp.o.provides: CMakeFiles/35_shadows.dir/src/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/35_shadows.dir/build.make CMakeFiles/35_shadows.dir/src/main.cpp.o.provides.build
.PHONY : CMakeFiles/35_shadows.dir/src/main.cpp.o.provides

CMakeFiles/35_shadows.dir/src/main.cpp.o.provides.build: CMakeFiles/35_shadows.dir/src/main.cpp.o


CMakeFiles/35_shadows.dir/include/gamesettings.cpp.o: CMakeFiles/35_shadows.dir/flags.make
CMakeFiles/35_shadows.dir/include/gamesettings.cpp.o: ../include/gamesettings.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ram/workarea/opengl/advanced_opengl/35_shadows/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/35_shadows.dir/include/gamesettings.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/35_shadows.dir/include/gamesettings.cpp.o -c /home/ram/workarea/opengl/advanced_opengl/35_shadows/include/gamesettings.cpp

CMakeFiles/35_shadows.dir/include/gamesettings.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/35_shadows.dir/include/gamesettings.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ram/workarea/opengl/advanced_opengl/35_shadows/include/gamesettings.cpp > CMakeFiles/35_shadows.dir/include/gamesettings.cpp.i

CMakeFiles/35_shadows.dir/include/gamesettings.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/35_shadows.dir/include/gamesettings.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ram/workarea/opengl/advanced_opengl/35_shadows/include/gamesettings.cpp -o CMakeFiles/35_shadows.dir/include/gamesettings.cpp.s

CMakeFiles/35_shadows.dir/include/gamesettings.cpp.o.requires:

.PHONY : CMakeFiles/35_shadows.dir/include/gamesettings.cpp.o.requires

CMakeFiles/35_shadows.dir/include/gamesettings.cpp.o.provides: CMakeFiles/35_shadows.dir/include/gamesettings.cpp.o.requires
	$(MAKE) -f CMakeFiles/35_shadows.dir/build.make CMakeFiles/35_shadows.dir/include/gamesettings.cpp.o.provides.build
.PHONY : CMakeFiles/35_shadows.dir/include/gamesettings.cpp.o.provides

CMakeFiles/35_shadows.dir/include/gamesettings.cpp.o.provides.build: CMakeFiles/35_shadows.dir/include/gamesettings.cpp.o


CMakeFiles/35_shadows.dir/include/shader.cpp.o: CMakeFiles/35_shadows.dir/flags.make
CMakeFiles/35_shadows.dir/include/shader.cpp.o: ../include/shader.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ram/workarea/opengl/advanced_opengl/35_shadows/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/35_shadows.dir/include/shader.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/35_shadows.dir/include/shader.cpp.o -c /home/ram/workarea/opengl/advanced_opengl/35_shadows/include/shader.cpp

CMakeFiles/35_shadows.dir/include/shader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/35_shadows.dir/include/shader.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ram/workarea/opengl/advanced_opengl/35_shadows/include/shader.cpp > CMakeFiles/35_shadows.dir/include/shader.cpp.i

CMakeFiles/35_shadows.dir/include/shader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/35_shadows.dir/include/shader.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ram/workarea/opengl/advanced_opengl/35_shadows/include/shader.cpp -o CMakeFiles/35_shadows.dir/include/shader.cpp.s

CMakeFiles/35_shadows.dir/include/shader.cpp.o.requires:

.PHONY : CMakeFiles/35_shadows.dir/include/shader.cpp.o.requires

CMakeFiles/35_shadows.dir/include/shader.cpp.o.provides: CMakeFiles/35_shadows.dir/include/shader.cpp.o.requires
	$(MAKE) -f CMakeFiles/35_shadows.dir/build.make CMakeFiles/35_shadows.dir/include/shader.cpp.o.provides.build
.PHONY : CMakeFiles/35_shadows.dir/include/shader.cpp.o.provides

CMakeFiles/35_shadows.dir/include/shader.cpp.o.provides.build: CMakeFiles/35_shadows.dir/include/shader.cpp.o


# Object files for target 35_shadows
35_shadows_OBJECTS = \
"CMakeFiles/35_shadows.dir/src/main.cpp.o" \
"CMakeFiles/35_shadows.dir/include/gamesettings.cpp.o" \
"CMakeFiles/35_shadows.dir/include/shader.cpp.o"

# External object files for target 35_shadows
35_shadows_EXTERNAL_OBJECTS =

../bin/35_shadows: CMakeFiles/35_shadows.dir/src/main.cpp.o
../bin/35_shadows: CMakeFiles/35_shadows.dir/include/gamesettings.cpp.o
../bin/35_shadows: CMakeFiles/35_shadows.dir/include/shader.cpp.o
../bin/35_shadows: CMakeFiles/35_shadows.dir/build.make
../bin/35_shadows: CMakeFiles/35_shadows.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ram/workarea/opengl/advanced_opengl/35_shadows/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable ../bin/35_shadows"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/35_shadows.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/35_shadows.dir/build: ../bin/35_shadows

.PHONY : CMakeFiles/35_shadows.dir/build

CMakeFiles/35_shadows.dir/requires: CMakeFiles/35_shadows.dir/src/main.cpp.o.requires
CMakeFiles/35_shadows.dir/requires: CMakeFiles/35_shadows.dir/include/gamesettings.cpp.o.requires
CMakeFiles/35_shadows.dir/requires: CMakeFiles/35_shadows.dir/include/shader.cpp.o.requires

.PHONY : CMakeFiles/35_shadows.dir/requires

CMakeFiles/35_shadows.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/35_shadows.dir/cmake_clean.cmake
.PHONY : CMakeFiles/35_shadows.dir/clean

CMakeFiles/35_shadows.dir/depend:
	cd /home/ram/workarea/opengl/advanced_opengl/35_shadows/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ram/workarea/opengl/advanced_opengl/35_shadows /home/ram/workarea/opengl/advanced_opengl/35_shadows /home/ram/workarea/opengl/advanced_opengl/35_shadows/build /home/ram/workarea/opengl/advanced_opengl/35_shadows/build /home/ram/workarea/opengl/advanced_opengl/35_shadows/build/CMakeFiles/35_shadows.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/35_shadows.dir/depend
