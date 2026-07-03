# RAID Safety Systems Project Context

This file is project memory for Codex and future AI assistants working on this repository.

The repository belongs to a university project for the course **Design and Analysis of Mission-Critical Systems** at **HAW Hamburg**. The project topic is **RAID storage dependability**. The goal is not just to write a C simulator, but to support a formal Safety Systems-style report about RAID architectures, reliability, criticality, and the probability of irreversible data loss over a 30-year operational horizon.

---

## 1. Central Project Question

The professor's practical question is:

> What is the probability of data loss in 30 years?

The professor also indicated that:

- A Markov model is a good choice.
- The project should consider specific deployment cases.
- One case can be "not a lot of data but very critical", e.g. 5 TB.
- Another case can be "a lot of data".
- Acceptable losses can be considered, but are not required.
- The report must include a holistic analysis of different scenarios.
- Procurement and maintenance costs must be considered.
- Probability of data loss should be linked to architecture and cost decisions.
- Markov models can be programmed for case-study elaboration.
- The central design question is whether the company wants a reliable system for day-to-day use.

---

## 2. Course / Report Expectations

The project follows a Safety Systems / mission-critical systems structure. The report is expected to show:

1. Executive summary
2. Overview
3. System requirements
4. Mission-critical functionality
5. System architecture
6. Proof of correct functional operation
7. Safety/dependability case
8. Summary
9. Literature and references
10. Appendix / code evidence

The report should be understandable to someone with general technical knowledge, not necessarily a RAID expert.

The grading concept emphasizes:

- Structure and logic
- Correctness
- Critical thinking
- Results
- Quality of figures/tables
- Citations
- Whether the defined project goals are met

Important: this is not just a generic RAID comparison. It must show a system-architect view: requirements, criticality, architecture choices, risk management, reliability modelling, and justification.

---

## 3. Current Report Framing

The report title is:

> Case study: RAID Storage Subsystem — Concept and Safety Evaluation for a 30-Year Operational Horizon

The system is framed as a RAID-based storage subsystem for a **global streaming and media production company**. The report sometimes says "example case: Netflix". It is safer to interpret this as a Netflix-like company rather than a claim about Netflix's real internal architecture.

The system under analysis is called:

> Dependable Media Storage Subsystem

The system boundary includes:

- local RAID storage arrays
- physical disks
- RAID controller or software RAID layer
- monitoring service
- hot-swap maintenance process
- disk replacement process
- data scrubbing
- degradation alerts
- scheduled backup to an independent storage target
- restore procedure

The system boundary excludes:

- full global CDN architecture
- application servers
- user devices
- cloud service architecture
- business software that uses the data

---

## 4. Three Workload Scenarios

The report evaluates three tiers.

### Tier 1: 5 TB Critical Records Vault

Purpose:

- Small but highly critical storage.
- Legal records.
- Production records.
- Rights-management records.
- Metadata and compliance records.

Main concern:

- Data integrity.
- Recoverability.
- Simple maintenance.
- Low absolute storage cost, so high overhead is acceptable.

Selected architecture:

- RAID 1 with a hot spare.

Rationale:

- The capacity is small, so mirroring overhead is acceptable.
- Simplicity and easy restoration matter more than parity efficiency.
- Avoids RAID 5/6 parity rebuild complexity.

### Tier 2: 50 TB Active Production Pool

Purpose:

- Active studio/media production workload.
- Editing files.
- Rendering files.
- Active project files.

Main concern:

- High random read/write performance.
- High availability.
- Short degraded-operation time.
- Fast rebuild.

Selected architecture:

- RAID 10.

Rationale:

- Good random read/write performance.
- Localized rebuilds: only the failed mirror member needs reconstruction.
- Suitable for active workloads where throughput and recovery time both matter.

### Tier 3: 300 TB Regional Media Storage Vault

Purpose:

- Large regional media/content storage.
- Large capacity tier.
- Distribution/retrieval of regional media files.

Main concern:

- Capacity efficiency.
- Cost.
- Long-term reliability.
- Rebuild risk.
- Latent bit corruption risk.

Selected architecture:

- RAID 6.

Rationale:

- RAID 6 provides dual parity.
- It can tolerate two disk failures.
- It is more defensible than RAID 5 at 300 TB scale.
- RAID 10 would impose much higher capacity overhead at this size.

---

## 5. RAID Levels Considered

The report compares:

- RAID 0
- RAID 1
- RAID 5
- RAID 6
- RAID 10

Other RAID levels such as RAID 2/3/4 may appear in the overview table, but the main decision set is 0/1/5/6/10.

Key treatment:

- **RAID 0**: baseline only; rejected for permanent or mission-critical storage because any single disk failure causes data loss.
- **RAID 1**: selected for small critical storage because of simplicity and mirroring.
- **RAID 5**: analyzed but rejected for 300 TB scale because it has only single parity and is vulnerable during rebuild.
- **RAID 6**: selected for the large 300 TB tier because it retains redundancy during rebuild.
- **RAID 10**: selected for the 50 TB performance-critical production tier.

Important wording corrections:

- Use **disk/disks**, not disc/discs.
- RAID 1 fault tolerance: a standard 2-disk mirror tolerates 1 disk failure. An m-way mirror can tolerate m-1 failures if one complete copy remains.
- RAID 10 fault tolerance: at least 1 disk failure is guaranteed; multiple failures may be tolerated if they occur in different mirror pairs. Data loss occurs if both disks in the same mirror pair fail.

---

## 6. Safety / Criticality Classification

The storage subsystem is **reliability-critical**, not directly safety-critical.

Reason:

- It does not command physical equipment.
- It is physically and logically separated from actuation.
- Its failure cannot directly lead to death or injury.
- Therefore, no tolerable hazard rate (THR) is derived.

However, irreversible data loss can cause:

- operational damage
- financial loss
- legal exposure
- reputational damage
- production delays
- service degradation

So the dependability case focuses on avoiding unacceptable data-loss hazards.

Important: If the storage system were later connected to safety-relevant actuation or safety-critical decisions, the classification would need to be revisited.

---

## 7. Functional Requirements

Current report functional requirements:

| ID | Functional requirement |
|---|---|
| FR-1 | Provide usable storage capacities of 5 TB, 50 TB, and 300 TB for the three defined scenarios. |
| FR-2 | Store and retrieve data reliably during normal day-to-day operation. |
| FR-3 | Continue operation in degraded mode after a tolerated disk failure according to the selected RAID level. |
| FR-4 | Detect disk failure, array degradation, and rebuild status automatically. |
| FR-5 | Support hot-swap replacement of failed disks where the architecture permits it. |
| FR-6 | Rebuild lost redundancy after disk replacement without causing irreversible data loss. |
| FR-7 | Perform scheduled data integrity checks or scrubbing to detect latent bit corruption. |
| FR-8 | Replicate critical data to an independent backup target and support tested restoration. |

---

## 8. Non-Functional Requirements

Current report non-functional requirements:

| ID | Non-functional requirement |
|---|---|
| NFR-1 | The probability of irreversible data loss over the 30-year operational horizon shall not exceed 10^-3 (0.1%) per tier. |
| NFR-2 | Achieve high availability for day-to-day storage access, especially for the 50 TB active production pool. |
| NFR-3 | Minimize rebuild exposure time after disk failure. |
| NFR-4 | Keep installation, operation, and maintenance costs proportional to the criticality of each scenario. |
| NFR-5 | Maintain acceptable read/write performance during normal and degraded operation. |
| NFR-6 | Detect and mitigate latent bit corruption through scheduled scrubbing. |
| NFR-7 | Reduce common-cause failure risk through backup, monitoring, UPS/power protection, human maintenance procedures, and restore testing. |
| NFR-8 | Ensure the system is understandable and maintainable by trained operators. |

Important recommended refinement for NFR-1:

> The probability of structural RAID-layer data loss caused by independent disk failures over the 30-year operational horizon shall not exceed 10^-3 (0.1%) per tier. Common-cause failures, human error, backup/restore failure, and site-level events are treated separately as residual system-level risks.

This avoids a contradiction with the beta-factor common-cause estimate, which may be around 1-5% over 30 years.

---

## 9. Cost and Procurement Analysis

The report connects disk procurement and expected maintenance effort.

Expected disk replacements over 30 years are calculated approximately as:

```text
Expected replacements = N * 30 / MTBF_years
```

Two MTBF cases are used:

1. Manufacturer MTBF: about 1.2e6 hours, about 137 years.
2. Guarantee-implied MTBF: about 28.5 years, derived from the project description's statement that about 10% of disks may fail within a 3-year guarantee period.

Current table values:

| Tier | Disks to procure (raw : usable) | Capacity overhead | Expected disk replacements in 30 yr (manufacturer / guarantee-implied MTBF) | Dominant cost driver |
|---|---:|---:|---:|---|
| 5 TB vault, RAID 1 + hot spare | 3 disks (3 : 1) | 200% | 0.7 / 3.2 | Overhead acceptable: absolute cost is small, integrity is decisive |
| 50 TB pool, RAID 10 | 8 disks (2 : 1) | 100% | 1.8 / 8.4 | Performance justifies mirror overhead; snapshots add license/admin cost |
| 300 TB vault, RAID 6 | 16 disks (16 : 14) | about 14% | 3.5 / 16.8 | Capacity efficiency; largest absolute spend, plus offsite replica |

Main conclusion:

- Mirroring overhead is acceptable for smaller tiers.
- RAID 6 is more economical at 300 TB.
- Under guarantee-implied failure rates, the 300 TB tier consumes around one replacement disk every two years.
- Monitoring, maintenance contracts, and cold spares are permanent operating costs.
- Since common-cause risk dominates residual risk, extra money should go to independent backup rather than endless RAID redundancy.

---

## 10. Markov Model Overview

The reliability analysis uses a continuous-time Markov chain solved with Euler numerical integration.

Input parameters:

- Disk failure rate: lambda = 1 / MTBF
- Repair rate: mu = 1 / MTTR
- MTTR = 24 hours
- Mission time = 30 years
- Hours per year = 8766
- Default timestep dt = 0.1 hours

Two failure-rate cases:

1. Manufacturer MTBF = 1.2e6 hours ≈ 137 years, AFR ≈ 0.73%.
2. Guarantee-implied MTBF ≈ 2.5e5 hours ≈ 28.5 years, AFR ≈ 3.5%.

Guarantee-implied derivation:

```text
R(3 years) = 0.9
MTBF = -3 / ln(0.9) ≈ 28.5 years
```

Important model assumptions:

1. Disk failures are statistically independent.
2. Failure and repair rates are constant.
3. One disk is repaired at a time.
4. All disks share the same MTBF.
5. The model captures structural disk redundancy only.
6. It excludes latent bit corruption, common-cause failures, backup/restore reliability, controller failure, ransomware, human error, and site-level events.
7. Those excluded risks are treated separately in the report.

---

## 11. Current Markov Results

Current report results:

| Workload Tier | Assigned Architecture | P(loss, 30 yr): manufacturer MTBF / guarantee-implied MTBF | Status vs. target < 0.1% |
|---|---|---:|---|
| 5 TB Records Vault | RAID 1 + hot spare (3 disks) | 0.0026% / 0.061% | Met in both cases |
| 50 TB Production Pool | RAID 10 (8 disks) | 0.0035% / 0.081% | Met in both cases |
| 300 TB Media Vault | RAID 6 (16 disks) | 0.000029% / 0.0033% | Met in both cases |

Program output for manufacturer-MTBF case:

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

These values are structural RAID-layer data loss probabilities from independent disk failures only.

---

## 12. Markov Equations

### RAID 1 with hot spare / simplified 3-state model

Current code uses states:

- P0: healthy
- P1: degraded / at risk
- P2: data loss

Approximate equations:

```text
dP0/dt = mu * P1 - N * lambda * P0
dP1/dt = N * lambda * P0 - mu * P1 - (N - 1) * lambda * P1
dP2/dt = (N - 1) * lambda * P1
```

Note: this is conservative and simplified because it does not distinguish active mirror disk failure from hot-spare failure.

### RAID 10 / simplified dominant-path model

States:

- P0: healthy
- P1: one failed disk
- P2: data loss if second failure hits the mirror twin

Dominant data-loss transition:

```text
twin_fail_rate = ((N - 1) * lambda * P1) * (1 / (N - 1)) = lambda * P1
```

Approximate equations:

```text
dP0/dt = mu * P1 - N * lambda * P0
dP1/dt = N * lambda * P0 - mu * P1 - twin_fail_rate
dP2/dt = twin_fail_rate
```

Limitation: this simplified model does not track all possible multi-degraded but still-surviving RAID 10 states, such as two failed disks in different mirror pairs.

### RAID 6 / 4-state model

States:

- P0: healthy
- P1: one disk failed
- P2: two disks failed, still surviving
- P3: data loss after third failure before repair

Equations:

```text
dP0/dt = mu * P1 - N * lambda * P0
dP1/dt = N * lambda * P0 - mu * P1 - (N - 1) * lambda * P1 + mu * P2
dP2/dt = (N - 1) * lambda * P1 - mu * P2 - (N - 2) * lambda * P2
dP3/dt = (N - 2) * lambda * P2
```

RAID 6 is the strongest and most defensible model in the current implementation.

---

## 13. Bit Corruption Argument

The project description gives a severe assumption:

> mean lifetime of a magnetic bit is approximately two years.

This should not be interpreted as the externally visible file-error rate of modern drives because drive-internal ECC and sector remapping already correct many physical-level defects.

Correct interpretation for the report:

- Passive long-term storage is not enough.
- Data must be actively checked.
- Scheduled scrubbing is required.
- RAID rebuild is the critical phase.
- In RAID 5, losing one disk consumes all parity redundancy.
- During rebuild, the remaining data and parity blocks must be read correctly.
- At 300 TB scale, the number of bits read during rebuild is extremely large, so residual URE risk cannot be ignored.
- RAID 6 is preferred because it retains a second parity relation during rebuild.

Avoid overclaiming that RAID 5 failure is mathematically guaranteed unless a full URE model is implemented.

Preferred wording:

> At 300 TB scale, the number of bits read during rebuild is so large that the residual risk of an unrecoverable read error can no longer be ignored. This is the main reason RAID 5 is rejected for the 300 TB tier.

---

## 14. Common-Cause Failure Argument

The Markov model assumes independent disk failures, but the project explicitly asks for common-cause failures.

Common-cause failures include:

- fire
- flooding
- power surge
- site loss
- RAID controller defect
- firmware defect
- bad manufacturing batch
- correlated disk lot failure
- ransomware
- malicious deletion
- administrative error

RAID does not protect against these if all copies fail or are corrupted together.

The report uses a simplified beta-factor estimate:

```text
lambda_CCF = beta * lambda_disk
```

With beta = 5% and guarantee-implied MTBF ≈ 28.5 years:

```text
P(CCF within 30 yr) = 1 - exp(-0.05 * 30 / 28.5) ≈ 5%
```

With manufacturer MTBF, the estimate is around 1%.

Important interpretation:

- This is not a precise site-loss prediction.
- It is an order-of-magnitude/bounding estimate.
- Not every common-cause event causes permanent data loss.
- A recoverable power outage should not be counted the same as fire or malicious deletion.
- The conclusion is that common-cause risk dominates independent disk-failure risk.
- Therefore, independent physically separated backup is more valuable than simply adding more RAID redundancy.

---

## 15. Safety / Dependability Case

Top-level claim:

> The subsystem ensures data preservation and write continuity across all operational tiers without exceeding defined risk tolerances.

Safer formulation:

> For independent disk failures, the selected RAID architectures meet the stated 30-year structural RAID-layer target. For common-cause and human-induced failures, the RAID layer is insufficient by design, so the dependability argument depends on independent backup, restore testing, access control, monitoring, and operational procedures.

Tier arguments:

- Tier 1: RAID 1 + hot spare + daily independent backup.
- Tier 2: RAID 10 + continuous snapshotting.
- Tier 3: RAID 6 + dual parity + independent backup/replication.

Residual risks:

- correlated lot failures
- logistical MTTR extension
- administrative/human error
- malicious software/ransomware
- controller/firmware corruption
- site-level hazards
- backup integrity and restore failure

Mitigations:

- disk batch/vendor diversity
- cold spares
- monitoring
- real-time alerts
- scheduled scrubbing
- UPS
- immutable offsite backup
- restore testing
- RBAC
- MFA for destructive actions

---

## 16. Current Repository Structure

Repository:

```text
https://github.com/harjeetks3/Raid-Markov-Model
```

Known files:

```text
README.md
Makefile
src/raidSim.c
src/setup.c
src/setup.h
src/sim.c
src/sim.h
```

Current code purpose:

- Build a C command-line utility called `raid_sim`.
- Run the three project case-study tiers.
- Numerically integrate Markov equations with Euler's method.
- Print final state probabilities after 30 years.

---

## 17. Current Code Behavior

`src/raidSim.c` runs three hardcoded tiers:

```c
/* Tier 1 - 5 TB Critical Records Vault -> RAID 1 with Hot-Spare (3 disks total) */
tier_configs[0] = base_config;
tier_configs[0].type = RAID1_SPARE;
tier_configs[0].num_disks = 3;

/* Tier 2 - 50 TB Active Production Pool -> RAID 10 (8 disks) */
tier_configs[1] = base_config;
tier_configs[1].type = RAID10;
tier_configs[1].num_disks = 8;

/* Tier 3 - 300 TB Regional Media Vault -> RAID 6 (e.g., 16 disks) */
tier_configs[2] = base_config;
tier_configs[2].type = RAID6;
tier_configs[2].num_disks = 16;
```

Default config in `src/setup.c`:

```c
config->type = RAID1_SPARE;
config->mtbf = 1200000.0; // 1.2M hours
config->mttr = 24.0;      // 1 day
config->num_disks = 3;
config->max_time = 30.0 * HOURS_PER_YEAR;
config->dt = 0.1;
```

The parser accepts:

- `--disks`, `-d`
- `--years`, `-y`
- `--mtbf`
- `--mttr`
- `--dt`

But note: `--disks` is currently misleading in case-study mode because `raidSim.c` overwrites `num_disks` for each hardcoded tier.

---

## 18. Known Repo / Code Issues to Fix

### Must-fix or document clearly

1. README currently may describe the model as only a 3-state model. This is incomplete because RAID 6 uses a fourth state, P3, for data loss.
2. README documents `--disks`, but the program overwrites disk count in the three case-study tiers. Either document this clearly or change the program to support `--type` / `--tier` single-run mode.
3. README should include both manufacturer-MTBF and guarantee-implied MTBF sample commands/results.
4. README should explain that the model covers independent disk failures only.
5. README should explain excluded risks: bit corruption, common-cause failures, backups, controller failures, site failures, ransomware, and human error.
6. README should include the actual Markov equations or at least state definitions.
7. Appendix and GitHub code must stay consistent.

### Good next coding improvements

Possible improved CLI design:

```bash
./raid_sim --case-study
./raid_sim --tier raid1_spare --disks 3 --mtbf 1200000 --mttr 24 --years 30
./raid_sim --tier raid10 --disks 8 --mtbf 1200000 --mttr 24 --years 30
./raid_sim --tier raid6 --disks 16 --mtbf 1200000 --mttr 24 --years 30
./raid_sim --case-study --mtbf 249700
```

Possible enum/string mapping:

```text
raid1_spare
raid10
raid6
```

Possible README clarification:

> By default, the program runs the three project case-study tiers. The `--mtbf`, `--mttr`, `--years`, and `--dt` options affect all tiers. Disk count is fixed per tier in case-study mode. To run arbitrary arrays, use single-tier mode.

If not implementing single-tier mode, remove or downplay `--disks` from README.

---

## 19. Important Report-Code Consistency Rules

Do not change code outputs without updating the report.

The report currently depends on these values:

```text
RAID 1 + hot spare, 3 disks, MTBF 1.2e6 h: 0.002629% data loss
RAID 10, 8 disks, MTBF 1.2e6 h: 0.003505% data loss
RAID 6, 16 disks, MTBF 1.2e6 h: 0.000029% data loss
```

And guarantee-implied approximate results:

```text
RAID 1 + hot spare, 3 disks: 0.061%
RAID 10, 8 disks: 0.081%
RAID 6, 16 disks: 0.0033%
```

If equations, timestep, transition logic, disk counts, MTBF, MTTR, or state models change, regenerate all outputs and update the report.

---

## 20. Current High-Priority Tasks for Codex

When Codex works on this repo, prioritize:

1. Update README to reflect 4-state RAID 6 model.
2. Clarify default case-study mode.
3. Fix or document the misleading `--disks` option.
4. Add sample output for both MTBF cases.
5. Add model assumptions and limitations.
6. Add equations in README.
7. Optionally implement single-tier mode with `--type` / `--tier`.
8. Ensure code compiles with `make` and `-Wall -Wextra -std=c99`.
9. Keep report numbers reproducible.
10. Do not overclaim safety or total real-world data-loss probability.

---

## 21. What Not To Do

Do not:

- Claim the model proves total system safety.
- Claim the model includes backup reliability unless explicitly implemented.
- Claim the model includes bit corruption unless explicitly implemented.
- Claim the model includes common-cause failures unless explicitly implemented.
- Change the default outputs without updating the report.
- Hide model limitations.
- Treat the beta-factor estimate as a precise prediction.
- Treat Wikipedia as a core technical source; it is used only for images in the report.
- Present RAID as a backup replacement. RAID is not a backup.

---

## 22. Preferred Final Messaging

The strongest final thesis of the project is:

> The selected RAID architectures meet the 30-year structural RAID-layer data-loss target for independent disk failures. However, RAID alone is not sufficient for dependable long-term storage. The residual risk is dominated by common-cause failures, human error, malicious deletion, site-level events, and backup integrity. Therefore, the dependable storage architecture must combine RAID with monitoring, hot-swap maintenance, scheduled scrubbing, independent physically separated backup, restore testing, access control, and operational procedures.

---

## 23. Current Submission Readiness

The report is close to submission-ready after final wording and GitHub consistency updates.

Remaining report polish:

- refine NFR-1 scope to independent structural RAID-layer data loss
- use "Netflix-like" if avoiding claims about real Netflix systems
- keep "disk" spelling consistent
- avoid overconfident wording around common-cause estimates
- avoid claiming full FTA if only using fault-tree-style/FMEA-style tables

Remaining repo work:

- update README
- document model states properly
- document case-study mode
- add guarantee-implied MTBF sample command/output
- optionally improve CLI

