#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "setup.h"
#include "sim.h"

int main(int argc, char *argv[]) {
    RaidConfig tier_configs[3]; /* 0 - RAID 1 | 1 - RAID 10 | 2 - RAID 6 */
    RaidConfig base_config;
    SimResult results;
    bool is_valid;
    int i;

    get_default_config(&base_config);
    parse_arguments(argc, argv, &base_config, &is_valid);
    if (!is_valid) {
        return EXIT_FAILURE;
    }

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

    for(i = 0; i < 3; i++) {
        run_raid_simulation(&tier_configs[i], &results);
        print_simulation_report(&tier_configs[i], &results); 

    }


    return EXIT_SUCCESS;
}