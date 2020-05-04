#include <stdio.h>
#include <stdlib.h>
#include <smbios_c/smi.h>
#include <getopt.h>
#include <ctype.h>
#include <string.h>

#define    PRINT_THERMAL_ZONE      0
#define    RETURN_THERMAL_ZONE     1
#define    PRINT_ALL_THERMAL_ZONE    2


const u16 smiClass = 17;
const u16 Select = 19;

const char *help_message[] = {
        "Usage: smbios-thermal-c [OPTION]\n",
        "Options:",
        "   -h, --help                          show this help message and exit",
        "   -i, --info                          This will Display the Supported Features of USTT",
        "   -g, --get-thermal-info              This will Display the Thermal Information of a system",
        "   -s, --set-thermal-mode=THERMAL_MODE Option to set Thermal Mode; balanced, cool-bottom, quiet, performance",
        NULL
};

bool is_set(u32 num, u32 pos) {
    u32 mask = 1u << pos;

    if (num & mask) {
        return true;
    }

    return false;
}

char *toLower(char *s) {
    for (char *p = s; *p; p++) *p = tolower(*p);
    return s;
}

void printThermalZone(u32 thermal_zone) {
    if (is_set(thermal_zone, 0)) fprintf(stdout, "Balanced\n");
    if (is_set(thermal_zone, 1)) fprintf(stdout, "Cool Bottom\n");
    if (is_set(thermal_zone, 2)) fprintf(stdout, "Quiet\n");
    if (is_set(thermal_zone, 3)) fprintf(stdout, "Performance\n");
}

u32 getThermalZoneByString(char *thermal_zone) {
    if (strcmp(thermal_zone, "balanced") == 0) return 1;
    if (strcmp(thermal_zone, "cool-bottom") == 0) return 1u << 1u;
    if (strcmp(thermal_zone, "quiet") == 0) return 1u << 2u;
    if (strcmp(thermal_zone, "performance") == 0) return 1u << 3u;

    printf("Invalid thermal mode option specified: %s\nSupported modes: balanced, cool-bottom, quiet, performance\n", thermal_zone);
    exit(EXIT_FAILURE);
}

u32 getThermalZone(int flag) {
    u32 res[4];
    u32 args[4] = {0};

    dell_simple_ci_smi(smiClass, Select, args, res);

    if (res[cbRES1] != 0) {
        fprintf(stderr, "Info: Unable to Get Thermal Information on this system\n");
        return EXIT_FAILURE;
    }

    switch (flag) {
        case RETURN_THERMAL_ZONE:
            return res[cbRES3];
        case PRINT_THERMAL_ZONE:
            printThermalZone(res[cbRES3]);
            break;
        default:
            printThermalZone(res[cbRES2]);

    }

    return EXIT_SUCCESS;
}

int setThermalZone(char *thermal_mode) {
    u32 res[4];
    u32 args[4] = {1};
    u32 var_thermal_mode = getThermalZoneByString(toLower(thermal_mode));

    args[cbARG2] = getThermalZone(RETURN_THERMAL_ZONE) << 8u | var_thermal_mode;

    dell_simple_ci_smi(smiClass, Select, args, res);

    if (res[cbRES1] != 0) {
        fprintf(stderr, "Info: Set Thermal Information failed\n");
        return EXIT_FAILURE;
    }
    fprintf(stdout, "Thermal Information Set successfully to: %s\n", thermal_mode);

    return EXIT_SUCCESS;
}

void print_usage() {
    const char **p = help_message;

    while (NULL != *p)
        printf("%s\n", *p++);
}

int main(int argc, char **argv) {
    int opt = 0;
    int long_index = 0;
    char *thermal_mode;

    static struct option long_options[] = {
            {"get-thermal-info", no_argument,       0, 'g'},
            {"set-thermal-mode", required_argument, 0, 's'},
            {"info",             no_argument,       0, 'i'},
            {"help",             no_argument,       0, 'h'},
            {0, 0,                                  0, 0}
    };

    while ((opt = getopt_long(argc, argv, "s:gih", long_options, &long_index)) != -1) {
        switch (opt) {
            case 'g' :
                getThermalZone(PRINT_THERMAL_ZONE);
                break;
            case 'i' :
                getThermalZone(PRINT_ALL_THERMAL_ZONE);
                break;
            case 's' :
                thermal_mode = optarg;
                setThermalZone(thermal_mode);
                break;
            case 'h':
                print_usage();
                exit(EXIT_SUCCESS);
            default:
                print_usage();
                exit(EXIT_FAILURE);
        }
    }

    if (argc == 1) print_usage();


    return EXIT_SUCCESS;
}