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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/nicolas/Rastreador-do-peixe

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/nicolas/Rastreador-do-peixe/build

# Include any dependencies generated for this target.
include vendor/CMakeFiles/vendor.dir/depend.make

# Include the progress variables for this target.
include vendor/CMakeFiles/vendor.dir/progress.make

# Include the compile flags for this target's objects.
include vendor/CMakeFiles/vendor.dir/flags.make

vendor/CMakeFiles/vendor.dir/lib/vendor.cpp.o: vendor/CMakeFiles/vendor.dir/flags.make
vendor/CMakeFiles/vendor.dir/lib/vendor.cpp.o: ../vendor/lib/vendor.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nicolas/Rastreador-do-peixe/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object vendor/CMakeFiles/vendor.dir/lib/vendor.cpp.o"
	cd /home/nicolas/Rastreador-do-peixe/build/vendor && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/vendor.dir/lib/vendor.cpp.o -c /home/nicolas/Rastreador-do-peixe/vendor/lib/vendor.cpp

vendor/CMakeFiles/vendor.dir/lib/vendor.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/vendor.dir/lib/vendor.cpp.i"
	cd /home/nicolas/Rastreador-do-peixe/build/vendor && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nicolas/Rastreador-do-peixe/vendor/lib/vendor.cpp > CMakeFiles/vendor.dir/lib/vendor.cpp.i

vendor/CMakeFiles/vendor.dir/lib/vendor.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/vendor.dir/lib/vendor.cpp.s"
	cd /home/nicolas/Rastreador-do-peixe/build/vendor && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nicolas/Rastreador-do-peixe/vendor/lib/vendor.cpp -o CMakeFiles/vendor.dir/lib/vendor.cpp.s

# Object files for target vendor
vendor_OBJECTS = \
"CMakeFiles/vendor.dir/lib/vendor.cpp.o"

# External object files for target vendor
vendor_EXTERNAL_OBJECTS =

vendor/libvendor.a: vendor/CMakeFiles/vendor.dir/lib/vendor.cpp.o
vendor/libvendor.a: vendor/CMakeFiles/vendor.dir/build.make
vendor/libvendor.a: vendor/CMakeFiles/vendor.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/nicolas/Rastreador-do-peixe/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libvendor.a"
	cd /home/nicolas/Rastreador-do-peixe/build/vendor && $(CMAKE_COMMAND) -P CMakeFiles/vendor.dir/cmake_clean_target.cmake
	cd /home/nicolas/Rastreador-do-peixe/build/vendor && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/vendor.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
vendor/CMakeFiles/vendor.dir/build: vendor/libvendor.a

.PHONY : vendor/CMakeFiles/vendor.dir/build

vendor/CMakeFiles/vendor.dir/clean:
	cd /home/nicolas/Rastreador-do-peixe/build/vendor && $(CMAKE_COMMAND) -P CMakeFiles/vendor.dir/cmake_clean.cmake
.PHONY : vendor/CMakeFiles/vendor.dir/clean

vendor/CMakeFiles/vendor.dir/depend:
	cd /home/nicolas/Rastreador-do-peixe/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/nicolas/Rastreador-do-peixe /home/nicolas/Rastreador-do-peixe/vendor /home/nicolas/Rastreador-do-peixe/build /home/nicolas/Rastreador-do-peixe/build/vendor /home/nicolas/Rastreador-do-peixe/build/vendor/CMakeFiles/vendor.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : vendor/CMakeFiles/vendor.dir/depend

