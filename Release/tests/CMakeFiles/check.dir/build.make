# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canoncical targets will work.
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

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/mion/s/143/kkamins1/Desktop/Mapy

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/mion/s/143/kkamins1/Desktop/Mapy/Release

# Utility rule file for check.

tests/CMakeFiles/check: tests/aisdiMapsTests
	cd /home/mion/s/143/kkamins1/Desktop/Mapy/Release/tests && /usr/bin/ctest --force-new-ctest-process --output-on-failure

check: tests/CMakeFiles/check
check: tests/CMakeFiles/check.dir/build.make
.PHONY : check

# Rule to build all files generated by this target.
tests/CMakeFiles/check.dir/build: check
.PHONY : tests/CMakeFiles/check.dir/build

tests/CMakeFiles/check.dir/clean:
	cd /home/mion/s/143/kkamins1/Desktop/Mapy/Release/tests && $(CMAKE_COMMAND) -P CMakeFiles/check.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/check.dir/clean

tests/CMakeFiles/check.dir/depend:
	cd /home/mion/s/143/kkamins1/Desktop/Mapy/Release && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mion/s/143/kkamins1/Desktop/Mapy /home/mion/s/143/kkamins1/Desktop/Mapy/tests /home/mion/s/143/kkamins1/Desktop/Mapy/Release /home/mion/s/143/kkamins1/Desktop/Mapy/Release/tests /home/mion/s/143/kkamins1/Desktop/Mapy/Release/tests/CMakeFiles/check.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/CMakeFiles/check.dir/depend

