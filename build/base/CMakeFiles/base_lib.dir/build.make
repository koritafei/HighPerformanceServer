# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.22.2/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.22.2/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/quwei5/Desktop/code/HighPerformanceServer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/quwei5/Desktop/code/HighPerformanceServer/build

# Include any dependencies generated for this target.
include base/CMakeFiles/base_lib.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include base/CMakeFiles/base_lib.dir/compiler_depend.make

# Include the progress variables for this target.
include base/CMakeFiles/base_lib.dir/progress.make

# Include the compile flags for this target's objects.
include base/CMakeFiles/base_lib.dir/flags.make

base/CMakeFiles/base_lib.dir/thread_pool/ctrip_thread_pool.cc.o: base/CMakeFiles/base_lib.dir/flags.make
base/CMakeFiles/base_lib.dir/thread_pool/ctrip_thread_pool.cc.o: ../base/thread_pool/ctrip_thread_pool.cc
base/CMakeFiles/base_lib.dir/thread_pool/ctrip_thread_pool.cc.o: base/CMakeFiles/base_lib.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/quwei5/Desktop/code/HighPerformanceServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object base/CMakeFiles/base_lib.dir/thread_pool/ctrip_thread_pool.cc.o"
	cd /Users/quwei5/Desktop/code/HighPerformanceServer/build/base && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT base/CMakeFiles/base_lib.dir/thread_pool/ctrip_thread_pool.cc.o -MF CMakeFiles/base_lib.dir/thread_pool/ctrip_thread_pool.cc.o.d -o CMakeFiles/base_lib.dir/thread_pool/ctrip_thread_pool.cc.o -c /Users/quwei5/Desktop/code/HighPerformanceServer/base/thread_pool/ctrip_thread_pool.cc

base/CMakeFiles/base_lib.dir/thread_pool/ctrip_thread_pool.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/base_lib.dir/thread_pool/ctrip_thread_pool.cc.i"
	cd /Users/quwei5/Desktop/code/HighPerformanceServer/build/base && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/quwei5/Desktop/code/HighPerformanceServer/base/thread_pool/ctrip_thread_pool.cc > CMakeFiles/base_lib.dir/thread_pool/ctrip_thread_pool.cc.i

base/CMakeFiles/base_lib.dir/thread_pool/ctrip_thread_pool.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/base_lib.dir/thread_pool/ctrip_thread_pool.cc.s"
	cd /Users/quwei5/Desktop/code/HighPerformanceServer/build/base && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/quwei5/Desktop/code/HighPerformanceServer/base/thread_pool/ctrip_thread_pool.cc -o CMakeFiles/base_lib.dir/thread_pool/ctrip_thread_pool.cc.s

# Object files for target base_lib
base_lib_OBJECTS = \
"CMakeFiles/base_lib.dir/thread_pool/ctrip_thread_pool.cc.o"

# External object files for target base_lib
base_lib_EXTERNAL_OBJECTS =

../lib/libbase_lib.a: base/CMakeFiles/base_lib.dir/thread_pool/ctrip_thread_pool.cc.o
../lib/libbase_lib.a: base/CMakeFiles/base_lib.dir/build.make
../lib/libbase_lib.a: base/CMakeFiles/base_lib.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/quwei5/Desktop/code/HighPerformanceServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library ../../lib/libbase_lib.a"
	cd /Users/quwei5/Desktop/code/HighPerformanceServer/build/base && $(CMAKE_COMMAND) -P CMakeFiles/base_lib.dir/cmake_clean_target.cmake
	cd /Users/quwei5/Desktop/code/HighPerformanceServer/build/base && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/base_lib.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
base/CMakeFiles/base_lib.dir/build: ../lib/libbase_lib.a
.PHONY : base/CMakeFiles/base_lib.dir/build

base/CMakeFiles/base_lib.dir/clean:
	cd /Users/quwei5/Desktop/code/HighPerformanceServer/build/base && $(CMAKE_COMMAND) -P CMakeFiles/base_lib.dir/cmake_clean.cmake
.PHONY : base/CMakeFiles/base_lib.dir/clean

base/CMakeFiles/base_lib.dir/depend:
	cd /Users/quwei5/Desktop/code/HighPerformanceServer/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/quwei5/Desktop/code/HighPerformanceServer /Users/quwei5/Desktop/code/HighPerformanceServer/base /Users/quwei5/Desktop/code/HighPerformanceServer/build /Users/quwei5/Desktop/code/HighPerformanceServer/build/base /Users/quwei5/Desktop/code/HighPerformanceServer/build/base/CMakeFiles/base_lib.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : base/CMakeFiles/base_lib.dir/depend
