# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/cc/keren/clockwork

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/cc/keren/clockwork/build

# Include any dependencies generated for this target.
include CMakeFiles/inflate.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/inflate.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/inflate.dir/flags.make

CMakeFiles/inflate.dir/src/inflate.cpp.o: CMakeFiles/inflate.dir/flags.make
CMakeFiles/inflate.dir/src/inflate.cpp.o: ../src/inflate.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cc/keren/clockwork/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/inflate.dir/src/inflate.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/inflate.dir/src/inflate.cpp.o -c /home/cc/keren/clockwork/src/inflate.cpp

CMakeFiles/inflate.dir/src/inflate.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/inflate.dir/src/inflate.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cc/keren/clockwork/src/inflate.cpp > CMakeFiles/inflate.dir/src/inflate.cpp.i

CMakeFiles/inflate.dir/src/inflate.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/inflate.dir/src/inflate.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cc/keren/clockwork/src/inflate.cpp -o CMakeFiles/inflate.dir/src/inflate.cpp.s

# Object files for target inflate
inflate_OBJECTS = \
"CMakeFiles/inflate.dir/src/inflate.cpp.o"

# External object files for target inflate
inflate_EXTERNAL_OBJECTS =

inflate: CMakeFiles/inflate.dir/src/inflate.cpp.o
inflate: CMakeFiles/inflate.dir/build.make
inflate: libclockwork.a
inflate: libclockwork_proto.a
inflate: /usr/lib/x86_64-linux-gnu/libboost_system.so.1.71.0
inflate: /usr/lib/x86_64-linux-gnu/libboost_filesystem.so.1.71.0
inflate: /usr/local/lib/libprotobuf.so
inflate: CMakeFiles/inflate.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/cc/keren/clockwork/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable inflate"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/inflate.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/inflate.dir/build: inflate

.PHONY : CMakeFiles/inflate.dir/build

CMakeFiles/inflate.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/inflate.dir/cmake_clean.cmake
.PHONY : CMakeFiles/inflate.dir/clean

CMakeFiles/inflate.dir/depend:
	cd /home/cc/keren/clockwork/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/cc/keren/clockwork /home/cc/keren/clockwork /home/cc/keren/clockwork/build /home/cc/keren/clockwork/build /home/cc/keren/clockwork/build/CMakeFiles/inflate.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/inflate.dir/depend

