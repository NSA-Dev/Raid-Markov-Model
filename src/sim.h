#ifndef SIM_H
#define SIM_H

#include "setup.h"

/* Initial State Probabilities at Time = 0 */ 
#define INIT_P0 1.0
#define INIT_P1 0.0
#define INIT_P2 0.0
#define INIT_P3 0.0

typedef struct {
    double p_healthy;
    double p_degraded;
    double p_degraded_2; /* Only used specifically for RAID 6 secondary degradation */
    double p_dataloss;
} SimResult;



void run_raid_simulation(const RaidConfig *config, SimResult* res);
void print_simulation_report(const RaidConfig *config, 
                                const SimResult *results);


#endif 