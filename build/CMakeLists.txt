cmake_minimum_required(VERSION 2.9)
project(subspace CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compile commands for VSCode
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Embed the contents of default_config.json as a string literal
file(STRINGS "../src/generated/default_config.json" CONFIG_STR_RAW)
string(REPLACE "\"" "\\\"" "${PROJECT_NAME}_DEFAULT_CONFIG_STR" ${CONFIG_STR_RAW})
configure_file(
    "../src/generated/default_config.hpp.in"
    "${PROJECT_BINARY_DIR}/generated/default_config.hpp"
)
include_directories(${PROJECT_BINARY_DIR})

# Grab all C++ sources and include all headers
file(GLOB_RECURSE SOURCES "../src/*.cpp")
include_directories("../src")

# Output to "bin/debug" for debug build
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    file(MAKE_DIRECTORY "../bin/debug")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin/debug")

# Output to "bin/release" for release build or default
else()
    file(MAKE_DIRECTORY "../bin/release")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin/release")
endif()

add_executable(${PROJECT_NAME} ${SOURCES})

# Include nlohman/json
target_include_directories(${PROJECT_NAME} PUBLIC "../lib/json/include")
