# NATCBS Guides

## Overview

NATCBS uses a backend-specific flow solver for the obstacle-centric planning stage. This repository supports two backend families:

- OR-Tools, which is the default build configuration.
- CPLEX, which can be run in cold-start or warm-start mode.

The executable and command-line interface stay the same across all modes. The difference is the backend chosen at CMake configure time and the runtime environment variables used by the CPLEX flow planner.

## Available Guides

- [OR-Tools mode](ortools/README.md)
- [CPLEX cold-start mode](cplex-cold/README.md)
- [CPLEX warm-start mode](cplex-warm/README.md)

## Shared Run Shape

All NATCBS modes use the same solver invocation pattern:

```bash
./MAWR -m <map_file> -s <scenario_file> -a NATCBS
```

The backend is selected when you configure CMake, not through the `-a` flag.