#include <stdio.h>
#include <math.h>



int main() {

    verify_args();
    calculate();
    printResults(); 

    // Operational Parameters
    double mtbf = 1200000.0;       // Hours
    double mttr = 24.0;            // Hours
    int N = 3;                     // Number of disks in array
    double T_max = 262800.0;       // 30 years in hours

    // Calculate transition rates
    double lambda = 1.0 / mtbf;
    double mu = 1.0 / mttr;

    // Initial State Probabilities at Time = 0
    double P0 = 1.0;  // 100% chance system starts completely healthy
    double P1 = 0.0;  // 0% chance degraded
    double P2 = 0.0;  // 0% chance data loss (Absorbing State)

    // Simulation step configuration
    double dt = 0.1;  // Step size in hours (smaller = more precise)
    double current_time = 0.0;

    // Numerical integration loop (Euler's Method)
    while (current_time < T_max) {
        // Calculate rate of change (derivatives) for each state
        double dP0 = (mu * P1) - (N * lambda * P0);
        double dP1 = (N * lambda * P0) - (mu * P1) - ((N - 1) * lambda * P1);
        double dP2 = ((N - 1) * lambda * P1);

        // Update state probabilities by moving forward by step dt
        P0 += dP0 * dt;
        P1 += dP1 * dt;
        P2 += dP2 * dt;  // Cumulative probability of entering data loss

        current_time += dt;
    }

    // Print Results
    printf("=== 30-Year Storage Reliability Verification ===\n");
    printf("Array Configuration: %d-disk RAID array\n", N);
    printf("Final State Probabilities after 30 Years:\n");
    printf("  Healthy (P0):   %.6f%%\n", P0 * 100.0);
    printf("  Degraded (P1):  %.6f%%\n", P1 * 100.0);
    printf("  Data Loss (P2): %.6f%%\n", P2 * 100.0);
    printf("================================================\n");

    return 0;
}