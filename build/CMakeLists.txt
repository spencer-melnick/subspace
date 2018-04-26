cmake_minimum_required(VERSION 2.9)
project(subspace CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Grab all C++ sources and include all headers
file(GLOB_RECURSE SOURCES "../src/*.cpp")
include_directories("../src")


# Output to "bin/debug" for debug build
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    file(MAKE_DIRECTORY "../bin/debug")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin/debug")

# Output to "bin/release" for release build
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    file(MAKE_DIRECTORY "../bin/release")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin/release")
endif()

add_executable(${PROJECT_NAME} ${SOURCES})
