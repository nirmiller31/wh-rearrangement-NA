# NATCBS with CPLEX Warm Start

## Overview

This mode uses the CPLEX backend with persistent model reuse and simplex basis warm-starting enabled. It is the configuration that is closest to the optimized CPLEX path in the code.

## Prerequisites

- The same IBM CPLEX access described in the cold-start guide
- CPLEX unpacked to the repository's expected path, or equivalent include and library paths in `CMakeLists.txt`

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
./build-cplex-warm/MAWR -m <map_file> -s <scenario_file> -a NATCBS -v 2
```

These are the defaults in the code, so you only need to export them if you want to make the mode explicit or override a shell that previously disabled them.

## What Warm Start Does

The planner keeps a persistent CPLEX network model and stores the simplex basis from the previous iteration. On the next solve, it tries to inject that basis before running `CPXNETprimopt` again. If the basis becomes incompatible, the code falls back to a cold start automatically.

## Verification

With verbose logging, a successful warm-start solve should print messages similar to:

```text
[CPLEX] Warm start basis loaded.
```

If the topology changes or the basis becomes invalid, the code may report a fallback to cold start instead.