# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.31

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "C:\Users\papej\Desktop\rasyteryzer 2 xd"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "C:\Users\papej\Desktop\rasyteryzer 2 xd\build"

# Include any dependencies generated for this target.
include CMakeFiles/rasteryzer.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/rasteryzer.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/rasteryzer.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/rasteryzer.dir/flags.make

CMakeFiles/rasteryzer.dir/codegen:
.PHONY : CMakeFiles/rasteryzer.dir/codegen

CMakeFiles/rasteryzer.dir/camera.c.obj: CMakeFiles/rasteryzer.dir/flags.make
CMakeFiles/rasteryzer.dir/camera.c.obj: C:/Users/papej/Desktop/rasyteryzer\ 2\ xd/camera.c
CMakeFiles/rasteryzer.dir/camera.c.obj: CMakeFiles/rasteryzer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="C:\Users\papej\Desktop\rasyteryzer 2 xd\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/rasteryzer.dir/camera.c.obj"
	C:\msys64\ucrt64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/rasteryzer.dir/camera.c.obj -MF CMakeFiles\rasteryzer.dir\camera.c.obj.d -o CMakeFiles\rasteryzer.dir\camera.c.obj -c "C:\Users\papej\Desktop\rasyteryzer 2 xd\camera.c"

CMakeFiles/rasteryzer.dir/camera.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/rasteryzer.dir/camera.c.i"
	C:\msys64\ucrt64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "C:\Users\papej\Desktop\rasyteryzer 2 xd\camera.c" > CMakeFiles\rasteryzer.dir\camera.c.i

CMakeFiles/rasteryzer.dir/camera.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/rasteryzer.dir/camera.c.s"
	C:\msys64\ucrt64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "C:\Users\papej\Desktop\rasyteryzer 2 xd\camera.c" -o CMakeFiles\rasteryzer.dir\camera.c.s

CMakeFiles/rasteryzer.dir/draw.c.obj: CMakeFiles/rasteryzer.dir/flags.make
CMakeFiles/rasteryzer.dir/draw.c.obj: C:/Users/papej/Desktop/rasyteryzer\ 2\ xd/draw.c
CMakeFiles/rasteryzer.dir/draw.c.obj: CMakeFiles/rasteryzer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="C:\Users\papej\Desktop\rasyteryzer 2 xd\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/rasteryzer.dir/draw.c.obj"
	C:\msys64\ucrt64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/rasteryzer.dir/draw.c.obj -MF CMakeFiles\rasteryzer.dir\draw.c.obj.d -o CMakeFiles\rasteryzer.dir\draw.c.obj -c "C:\Users\papej\Desktop\rasyteryzer 2 xd\draw.c"

CMakeFiles/rasteryzer.dir/draw.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/rasteryzer.dir/draw.c.i"
	C:\msys64\ucrt64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "C:\Users\papej\Desktop\rasyteryzer 2 xd\draw.c" > CMakeFiles\rasteryzer.dir\draw.c.i

CMakeFiles/rasteryzer.dir/draw.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/rasteryzer.dir/draw.c.s"
	C:\msys64\ucrt64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "C:\Users\papej\Desktop\rasyteryzer 2 xd\draw.c" -o CMakeFiles\rasteryzer.dir\draw.c.s

# Object files for target rasteryzer
rasteryzer_OBJECTS = \
"CMakeFiles/rasteryzer.dir/camera.c.obj" \
"CMakeFiles/rasteryzer.dir/draw.c.obj"

# External object files for target rasteryzer
rasteryzer_EXTERNAL_OBJECTS =

rasteryzer.exe: CMakeFiles/rasteryzer.dir/camera.c.obj
rasteryzer.exe: CMakeFiles/rasteryzer.dir/draw.c.obj
rasteryzer.exe: CMakeFiles/rasteryzer.dir/build.make
rasteryzer.exe: CMakeFiles/rasteryzer.dir/linkLibs.rsp
rasteryzer.exe: CMakeFiles/rasteryzer.dir/objects1.rsp
rasteryzer.exe: CMakeFiles/rasteryzer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir="C:\Users\papej\Desktop\rasyteryzer 2 xd\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable rasteryzer.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\rasteryzer.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/rasteryzer.dir/build: rasteryzer.exe
.PHONY : CMakeFiles/rasteryzer.dir/build

CMakeFiles/rasteryzer.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\rasteryzer.dir\cmake_clean.cmake
.PHONY : CMakeFiles/rasteryzer.dir/clean

CMakeFiles/rasteryzer.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" "C:\Users\papej\Desktop\rasyteryzer 2 xd" "C:\Users\papej\Desktop\rasyteryzer 2 xd" "C:\Users\papej\Desktop\rasyteryzer 2 xd\build" "C:\Users\papej\Desktop\rasyteryzer 2 xd\build" "C:\Users\papej\Desktop\rasyteryzer 2 xd\build\CMakeFiles\rasteryzer.dir\DependInfo.cmake" "--color=$(COLOR)"
.PHONY : CMakeFiles/rasteryzer.dir/depend

