# RAID Markov Model Simulator

A lightweight command-line utility written in C for calculating the reliability of a RAID storage system using Markov chains. The program uses state-space mapping integrated via Euler's Method to predict data loss probabilities over a customizable time horizon based on disk failure rates and recovery windows.

---

## Build Instructions

### Linux and macOS

The repository includes a `Makefile` to handle multi-file compilation.

Open a terminal in the project root directory and run:

```bash
# Build the executable
make

# Remove object files and generated binaries
make clean
```

---

### Windows

Windows users have two primary options for building the project.

#### Option A: GCC via MinGW/MSYS2 (Recommended)

1. Install MSYS2 or MinGW with the GCC toolchain and `make`.
2. Open the **MSYS2 MinGW UCRT64** terminal.
3. Navigate to the project directory and build:

```bash
make
```

Run the executable:

```bash
./raid_sim.exe
```

---

#### Option B: Microsoft Visual C++ (MSVC)

1. Open the **Developer Command Prompt for Visual Studio**.
2. Navigate to the project directory.
3. Compile the source files:

```cmd
cl /O2 /Fe:raid_sim.exe src\raidSim.c src\setup.c src\sim.c
```

Run the executable:

```cmd
raid_sim.exe
```

---

## Command-Line Options

```
RAID_SIM(1)                 User Commands                 RAID_SIM(1)

NAME
       raid_sim - Markov chain simulation for RAID reliability analysis

SYNOPSIS
       raid_sim [OPTIONS]

DESCRIPTION
       raid_sim models data loss probabilities for a disk storage array
       over a given time horizon using Euler numerical integration.

OPTIONS
       -d, --disks NUM
              Number of disks in the array (Integer >= 2).
              Default: 3.

       -y, --years NUM
              Simulation timeline length in years (Float > 0).
              Default: 30.0.

       --mtbf NUM
              Mean Time Between Failures in hours (Float > 0).
              Default: 1200000.0.

       --mttr NUM
              Mean Time To Repair in hours (Float > 0).
              Default: 24.0.

       --dt NUM
              Euler integration step size (Float > 0).
              Default: 0.1.

EXAMPLES
       Simulate a 5-disk array over 10 years with a
       12.5 hour MTTR:

              raid_sim -d 5 -y 10 --mttr 12.5
```

---

## Model Initialization

The simulation tracks the storage array through three Markov states:

| State | Description |
|-------|-------------|
| **P0** | Fully healthy array |
| **P1** | Degraded array (one failed disk undergoing rebuild) |
| **P2** | Data loss (absorbing state) |

The initial state probabilities are configured globally in `src/sim.h`:

| Macro | Default | Description |
|-------|---------|-------------|
| `INIT_P0` | `1.0` | Probability the array starts healthy |
| `INIT_P1` | `0.0` | Probability the array starts degraded |
| `INIT_P2` | `0.0` | Probability data loss has already occurred |

Changing these values allows the simulator to begin from different operating conditions without modifying the simulation logic.

---

## Simulation

The simulator advances the Markov state probabilities using Euler's Method. At each time step (`--dt`), transition probabilities are updated according to the specified failure and repair rates until the requested simulation duration (`--years`) is reached.

The resulting state probabilities provide an estimate of storage reliability over time under the selected configuration.