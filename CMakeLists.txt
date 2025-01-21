# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name
project(gbemu)

# Set the C++ Standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add include directories (where the .h files are located)
include_directories(${CMAKE_SOURCE_DIR}/include)

# Find all .cpp files in src directory
file(GLOB SRC_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp)

# Add the executable
add_executable(gbemu main.cpp ${SRC_FILES})

# If you want to separate testing or additional directories in the future, you can add those here.
