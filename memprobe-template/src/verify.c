/* Provided. Asks the operating system what the cache hierarchy is, so you
 * have ground truth to compare your measurements against. */
#include "memprobe.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static long read_long(const char *path) {
    FILE *f = fopen(path, "r");
    if (!f) return -1;
    char b[64] = {0};
    if (!fgets(b, sizeof b, f)) { fclose(f); return -1; }
    fclose(f);
    long v = strtol(b, NULL, 10);
    for (char *p = b; *p; p++) {
        if (*p == 'K') return v * 1024;
        if (*p == 'M') return v * 1024 * 1024;
    }
    return v;
}

int cmd_verify(void) {
    printf("%-10s %-10s %-12s %-10s\n", "LEVEL", "TYPE", "SIZE(B)", "LINE(B)");
    int found = 0;
    for (int i = 0; i < 10; i++) {
        char p[256], t[64] = {0};
        snprintf(p, sizeof p, "/sys/devices/system/cpu/cpu0/cache/index%d/level", i);
        long level = read_long(p);
        if (level < 0) continue;
        snprintf(p, sizeof p, "/sys/devices/system/cpu/cpu0/cache/index%d/type", i);
        FILE *f = fopen(p, "r");
        if (f) { if (fgets(t, sizeof t, f)) t[strcspn(t, "\n")] = 0; fclose(f); }
        snprintf(p, sizeof p, "/sys/devices/system/cpu/cpu0/cache/index%d/size", i);
        long size = read_long(p);
        snprintf(p, sizeof p, "/sys/devices/system/cpu/cpu0/cache/index%d/coherency_line_size", i);
        long line = read_long(p);
        printf("L%-9ld %-10s %-12ld %-10ld\n", level, t, size, line);
        found = 1;
    }
    if (!found)
        printf("(no sysfs cache data; on macOS run: sysctl -a | grep cachesize)\n");
    return 0;
}
