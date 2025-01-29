# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name
project(gbemu)

# Set the C++ Standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)

# Declare SFML library
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON)

# Disable unnecessary SFML components
set(SFML_BUILD_DOC OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_TEST_FRAMEWORK OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)

# Fetch and make SFML available
FetchContent_MakeAvailable(SFML)

# Add include directories (Ensure SFML headers are visible)
include_directories(${SFML_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/include)

# Find all .cpp files in src directory
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/*.cpp)

# Add the executable
add_executable(gbemu main.cpp ${SRC_FILES})

# Link SFML properly
target_link_libraries(gbemu PRIVATE sfml-graphics sfml-window sfml-system)
