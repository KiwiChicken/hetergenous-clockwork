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
include CMakeFiles/netcontroller.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/netcontroller.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/netcontroller.dir/flags.make

CMakeFiles/netcontroller.dir/src/network/netcontroller.cpp.o: CMakeFiles/netcontroller.dir/flags.make
CMakeFiles/netcontroller.dir/src/network/netcontroller.cpp.o: ../src/network/netcontroller.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cc/keren/clockwork/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/netcontroller.dir/src/network/netcontroller.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/netcontroller.dir/src/network/netcontroller.cpp.o -c /home/cc/keren/clockwork/src/network/netcontroller.cpp

CMakeFiles/netcontroller.dir/src/network/netcontroller.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/netcontroller.dir/src/network/netcontroller.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cc/keren/clockwork/src/network/netcontroller.cpp > CMakeFiles/netcontroller.dir/src/network/netcontroller.cpp.i

CMakeFiles/netcontroller.dir/src/network/netcontroller.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/netcontroller.dir/src/network/netcontroller.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cc/keren/clockwork/src/network/netcontroller.cpp -o CMakeFiles/netcontroller.dir/src/network/netcontroller.cpp.s

# Object files for target netcontroller
netcontroller_OBJECTS = \
"CMakeFiles/netcontroller.dir/src/network/netcontroller.cpp.o"

# External object files for target netcontroller
netcontroller_EXTERNAL_OBJECTS =

netcontroller: CMakeFiles/netcontroller.dir/src/network/netcontroller.cpp.o
netcontroller: CMakeFiles/netcontroller.dir/build.make
netcontroller: libclockwork.a
netcontroller: libclockwork_proto.a
netcontroller: /usr/lib/x86_64-linux-gnu/libboost_system.so.1.71.0
netcontroller: /usr/lib/x86_64-linux-gnu/libboost_filesystem.so.1.71.0
netcontroller: /usr/local/lib/libprotobuf.so
netcontroller: CMakeFiles/netcontroller.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/cc/keren/clockwork/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable netcontroller"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/netcontroller.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/netcontroller.dir/build: netcontroller

.PHONY : CMakeFiles/netcontroller.dir/build

CMakeFiles/netcontroller.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/netcontroller.dir/cmake_clean.cmake
.PHONY : CMakeFiles/netcontroller.dir/clean

CMakeFiles/netcontroller.dir/depend:
	cd /home/cc/keren/clockwork/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/cc/keren/clockwork /home/cc/keren/clockwork /home/cc/keren/clockwork/build /home/cc/keren/clockwork/build /home/cc/keren/clockwork/build/CMakeFiles/netcontroller.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/netcontroller.dir/depend

