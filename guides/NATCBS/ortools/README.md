# NATCBS with OR-Tools

## Overview

This mode uses Google OR-Tools for the flow-planning component of NATCBS. It is the default backend in this repository and is the simplest way to build and run the solver.

## Prerequisites

- A C++20 compiler
- CMake 3.21 or newer
- Boost with the `program_options` component
- Google OR-Tools with CMake package files available to `find_package(ortools REQUIRED)`

If CMake cannot find OR-Tools, add the installation prefix to `CMAKE_PREFIX_PATH` or set the package path for your local setup.

## How to get OR-Tools

OR-Tools is an open-source library from Google, so you do not need a paid license or special account to use it. The main requirement is to install it in a way that CMake can discover.

### Option 1: Build and install from source on Linux

This is the most direct option if you want a local install prefix that the project can use immediately.

```bash
sudo apt update
sudo apt install -y git cmake build-essential pkg-config python3

git clone https://github.com/google/or-tools.git
cd or-tools

# Pick a released version tag if you want a stable build.
# git checkout v9.10

cmake -S . -B build \
	-DCMAKE_BUILD_TYPE=Release \
	-DCMAKE_INSTALL_PREFIX="$HOME/local/or-tools" \
	-DBUILD_TESTING=OFF \
	-DBUILD_EXAMPLES=OFF

cmake --build build -j
cmake --install build
```

After installation, expose the prefix to this repository:

```bash
export CMAKE_PREFIX_PATH="$HOME/local/or-tools:${CMAKE_PREFIX_PATH}"
```

### Option 2: Use a package manager install

If your package manager provides OR-Tools with CMake package files, install it there and then point this project at that prefix.

```bash
# Replace <package-command> with the command for your environment.
# Examples might be apt, brew, conda, or another package manager.
<package-command> install ortools

export CMAKE_PREFIX_PATH="<install-prefix>:${CMAKE_PREFIX_PATH}"
```

### Verify the install

Check that the install prefix contains the CMake package files expected by `find_package(ortools REQUIRED)`:

```bash
find "$HOME/local/or-tools" -name 'ortoolsConfig.cmake'
```

If the command prints a path, CMake should be able to find OR-Tools once `CMAKE_PREFIX_PATH` includes that install prefix.

## Build

Configure the project with the OR-Tools backend:

```bash
cmake -S . -B build-ortools -DFLOW_BACKEND=ORTOOLS -DCMAKE_BUILD_TYPE=Release
cmake --build build-ortools -j
```

## Run

```bash
./build-ortools/MAWR -m <map_file> -s <scenario_file> -a NATCBS
```

Useful optional flags:

- `-t` sets the time limit in seconds.
- `-o` selects the CSV output file.
- `-v 2` enables debug logging, which is useful when comparing solver behavior.

## Verification

A successful run should print the map, scenario summary, and a final plan with an elapsed time. If OR-Tools is missing, the failure usually happens at configure time when `find_package(ortools REQUIRED)` cannot resolve the package.