#include "sim.h"
#include <stdio.h>

void run_raid_simulation(const RaidConfig *config, SimResult* res) {
    double lambda, mu, dt, current_time;
    double P0, P1, P2; 
    double dP0, dP1, dP2;
    int N;
    /* Used for RAID6 calculations */
    double P3;
    double dP3;

    if(!config || !res) return; 

    lambda = 1.0 / config->mtbf;
    mu  = 1.0 / config->mttr;
    N   = config->num_disks;
    dt  = config->dt;
    current_time = 0.0;

    P0 = INIT_P0; 
    P1 = INIT_P1; 
    P2 = INIT_P2;
    P3 = INIT_P3; 
    

    dP3 = 0.0; 

    /*  Numerical Integration Loop using Euler's Method */
    while (current_time < config->max_time) {
        switch(config->type) {
            
            case RAID1_SPARE:
                /* RAID 1 with a Hot-Spare handles the first failure instantly. 
                   Conservative 3-state approximation where any subsequent failure causes loss. */
                dP0 = (mu * P1) - (N * lambda * P0);
                dP1 = (N * lambda * P0) - (mu * P1) - ((N - 1) * lambda * P1);
                dP2 = ((N - 1) * lambda * P1);
                
                P0 += dP0 * dt;
                P1 += dP1 * dt;
                P2 += dP2 * dt;
                break;

            case RAID10:
                /* RAID 10 consists of mirrored pairs. 
                   A 2nd failure only causes data loss if it hits the exact mirror twin (1 / (N-1) chance). */
                {
                    double twin_fail_rate = ((double)(N - 1) * lambda * P1) * (1.0 / (N - 1));
                    
                    dP0 = (mu * P1) - (N * lambda * P0);
                    dP1 = (N * lambda * P0) - (mu * P1) - twin_fail_rate;
                    dP2 = twin_fail_rate;
                    
                    P0 += dP0 * dt;
                    P1 += dP1 * dt;
                    P2 += dP2 * dt;
                }
                break;

            case RAID6:
                /* RAID 6 transitions through an extra survival state (P2 = 2 drives down) 
                   before entering complete data loss (P3 = 3 drives down). */
                dP0 = (mu * P1) - (N * lambda * P0);
                dP1 = (N * lambda * P0) - (mu * P1) - ((N - 1) * lambda * P1) + (mu * P2); 
                dP2 = ((N - 1) * lambda * P1) - (mu * P2) - ((N - 2) * lambda * P2);
                dP3 = ((N - 2) * lambda * P2);
                
                P0 += dP0 * dt;
                P1 += dP1 * dt;
                P2 += dP2 * dt;
                P3 += dP3 * dt;
                break;
        }

        current_time += dt;
    }
    res->p_healthy  = P0; 
    res->p_degraded = P1; 
    if (config->type == RAID6) {
        res->p_degraded_2 = P2;
        res->p_dataloss   = P3;
    } else {
        res->p_degraded_2 = 0.0;
        res->p_dataloss   = P2;
    }
}

void print_simulation_report(const RaidConfig *config, const SimResult *results) {
    const char* type_str = "Unknown";
    switch(config->type) {
        case RAID1_SPARE: type_str = "RAID 1 (+ Hot-Spare)"; break;
        case RAID10:      type_str = "RAID 10"; break;
        case RAID6:       type_str = "RAID 6"; break;
    }
    printf("=== 30-Year Storage Reliability Verification ===\n");
    printf("Array Configuration: %d-disk %s array\n", config->num_disks, type_str);
    printf("Final State Probabilities after %.1f Hours:\n", config->max_time);
    printf("  Healthy (P0):            %.6f%%\n", results->p_healthy * 100.0);
    printf("  Degraded/At Risk (P1):   %.6f%%\n", results->p_degraded * 100.0);
    printf("  Permanent Data Loss:     %.6f%%\n", results->p_dataloss * 100.0);
    printf("================================================\n");
}