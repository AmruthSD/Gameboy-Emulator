# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name
project(gbemu)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 11)

# Add all the source files (search for all .cpp files in the current directory)
file(GLOB SOURCES *.cpp)

# Create executable from the source files
add_executable(program ${SOURCES})
