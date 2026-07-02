#include "setup.h"
#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <errno.h>

void get_default_config(RaidConfig *config) {
    if(!config) return; 
    
    config->mtbf = 1200000.0; // 1.2M Hours
    config->mttr = 24.0;      // 1 Day
    config->num_disks = 3;    // Minimum RAID-5 array
    config->max_time = 30.0 * HOURS_PER_YEAR; // 30 Years default
    config->dt = 0.1;           // 6-minute simulation steps
   
}


void parse_arguments(int argc, char *argv[], RaidConfig *config, bool *result) {
    char *endptr;
    long disk_num;
    double time;
    int i;  
    
    *result = false; 

    for (i = 1; i < argc; i++) {
        
        if (strcmp(argv[i], "--disks") == 0 || strcmp(argv[i], "-d") == 0) {
            if (i + 1 >= argc) { *result = false; return; }
            disk_num = strtol(argv[++i], &endptr, 10);
            if (*endptr != '\0' || disk_num < 2) { *result = false; return; }
            config->num_disks = (int)disk_num;
        } 
        else if (strcmp(argv[i], "--years") == 0 || strcmp(argv[i], "-y") == 0) {
            if (i + 1 >= argc) { *result = false; return; }
            time = strtod(argv[++i], &endptr);
            if (*endptr != '\0' || time <= 0.0) { *result = false; return; }
            config->max_time = time * HOURS_PER_YEAR;
        } 
        else if (strcmp(argv[i], "--mtbf") == 0) {
            if (i + 1 >= argc) { *result = false; return; }
            time = strtod(argv[++i], &endptr);
            if (*endptr != '\0' || time <= 0.0) { *result = false; return; }
            config->mtbf = time;
        } 
        else if (strcmp(argv[i], "--mttr") == 0) {
            if (i + 1 >= argc) { *result = false; return; }
            time = strtod(argv[++i], &endptr);
            if (*endptr != '\0' || time <= 0.0) { *result = false; return; }
            config->mttr = time;
        } 
        else if (strcmp(argv[i], "--dt") == 0) {
            if (i + 1 >= argc) { *result = false; return; }
            time = strtod(argv[++i], &endptr);
            if (*endptr != '\0' || time <= 0.0) { *result = false; return; }
            config->dt = time;
        } 
        else {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            *result = false;
            return; 
        }
    }
    *result = true;
}