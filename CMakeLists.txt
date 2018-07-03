cmake_minimum_required(VERSION 3.11)
project(subspace CXX)

# Use C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable all warnings
if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)

    # Enable all warnings on GCC
    # if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    #     add_compile_options(-Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization
    #     -Wformat=2 -Winit-self -Wlogical-op -Wmissing-include-dirs -Wnoexcept -Wold-style-cast 
    #     -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-promo -Wstrict-null-sentinel
    #     -Wstrict-overflow=5 -Wundef -Wno-unused -Wno-variadic-macros -Wno-parentheses)
    # endif()
endif()

# Compile commands for VSCode
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# Embed the contents of default_config.json as a string literal
file(STRINGS "src/generated/default_config.json" CONFIG_STR_RAW)
string(REPLACE "\"" "\\\"" "${PROJECT_NAME}_DEFAULT_CONFIG_STR" ${CONFIG_STR_RAW})
configure_file(
    "src/generated/default_config.hpp.in"
    "${PROJECT_BINARY_DIR}/generated/default_config.hpp"
)
include_directories(${PROJECT_BINARY_DIR})


# Grab all C++ sources and include all headers
set(SOURCES
    "src/main.cpp"
    "src/engine/util/config.cpp"
    "src/engine/util/logger.cpp"
    "src/engine/util/simple_file.cpp"
    "src/engine/video/vulkan/context.cpp"
    "src/engine/video/vulkan/physical_device.cpp"
    "src/engine/video/vulkan/sdl_window.cpp"
    "src/engine/video/vulkan/swapchain.cpp"
    "src/engine/video/vulkan/vulkan_renderer.cpp"
    "src/engine/video/vulkan/vulkan_window.cpp"
)
include_directories("src")


# Output to "bin/debug" for debug build
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/debug")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/debug")
# Output to "bin/release" for release build or default
else()
    file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/release")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/release")
endif()


# Custom target for building documentation
file(MAKE_DIRECTORY "doc/output")
add_custom_target(docs COMMAND "doxygen" "${CMAKE_SOURCE_DIR}/doc/Doxyfile"
                       #COMMAND "make" "-Cdoc" "html"
                 )


# Executable
add_executable(${PROJECT_NAME} ${SOURCES})


# Link/include external libraries


# Use extra CMake modules on Windows
if (WIN32)
	list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/lib/cmake-modules") 
endif()


# Include nlohman/json
target_include_directories(${PROJECT_NAME} PRIVATE "lib/json/include")

# {fmt}
find_package(fmt REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)
target_include_directories(${PROJECT_NAME} PRIVATE fmt::fmt)

# Vulkan API
find_package(Vulkan REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE ${Vulkan_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${Vulkan_INCLUDE_DIRS})

# SDL2
find_package(SDL2 REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIRS})
