![Subspace Logo](/rc/logo.png)

# subspace

The subspace engine is a simple game engine written in C++. It was created primarily as an application of and to further hone my own programming skills, and to also allow me to rapidly prototype different game concepts in C++ without the requirement of a commercial engine such as Unreal or Unity.

Subspace is designed to be easily compiled on any modern Windows, Linux, or OS X system.

## Visual Studio Code

Subspace is developed primarily using Visual Studio Code on Ubuntu and includes `.vscode` configuration files including automated tasks for configuring, building, and debugging.

## Vulkan

Subspace's primary (work-in-progress) renderer is written using the cross-platform [Vulkan API](https://www.khronos.org/vulkan/). Vulkan utilizes lower-level access to graphics hardware, allowing for better automated GPU optimization and CPU/GPU synchronization, particularly with regards the multithreaded rendering.

Vulkan support is provided on most graphics cards 2012 and newer, and on newer Intel intergrated GPUs. Vulkan will also receive continued support on Mac OS X via the [MoltenVK](https://moltengl.com/moltenvk/) library.

However, as older hardware, and many integrated GPUs on Windows do not support the Vulkan API, a legacy OpenGL renderer may be developed at a later time.

To see if your device supports Vulkan, a quick [reference](https://moltengl.com/moltenvk/) is available via Wikipedia.

## Dependencies

Subspace currently uses the following libraries:
* [SDL2](https://www.libsdl.org/)
* [fmt](https://github.com/fmtlib/fmt)
* [Vulkan](https://www.lunarg.com/vulkan-sdk/)

The following libraries are automatically included from source:
* [nlohmann/json](https://github.com/nlohmann/json)


# Building

## Requirements

Subspace is configured using [CMake](https://cmake.org) and currently supports the latest versions of the following compilers:

* [gcc](https://gcc.gnu.org/)
* [Mingw-w64](https://mingw-w64.org/doku.php)
* [MSVC](https://visualstudio.microsoft.com/)

## Initializing the git submodules

Because subspace includes some libraries from source, you must make sure to run `git submodule init` and `git submodule update` to ensure that the latest versions or these libraries are fetched from their git repositories.

## Fetching dependencies

If you are on a Unix system, you can download some of the necessary libraries from your package manager. These are their package names on Ubuntu:

* `libsdl2-dev`*
* `libvulkan-dev`

Other libraries will need to be built from source, including:
* [fmt](https://github.com/fmtlib/fmt)


<sub>*subspace requires at least SDL 2.0.7 for Vulkan support. If your package manager does not supply this version or later, you will need to build SDL from source</sub>

## Configuring

### Windows

Download or build binaries for subspace's dependencies as needed. To configure,navigate to the root directory and run CMake in GUI mode, selecting the folders containing the libraries' `find[LibraryNameHere].cmake` file.

Select either Visual Studio or MinGW makefiles and hit generate. With Visual Studio generation, CMake should generate a Visual Studio project file.

### Linux

To configure subspace for Linux navigate to the root directory and create a `build` directory. Navigate to the `build` directory and run `cmake ../`

When CMake generation is completed run `make` to build the project, or `make docs` to generate Doxygen documentation.

If the build completed successfully, `subspace` should be created in `bin/release/` or `bin/debug/`, depending on which configuration you selected when running CMake generation.

### Mac OS X

Mac OS X builds have not been tested yet, but the build process should be similar to the one for Linux.
