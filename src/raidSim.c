#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "setup.h"
#include "sim.h"

int main(int argc, char *argv[]) {
    RaidConfig config;
    SimResult results;
    bool is_valid;

    get_default_config(&config);
    parse_arguments(argc, argv, &config, &is_valid);
    if (!is_valid) {
        return EXIT_FAILURE;
    }

    run_raid_simulation(&config, &results);
    print_simulation_report(&config, &results);

    return EXIT_SUCCESS;
}