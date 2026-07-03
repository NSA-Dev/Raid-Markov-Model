# RAID Markov Model Simulator

This repository contains a small C-based Markov reliability simulator for the
HAW Hamburg Safety Systems project report:

> Case study: RAID Storage Subsystem - Concept and Safety Evaluation for a
> 30-Year Operational Horizon

The simulator estimates structural RAID-layer data loss caused by independent
disk failures only. It supports the report by reproducing the three fixed
case-study tiers and their 30-year data-loss probabilities.

By default, the executable runs all three report scenarios:

| Report tier | Architecture | Disk count |
|---|---|---:|
| 5 TB Critical Records Vault | RAID 1 + hot spare | 3 |
| 50 TB Active Production Pool | RAID 10 | 8 |
| 300 TB Regional Media Vault | RAID 6 | 16 |

The model does not include latent bit corruption, common-cause failures,
backup/restore reliability, controller failure, site-level events, ransomware,
or human error. Those risks are treated separately in the report.

---

## Build

### Linux and macOS

```bash
make
./raid_sim
```

To remove generated files:

```bash
make clean
```

### Windows

With MinGW/MSYS2:

```bash
make
./raid_sim.exe
```

With Microsoft Visual C++:

```cmd
cl /O2 /Fe:raid_sim.exe src\raidSim.c src\setup.c src\sim.c
raid_sim.exe
```

---

## Command-Line Options

The current executable is case-study oriented. It always runs the three fixed
report tiers listed above. The following options adjust the shared assumptions
for all tiers:

| Option | Meaning | Default |
|---|---|---:|
| `--mtbf NUM` | Mean Time Between Failures in hours | `1200000.0` |
| `--mttr NUM` | Mean Time To Repair in hours | `24.0` |
| `--years NUM`, `-y NUM` | Simulation horizon in years | `30.0` |
| `--dt NUM` | Euler integration timestep in hours | `0.1` |

The parser also accepts `--disks NUM` / `-d NUM`, but this option is not
meaningful in the default three-tier case-study mode. Each report tier
overwrites the disk count internally: 3 disks for RAID 1 + hot spare, 8 disks
for RAID 10, and 16 disks for RAID 6.

---

## Model Scope

The Markov model estimates only the probability that the RAID layer reaches a
structural data-loss state due to independent disk failures before repair.

Model assumptions:

- independent disk failures
- constant failure rate
- constant repair rate
- one disk repaired at a time
- identical MTBF for all disks
- no correlated failures inside the Markov model
- no bit-corruption modelling inside the Markov model
- no backup/restore modelling inside the Markov model

Limitations:

- RAID 10 is simplified. It models the dominant data-loss path where a second
  failure hits the exact mirror twin, but it does not track all multi-degraded
  surviving states.
- RAID 1 + hot spare is a simplified conservative model because it does not
  distinguish active mirror disk failure from spare disk failure.
- RAID 6 uses a four-state model and is the most explicit state model in the
  current implementation.
- The output is not a total real-world probability of losing business data.
  Common-cause events, backup integrity, restore reliability, human error,
  ransomware, controller failure, site loss, and latent bit corruption require
  separate safety/dependability arguments.

---

## Markov States

The implementation uses these state meanings:

| State | Meaning |
|---|---|
| `P0` | Healthy array |
| `P1` | One failed disk / degraded state |
| `P2` | Second degraded state for RAID 6, or absorbing data-loss state for simpler models depending on RAID type |
| `P3` | RAID 6 absorbing data-loss state |

Initial probabilities are defined in `src/sim.h`:

| Macro | Default |
|---|---:|
| `INIT_P0` | `1.0` |
| `INIT_P1` | `0.0` |
| `INIT_P2` | `0.0` |
| `INIT_P3` | `0.0` |

---

## Markov Equations

The simulator integrates these continuous-time Markov equations with Euler's
method. Let:

- `N` be the number of disks in the tier
- `lambda = 1 / MTBF`
- `mu = 1 / MTTR`

### RAID 1 + Hot Spare

```text
dP0/dt = mu*P1 - N*lambda*P0
dP1/dt = N*lambda*P0 - mu*P1 - (N-1)*lambda*P1
dP2/dt = (N-1)*lambda*P1
```

### RAID 10

```text
twin_fail_rate = ((N-1)*lambda*P1) * (1/(N-1)) = lambda*P1

dP0/dt = mu*P1 - N*lambda*P0
dP1/dt = N*lambda*P0 - mu*P1 - twin_fail_rate
dP2/dt = twin_fail_rate
```

### RAID 6

```text
dP0/dt = mu*P1 - N*lambda*P0
dP1/dt = N*lambda*P0 - mu*P1 - (N-1)*lambda*P1 + mu*P2
dP2/dt = (N-1)*lambda*P1 - mu*P2 - (N-2)*lambda*P2
dP3/dt = (N-2)*lambda*P2
```

---

## Sample Outputs

### Manufacturer MTBF Case

Command:

```bash
make
./raid_sim
```

Output:

```text
=== 30-Year Storage Reliability Verification ===
Array Configuration: 3-disk RAID 1 (+ Hot-Spare) array
Final State Probabilities after 262980.0 Hours:
  Healthy (P0):            99.991371%
  Degraded/At Risk (P1):   0.005999%
  Permanent Data Loss:     0.002629%
================================================
=== 30-Year Storage Reliability Verification ===
Array Configuration: 8-disk RAID 10 array
Final State Probabilities after 262980.0 Hours:
  Healthy (P0):            99.980498%
  Degraded/At Risk (P1):   0.015997%
  Permanent Data Loss:     0.003505%
================================================
=== 30-Year Storage Reliability Verification ===
Array Configuration: 16-disk RAID 6 array
Final State Probabilities after 262980.0 Hours:
  Healthy (P0):            99.967971%
  Degraded/At Risk (P1):   0.031990%
  Permanent Data Loss:     0.000029%
================================================
```

These match the report values:

| Architecture | Manufacturer MTBF result |
|---|---:|
| RAID 1 + hot spare | 0.002629% |
| RAID 10 | 0.003505% |
| RAID 6 | 0.000029% |

### Guarantee-Implied MTBF Case

Command:

```bash
./raid_sim --mtbf 249700
```

Output:

```text
=== 30-Year Storage Reliability Verification ===
Array Configuration: 3-disk RAID 1 (+ Hot-Spare) array
Final State Probabilities after 262980.0 Hours:
  Healthy (P0):            99.910514%
  Degraded/At Risk (P1):   0.028803%
  Permanent Data Loss:     0.060683%
================================================
=== 30-Year Storage Reliability Verification ===
Array Configuration: 8-disk RAID 10 array
Final State Probabilities after 262980.0 Hours:
  Healthy (P0):            99.842365%
  Degraded/At Risk (P1):   0.076764%
  Permanent Data Loss:     0.080872%
================================================
=== 30-Year Storage Reliability Verification ===
Array Configuration: 16-disk RAID 6 array
Final State Probabilities after 262980.0 Hours:
  Healthy (P0):            99.842977%
  Degraded/At Risk (P1):   0.153543%
  Permanent Data Loss:     0.003259%
================================================
```

Rounded for the report, the guarantee-implied results are approximately:

| Architecture | Guarantee-implied MTBF result |
|---|---:|
| RAID 1 + hot spare | 0.061% |
| RAID 10 | 0.081% |
| RAID 6 | 0.0033% |
