![Subspace Logo](/rc/logo.png)

# subspace

Subspace is designed to be easily compiled on any modern Windows, Linux, or OS X system.

## Visual Studio Code

Subspace is developed primarily using Visual Studio Code and includes `.vscode` configuration files including automated tasks for configuring, building, and debugging.

## Dependencies

Subspace currently uses the following libraries:
* Nothing here yet!

The following libraries are automatically included from source:
* [nlohmann/json](https://github.com/nlohmann/json)


# Building

## Requirements

Subspace is configured using [CMake](https://cmake.org) and currently supports the latest versins of the following compilers:

* [gcc](https://gcc.gnu.org/)
* [Mingw-w64](https://mingw-w64.org/doku.php)

## Initializing the git submodules

Because subspace includes some libraries from source, you must make sure to run `git submodule init` and `git submodule update` to ensure that the latest versions or these libraries are fetched from their git repositories.

## Fetching dependencies

If you are on a Unix system, you can download the necessary libraries from your package manager.

No libraries necessary yet!

## Configuring

### Windows

To configure subspace for Windows navigate to the root directory and run `cmake -DCMAKE_BUILD_TYPE="Release" -Hbuild -Bbuild -G'MinGW Makefiles`.

To build run `mingw32-make -C build`.

If the build completed successfully, `subspace.exe` should be created in `bin/release/`.

### Unix

To configure subspace for Windows navigate to the root directory and run `cmake -DCMAKE_BUILD_TYPE="Release" -Hbuild -Bbuild`.

To build run `make -C build`.

If the build completed successfully, `subspace` should be created in `bin/release/`.
