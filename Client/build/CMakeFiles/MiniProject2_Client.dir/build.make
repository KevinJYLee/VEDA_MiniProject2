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
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.30.2/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.30.2/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/junyoung/Proj1/MiniProj2/Client

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/junyoung/Proj1/MiniProj2/Client/build

# Include any dependencies generated for this target.
include CMakeFiles/MiniProject2_Client.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/MiniProject2_Client.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/MiniProject2_Client.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/MiniProject2_Client.dir/flags.make

CMakeFiles/MiniProject2_Client.dir/src/myClient.c.o: CMakeFiles/MiniProject2_Client.dir/flags.make
CMakeFiles/MiniProject2_Client.dir/src/myClient.c.o: /Users/junyoung/Proj1/MiniProj2/Client/src/myClient.c
CMakeFiles/MiniProject2_Client.dir/src/myClient.c.o: CMakeFiles/MiniProject2_Client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/junyoung/Proj1/MiniProj2/Client/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/MiniProject2_Client.dir/src/myClient.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/MiniProject2_Client.dir/src/myClient.c.o -MF CMakeFiles/MiniProject2_Client.dir/src/myClient.c.o.d -o CMakeFiles/MiniProject2_Client.dir/src/myClient.c.o -c /Users/junyoung/Proj1/MiniProj2/Client/src/myClient.c

CMakeFiles/MiniProject2_Client.dir/src/myClient.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/MiniProject2_Client.dir/src/myClient.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/junyoung/Proj1/MiniProj2/Client/src/myClient.c > CMakeFiles/MiniProject2_Client.dir/src/myClient.c.i

CMakeFiles/MiniProject2_Client.dir/src/myClient.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/MiniProject2_Client.dir/src/myClient.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/junyoung/Proj1/MiniProj2/Client/src/myClient.c -o CMakeFiles/MiniProject2_Client.dir/src/myClient.c.s

# Object files for target MiniProject2_Client
MiniProject2_Client_OBJECTS = \
"CMakeFiles/MiniProject2_Client.dir/src/myClient.c.o"

# External object files for target MiniProject2_Client
MiniProject2_Client_EXTERNAL_OBJECTS =

MiniProject2_Client: CMakeFiles/MiniProject2_Client.dir/src/myClient.c.o
MiniProject2_Client: CMakeFiles/MiniProject2_Client.dir/build.make
MiniProject2_Client: CMakeFiles/MiniProject2_Client.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/junyoung/Proj1/MiniProj2/Client/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable MiniProject2_Client"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/MiniProject2_Client.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/MiniProject2_Client.dir/build: MiniProject2_Client
.PHONY : CMakeFiles/MiniProject2_Client.dir/build

CMakeFiles/MiniProject2_Client.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/MiniProject2_Client.dir/cmake_clean.cmake
.PHONY : CMakeFiles/MiniProject2_Client.dir/clean

CMakeFiles/MiniProject2_Client.dir/depend:
	cd /Users/junyoung/Proj1/MiniProj2/Client/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/junyoung/Proj1/MiniProj2/Client /Users/junyoung/Proj1/MiniProj2/Client /Users/junyoung/Proj1/MiniProj2/Client/build /Users/junyoung/Proj1/MiniProj2/Client/build /Users/junyoung/Proj1/MiniProj2/Client/build/CMakeFiles/MiniProject2_Client.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/MiniProject2_Client.dir/depend

