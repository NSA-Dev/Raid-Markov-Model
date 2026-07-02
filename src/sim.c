#include "sim.h"
#include <stdio.h>

void run_raid_simulation(const RaidConfig *config, SimResult* res) {
    double lambda, mu, dt, current_time;
    double P0, P1, P2; 
    double dP0, dP1, dP2;
    int N;
    
    if(!config || !res) return; 

    lambda = 1.0 / config->mtbf;
    mu  = 1.0 / config->mttr;
    N   = config->num_disks;
    dt  = config->dt;
    current_time = 0.0;

    P0 = INIT_P0; 
    P1 = INIT_P1; 
    P2 = INIT_P2; 

    /*  Numerical Integration Loop using Euler's Method */
    while (current_time < config->max_time) {
        /* Evaluate state derivatives */
        dP0 = (mu * P1) - (N * lambda * P0);
        dP1 = (N * lambda * P0) - (mu * P1) - ((N - 1) * lambda * P1);
        dP2 = ((N - 1) * lambda * P1);

        /* Step forward via integration step delta */
        P0 += dP0 * dt;
        P1 += dP1 * dt;
        P2 += dP2 * dt; 

        current_time += dt;
    }

    /* 5. Assign Output Results Matrix */
    res->p_healthy  = P0; 
    res->p_degraded = P1; 
    res->p_dataloss = P2;
}

void print_simulation_report(const RaidConfig *config, const SimResult *results) {
    printf("=== 30-Year Storage Reliability Verification ===\n");
    printf("Array Configuration: %d-disk RAID array\n", config->num_disks);
    printf("Final State Probabilities after %.1f Hours:\n", config->max_time);
    printf("  Healthy (P0):   %.6f%%\n", results->p_healthy * 100.0);
    printf("  Degraded (P1):  %.6f%%\n", results->p_degraded * 100.0);
    printf("  Data Loss (P2): %.6f%%\n", results->p_dataloss * 100.0);
    printf("================================================\n");
}