# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.11

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /webrtc/test_linux/linux_test/test_flock

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /webrtc/test_linux/linux_test/test_flock/build

# Include any dependencies generated for this target.
include CMakeFiles/teset_flock.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/teset_flock.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/teset_flock.dir/flags.make

CMakeFiles/teset_flock.dir/main.cpp.o: CMakeFiles/teset_flock.dir/flags.make
CMakeFiles/teset_flock.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/webrtc/test_linux/linux_test/test_flock/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/teset_flock.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/teset_flock.dir/main.cpp.o -c /webrtc/test_linux/linux_test/test_flock/main.cpp

CMakeFiles/teset_flock.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/teset_flock.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /webrtc/test_linux/linux_test/test_flock/main.cpp > CMakeFiles/teset_flock.dir/main.cpp.i

CMakeFiles/teset_flock.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/teset_flock.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /webrtc/test_linux/linux_test/test_flock/main.cpp -o CMakeFiles/teset_flock.dir/main.cpp.s

CMakeFiles/teset_flock.dir/file_locker.cpp.o: CMakeFiles/teset_flock.dir/flags.make
CMakeFiles/teset_flock.dir/file_locker.cpp.o: ../file_locker.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/webrtc/test_linux/linux_test/test_flock/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/teset_flock.dir/file_locker.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/teset_flock.dir/file_locker.cpp.o -c /webrtc/test_linux/linux_test/test_flock/file_locker.cpp

CMakeFiles/teset_flock.dir/file_locker.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/teset_flock.dir/file_locker.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /webrtc/test_linux/linux_test/test_flock/file_locker.cpp > CMakeFiles/teset_flock.dir/file_locker.cpp.i

CMakeFiles/teset_flock.dir/file_locker.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/teset_flock.dir/file_locker.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /webrtc/test_linux/linux_test/test_flock/file_locker.cpp -o CMakeFiles/teset_flock.dir/file_locker.cpp.s

# Object files for target teset_flock
teset_flock_OBJECTS = \
"CMakeFiles/teset_flock.dir/main.cpp.o" \
"CMakeFiles/teset_flock.dir/file_locker.cpp.o"

# External object files for target teset_flock
teset_flock_EXTERNAL_OBJECTS =

teset_flock: CMakeFiles/teset_flock.dir/main.cpp.o
teset_flock: CMakeFiles/teset_flock.dir/file_locker.cpp.o
teset_flock: CMakeFiles/teset_flock.dir/build.make
teset_flock: CMakeFiles/teset_flock.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/webrtc/test_linux/linux_test/test_flock/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable teset_flock"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/teset_flock.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/teset_flock.dir/build: teset_flock

.PHONY : CMakeFiles/teset_flock.dir/build

CMakeFiles/teset_flock.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/teset_flock.dir/cmake_clean.cmake
.PHONY : CMakeFiles/teset_flock.dir/clean

CMakeFiles/teset_flock.dir/depend:
	cd /webrtc/test_linux/linux_test/test_flock/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /webrtc/test_linux/linux_test/test_flock /webrtc/test_linux/linux_test/test_flock /webrtc/test_linux/linux_test/test_flock/build /webrtc/test_linux/linux_test/test_flock/build /webrtc/test_linux/linux_test/test_flock/build/CMakeFiles/teset_flock.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/teset_flock.dir/depend

