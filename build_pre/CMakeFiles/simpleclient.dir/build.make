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
include CMakeFiles/simpleclient.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/simpleclient.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/simpleclient.dir/flags.make

CMakeFiles/simpleclient.dir/src/simpleclient.cpp.o: CMakeFiles/simpleclient.dir/flags.make
CMakeFiles/simpleclient.dir/src/simpleclient.cpp.o: ../src/simpleclient.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cc/keren/clockwork/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/simpleclient.dir/src/simpleclient.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/simpleclient.dir/src/simpleclient.cpp.o -c /home/cc/keren/clockwork/src/simpleclient.cpp

CMakeFiles/simpleclient.dir/src/simpleclient.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/simpleclient.dir/src/simpleclient.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cc/keren/clockwork/src/simpleclient.cpp > CMakeFiles/simpleclient.dir/src/simpleclient.cpp.i

CMakeFiles/simpleclient.dir/src/simpleclient.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/simpleclient.dir/src/simpleclient.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cc/keren/clockwork/src/simpleclient.cpp -o CMakeFiles/simpleclient.dir/src/simpleclient.cpp.s

# Object files for target simpleclient
simpleclient_OBJECTS = \
"CMakeFiles/simpleclient.dir/src/simpleclient.cpp.o"

# External object files for target simpleclient
simpleclient_EXTERNAL_OBJECTS =

simpleclient: CMakeFiles/simpleclient.dir/src/simpleclient.cpp.o
simpleclient: CMakeFiles/simpleclient.dir/build.make
simpleclient: libclockwork.a
simpleclient: libclockwork_proto.a
simpleclient: /usr/lib/x86_64-linux-gnu/libboost_system.so.1.71.0
simpleclient: /usr/lib/x86_64-linux-gnu/libboost_filesystem.so.1.71.0
simpleclient: /usr/local/lib/libprotobuf.so
simpleclient: CMakeFiles/simpleclient.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/cc/keren/clockwork/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable simpleclient"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/simpleclient.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/simpleclient.dir/build: simpleclient

.PHONY : CMakeFiles/simpleclient.dir/build

CMakeFiles/simpleclient.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/simpleclient.dir/cmake_clean.cmake
.PHONY : CMakeFiles/simpleclient.dir/clean

CMakeFiles/simpleclient.dir/depend:
	cd /home/cc/keren/clockwork/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/cc/keren/clockwork /home/cc/keren/clockwork /home/cc/keren/clockwork/build /home/cc/keren/clockwork/build /home/cc/keren/clockwork/build/CMakeFiles/simpleclient.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/simpleclient.dir/depend

