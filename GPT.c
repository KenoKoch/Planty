#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int get_status(const char* status) {
    FILE* f = fopen("/home/keno/Desktop/data/state.txt", "r");
    if (!f) return 0;
    char line[64];
    int value = 0;
    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, status, strlen(status)) == 0) {
            value = atoi(line + strlen(status) + 1);
            break;
        }
    }
    fclose(f);
    return value;
}