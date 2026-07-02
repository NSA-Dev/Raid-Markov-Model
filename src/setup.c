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
        if (strcmp(argv[i], "--help") == 0 || strcmp(strcmp(argv[i], "-h") == 0)) {
            print_usage(argv[0]);
            return;  
        }
        
        if (strcmp(argv[i], "--disks") == 0 || strcmp(argv[i], "-d") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "Error: Option %s requires an argument.\n", argv[i]);
                return;
            }
            disk_num = strtol(argv[++i], &endptr, 10);
            if (*endptr != '\0' || disk_num < 2) return;
            config->num_disks = (int)disk_num;
        } 
        else if (strcmp(argv[i], "--years") == 0 || strcmp(argv[i], "-y") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "Error: Option %s requires an argument.\n", argv[i]);
                return;
            }
            time = strtod(argv[++i], &endptr);
            if (*endptr != '\0' || time <= 0.0) return;
            config->max_time = time * HOURS_PER_YEAR;
        } 
        else if (strcmp(argv[i], "--mtbf") == 0) {
            if (i + 1 >= argc) return;
            time = strtod(argv[++i], &endptr);
            if (*endptr != '\0' || time <= 0.0) return;
            config->mtbf = time;
        } 
        else if (strcmp(argv[i], "--mttr") == 0) {
            if (i + 1 >= argc) return;
            time = strtod(argv[++i], &endptr);
            if (*endptr != '\0' || time <= 0.0) return;
            config->mttr = time;
        } 
        else if (strcmp(argv[i], "--dt") == 0) {
            if (i + 1 >= argc) return;
            time = strtod(argv[++i], &endptr);
            if (*endptr != '\0' || time <= 0.0) return;
            config->dt = time;
        } 
        else {
            fprintf(stderr, "raid_sim: unrecognized option '%s'\n", argv[i]);
            fprintf(stderr, "Try '%s --help' or '%s -h' for more information.\n", argv[0], argv[0]);
            return; 
        }
    }
    *result = true; 
}

void print_usage(const char *program_name) {
    fprintf(stderr, "RAID_SIM(1)                 User Commands                 RAID_SIM(1)\n\n");
    
    fprintf(stderr, "NAME\n");
    fprintf(stderr, "       %s - Markov chain simulation for RAID reliability analysis\n\n", program_name);
    
    fprintf(stderr, "SYNOPSIS\n");
    fprintf(stderr, "       %s [OPTIONS]\n\n", program_name);
    
    fprintf(stderr, "DESCRIPTION\n");
    fprintf(stderr, "       %s models data loss probabilities for a disk storage array\n", program_name);
    fprintf(stderr, "       over a given time horizon using Euler numerical integration.\n\n");
    
    fprintf(stderr, "OPTIONS\n");
    fprintf(stderr, "       -d, --disks NUM\n");
    fprintf(stderr, "              Number of disks in the array (Integer >= 2). Default: 3.\n\n");
    
    fprintf(stderr, "       -y, --years NUM\n");
    fprintf(stderr, "              Simulation timeline length in years (Float > 0). Default: 30.0.\n\n");
    
    fprintf(stderr, "       --mtbf NUM\n");
    fprintf(stderr, "              Mean Time Between Failures in hours (Float > 0).\n");
    fprintf(stderr, "              Default: 1200000.0.\n\n");
    
    fprintf(stderr, "       --mttr NUM\n");
    fprintf(stderr, "              Mean Time To Repair in hours (Float > 0). Default: 24.0.\n\n");
    
    fprintf(stderr, "       --dt NUM\n");
    fprintf(stderr, "              Mathematical delta step size (Float > 0). Default: 0.1.\n\n");
    
    fprintf(stderr, "EXAMPLES\n");
    fprintf(stderr, "       Simulate a 5-disk array over 10 years with an explicit 12.5 hour MTTR:\n\n");
    fprintf(stderr, "              %s -d 5 -y 10 --mttr 12.5\n\n", program_name);
}