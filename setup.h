#ifndef SETUP_H
#define SETUP_H

#include <stdbool.h>
#define HOURS_PER_YEAR 8766.0

typedef struct {
    double mtbf;
    double mttr;
    int num_disks;
    double max_time;
    double dt;
} RaidConfig;

RaidConfig get_default_config(void);
void parse_arguments(int argc, char *argv[], RaidConfig *config, bool *result); 


#endif 