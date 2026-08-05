# NATCBS with CPLEX Warm Start

## Overview

This mode uses the CPLEX backend with persistent model reuse and Simplex basis warm-starting enabled. It is the configuration that is closest to the optimized CPLEX path in the code.

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
cmake -S . -B build-cplex-warm -DFLOW_BACKEND=CPLEX -DCMAKE_BUILD_TYPE=Release
cmake --build build-cplex-warm -j
```

## Run

Enable warm start and model reuse before launching the solver:

```bash
export MAWR_CPLEX_WARM_START=1
export MAWR_CPLEX_REUSE_MODEL=1
./build-cplex-warm/MAWR -m <map_file> -s <scenario_file> -a NATCBS --v 2
```

These are the defaults in the code, so you only need to export them if you want to make the mode explicit or override a shell that previously disabled them.

## What Warm Start Does

The planner keeps a persistent CPLEX network model and stores the Simplex basis from the previous iteration. On the next solve, it tries to inject that basis before running `CPXNETprimopt` again. If the basis becomes incompatible, the code falls back to a cold start automatically.

## Verification

With verbose logging, a successful warm-start solve should include:

```text
[CPLEX] Applied differential update (bounds/objective).
[CPLEX] Warm start basis loaded.
```

To view the full log, redirect the solver output to a file.