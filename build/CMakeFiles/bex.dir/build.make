# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.30

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/rjajko/Code/Bex

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/rjajko/Code/Bex/build

# Include any dependencies generated for this target.
include CMakeFiles/bex.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/bex.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/bex.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/bex.dir/flags.make

CMakeFiles/bex.dir/Bex.cpp.o: CMakeFiles/bex.dir/flags.make
CMakeFiles/bex.dir/Bex.cpp.o: /home/rjajko/Code/Bex/Bex.cpp
CMakeFiles/bex.dir/Bex.cpp.o: CMakeFiles/bex.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/rjajko/Code/Bex/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/bex.dir/Bex.cpp.o"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/bex.dir/Bex.cpp.o -MF CMakeFiles/bex.dir/Bex.cpp.o.d -o CMakeFiles/bex.dir/Bex.cpp.o -c /home/rjajko/Code/Bex/Bex.cpp

CMakeFiles/bex.dir/Bex.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/bex.dir/Bex.cpp.i"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/rjajko/Code/Bex/Bex.cpp > CMakeFiles/bex.dir/Bex.cpp.i

CMakeFiles/bex.dir/Bex.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/bex.dir/Bex.cpp.s"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/rjajko/Code/Bex/Bex.cpp -o CMakeFiles/bex.dir/Bex.cpp.s

CMakeFiles/bex.dir/BexInterpreter.cpp.o: CMakeFiles/bex.dir/flags.make
CMakeFiles/bex.dir/BexInterpreter.cpp.o: /home/rjajko/Code/Bex/BexInterpreter.cpp
CMakeFiles/bex.dir/BexInterpreter.cpp.o: CMakeFiles/bex.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/rjajko/Code/Bex/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/bex.dir/BexInterpreter.cpp.o"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/bex.dir/BexInterpreter.cpp.o -MF CMakeFiles/bex.dir/BexInterpreter.cpp.o.d -o CMakeFiles/bex.dir/BexInterpreter.cpp.o -c /home/rjajko/Code/Bex/BexInterpreter.cpp

CMakeFiles/bex.dir/BexInterpreter.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/bex.dir/BexInterpreter.cpp.i"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/rjajko/Code/Bex/BexInterpreter.cpp > CMakeFiles/bex.dir/BexInterpreter.cpp.i

CMakeFiles/bex.dir/BexInterpreter.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/bex.dir/BexInterpreter.cpp.s"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/rjajko/Code/Bex/BexInterpreter.cpp -o CMakeFiles/bex.dir/BexInterpreter.cpp.s

CMakeFiles/bex.dir/Token.cpp.o: CMakeFiles/bex.dir/flags.make
CMakeFiles/bex.dir/Token.cpp.o: /home/rjajko/Code/Bex/Token.cpp
CMakeFiles/bex.dir/Token.cpp.o: CMakeFiles/bex.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/rjajko/Code/Bex/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/bex.dir/Token.cpp.o"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/bex.dir/Token.cpp.o -MF CMakeFiles/bex.dir/Token.cpp.o.d -o CMakeFiles/bex.dir/Token.cpp.o -c /home/rjajko/Code/Bex/Token.cpp

CMakeFiles/bex.dir/Token.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/bex.dir/Token.cpp.i"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/rjajko/Code/Bex/Token.cpp > CMakeFiles/bex.dir/Token.cpp.i

CMakeFiles/bex.dir/Token.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/bex.dir/Token.cpp.s"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/rjajko/Code/Bex/Token.cpp -o CMakeFiles/bex.dir/Token.cpp.s

# Object files for target bex
bex_OBJECTS = \
"CMakeFiles/bex.dir/Bex.cpp.o" \
"CMakeFiles/bex.dir/BexInterpreter.cpp.o" \
"CMakeFiles/bex.dir/Token.cpp.o"

# External object files for target bex
bex_EXTERNAL_OBJECTS =

bex: CMakeFiles/bex.dir/Bex.cpp.o
bex: CMakeFiles/bex.dir/BexInterpreter.cpp.o
bex: CMakeFiles/bex.dir/Token.cpp.o
bex: CMakeFiles/bex.dir/build.make
bex: CMakeFiles/bex.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/rjajko/Code/Bex/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable bex"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/bex.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/bex.dir/build: bex
.PHONY : CMakeFiles/bex.dir/build

CMakeFiles/bex.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/bex.dir/cmake_clean.cmake
.PHONY : CMakeFiles/bex.dir/clean

CMakeFiles/bex.dir/depend:
	cd /home/rjajko/Code/Bex/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/rjajko/Code/Bex /home/rjajko/Code/Bex /home/rjajko/Code/Bex/build /home/rjajko/Code/Bex/build /home/rjajko/Code/Bex/build/CMakeFiles/bex.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/bex.dir/depend

