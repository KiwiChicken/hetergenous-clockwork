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
include CMakeFiles/clockwork_proto.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/clockwork_proto.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/clockwork_proto.dir/flags.make

clockwork.pb.h: ../proto/clockwork.proto
clockwork.pb.h: /usr/local/bin/protoc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/cc/keren/clockwork/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Running cpp protocol buffer compiler on proto/clockwork.proto"
	/usr/local/bin/protoc --cpp_out /home/cc/keren/clockwork/build -I /home/cc/keren/clockwork/proto /home/cc/keren/clockwork/proto/clockwork.proto

clockwork.pb.cc: clockwork.pb.h
	@$(CMAKE_COMMAND) -E touch_nocreate clockwork.pb.cc

CMakeFiles/clockwork_proto.dir/clockwork.pb.cc.o: CMakeFiles/clockwork_proto.dir/flags.make
CMakeFiles/clockwork_proto.dir/clockwork.pb.cc.o: clockwork.pb.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cc/keren/clockwork/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/clockwork_proto.dir/clockwork.pb.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/clockwork_proto.dir/clockwork.pb.cc.o -c /home/cc/keren/clockwork/build/clockwork.pb.cc

CMakeFiles/clockwork_proto.dir/clockwork.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/clockwork_proto.dir/clockwork.pb.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cc/keren/clockwork/build/clockwork.pb.cc > CMakeFiles/clockwork_proto.dir/clockwork.pb.cc.i

CMakeFiles/clockwork_proto.dir/clockwork.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/clockwork_proto.dir/clockwork.pb.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cc/keren/clockwork/build/clockwork.pb.cc -o CMakeFiles/clockwork_proto.dir/clockwork.pb.cc.s

# Object files for target clockwork_proto
clockwork_proto_OBJECTS = \
"CMakeFiles/clockwork_proto.dir/clockwork.pb.cc.o"

# External object files for target clockwork_proto
clockwork_proto_EXTERNAL_OBJECTS =

libclockwork_proto.a: CMakeFiles/clockwork_proto.dir/clockwork.pb.cc.o
libclockwork_proto.a: CMakeFiles/clockwork_proto.dir/build.make
libclockwork_proto.a: CMakeFiles/clockwork_proto.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/cc/keren/clockwork/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX static library libclockwork_proto.a"
	$(CMAKE_COMMAND) -P CMakeFiles/clockwork_proto.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/clockwork_proto.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/clockwork_proto.dir/build: libclockwork_proto.a

.PHONY : CMakeFiles/clockwork_proto.dir/build

CMakeFiles/clockwork_proto.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/clockwork_proto.dir/cmake_clean.cmake
.PHONY : CMakeFiles/clockwork_proto.dir/clean

CMakeFiles/clockwork_proto.dir/depend: clockwork.pb.h
CMakeFiles/clockwork_proto.dir/depend: clockwork.pb.cc
	cd /home/cc/keren/clockwork/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/cc/keren/clockwork /home/cc/keren/clockwork /home/cc/keren/clockwork/build /home/cc/keren/clockwork/build /home/cc/keren/clockwork/build/CMakeFiles/clockwork_proto.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/clockwork_proto.dir/depend

