# Optimal Multi-Agent Warehouse Rearrangement

This repository contains the source code for the paper:

**"From Agent Centric to Obstacle Centric Planning: A Makespan-Optimal Algorithm for the Multi-Agent Warehouse Rearrangement Problem"**

Yaakov Sherma, [Eyal Weiss](https://sites.google.com/view/eyal-weiss/home), and [Oren Salzman](https://crl.cs.technion.ac.il/)

Published in the *Proceedings of the 18th International Symposium on Combinatorial Search* (SoCS 2025), pp. 136–144, Glasgow, UK.

🏆 **Best Paper Award — SoCS 2025**

📄 [Paper (PDF)](https://ojs.aaai.org/index.php/SOCS/article/view/35985/38140)

## Overview

We present a novel approach to the Multi-Agent Warehouse Rearrangement Problem that shifts the planning perspective from agent-centric to obstacle-centric. Instead of planning paths for agents while avoiding collisions, our algorithm focuses on the obstacles that need to be moved and plans the minimal set of agent actions required to rearrange them. This yields a **makespan-optimal** algorithm that outperforms prior unbounded sub-optimal approaches to makespan minimization.

## Dependencies

- C++20 compatible compiler
- CMake 3.21+
- [Boost](https://www.boost.org/) (program_options)
- [Google OR-Tools](https://developers.google.com/optimization)

## Building

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
```

## Running

```bash
./MAWR -m <map_file> -s <scenario_file> -a NATCBS [options]
```

### Required Arguments
| Flag | Description |
|------|-------------|
| `-m, --map` | Map file (`.map` format) |
| `-s, --scenario` | Scenario file (`.scen` format) |
| `-a, --alg` | Solver algorithm (`NATCBS`) |

### Optional Arguments
| Flag | Default | Description |
|------|---------|-------------|
| `-t, --time` | 60 | Time limit in seconds |
| `-o, --output` | `results.csv` | Results output file |
| `--v` | 1 | Verbosity level: 0 (minimal), 1 (normal), 2 (debug) |

### Input Format

**Map file:** First line contains rows and columns. Grid uses `.` for passable cells, `#` for static obstacles, and `@` for blocked cells.

**Scenario file:** First line contains the number of agents, obstacles, and tasks. Followed by agent start locations, then pickup and delivery locations for each obstacle.

## Citation

If you use this code in your research, please cite:

```bibtex
@inproceedings{sherma2025agent,
  title     = {From Agent Centric to Obstacle Centric Planning: A Makespan-Optimal Algorithm for the Multi-Agent Warehouse Rearrangement Problem},
  author    = {Sherma, Yaakov and Weiss, Eyal and Salzman, Oren},
  booktitle = {Proceedings of the 18th International Symposium on Combinatorial Search (SoCS)},
  pages     = {136--144},
  year      = {2025}
}
```

## License

This project is licensed under the MIT License — see the [LICENSE](LICENSE) file for details.

## Acknowledgments

This research was conducted at the [Computational Robotics Lab (CRL)](https://crl.cs.technion.ac.il/), Computer Science Department, Technion — Israel Institute of Technology.
