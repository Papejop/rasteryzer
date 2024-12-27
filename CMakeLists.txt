cmake_minimum_required(VERSION 3.10)

# Set the project name
project(rasteryzer)

# Specify the C standard
set(CMAKE_C_STANDARD 99)

# Add the executable
add_executable(rasteryzer camera.c draw.c)

# Link the math library
target_link_libraries(rasteryzer m)

# Set the build type to Debug by default if not specified
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

# Add debug flags
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")

# Optionally, you can add more configurations for other build types
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3")