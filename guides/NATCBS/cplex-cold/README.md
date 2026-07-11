# NATCBS with CPLEX Cold Start

## Overview

This mode uses IBM CPLEX for the NATCBS flow planner with warm-start disabled. It still uses the CPLEX backend, but each solve starts from scratch instead of reusing a previous simplex basis.

## Prerequisites

- A valid IBM account with access to IBM ILOG CPLEX Optimization Studio
- The CPLEX download and license package for Linux
- A C++20 compiler, CMake 3.21+, and Boost `program_options`

## How to get a CPLEX account

1. Create or sign in to an IBMid on IBM's account portal.
2. Use that IBMid to access the IBM ILOG CPLEX Optimization Studio download page.
3. Accept the license terms for the edition you are entitled to use, then download the Linux package.
4. If your access is academic or trial-based, keep the entitlement or license file that IBM provides during the download or activation flow.

IBM may present slightly different screens depending on whether you are using a trial, academic, or commercial entitlement, but the end result is the same: a signed-in IBM account that can download and activate CPLEX.

## How to integrate CPLEX in this repository

The current CMake file expects CPLEX to be unpacked under this path:

```bash
$HOME/cplex2212/cplex
```

That means these files must exist:

- `include/ilcplex/cplex.h` under the include tree
- `lib/x86-64_linux/static_pic/libcplex.a` under the library tree

If you install CPLEX somewhere else, either create a symlink to match that layout or update the include and library paths in `CMakeLists.txt`.

If your license setup uses a local file, make sure CPLEX can find it before running the program. Follow the IBM activation instructions that came with your download.

## Build

```bash
cmake -S . -B build-cplex-cold -DFLOW_BACKEND=CPLEX -DCMAKE_BUILD_TYPE=Release
cmake --build build-cplex-cold -j
```

## Run

Set CPLEX to cold-start mode before launching the solver:

```bash
export MAWR_CPLEX_WARM_START=0
export MAWR_CPLEX_REUSE_MODEL=1
./build-cplex-cold/MAWR -m <map_file> -s <scenario_file> -a NATCBS -v 2
```

`MAWR_CPLEX_REUSE_MODEL` can stay enabled for incremental model updates, but the solver will not reuse the previous simplex basis while `MAWR_CPLEX_WARM_START=0`.

## Verification

With verbosity enabled, a cold-start run should log that no reusable basis is available or that the solver is running without warm start. If the backend is wired correctly, the build should link against the CPLEX headers and static library during configuration and compilation.

## Common Issues

- CMake cannot find `ilcplex/cplex.h`: check the unpack path or update the include directory in `CMakeLists.txt`.
- Link errors against `libcplex.a`: confirm the static library path matches the repository's expected layout.
- Runtime license errors: verify that your IBM entitlement or license file is active and visible to CPLEX.