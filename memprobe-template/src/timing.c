/* Provided. A monotonic clock and a request to stay on one core.
 * You do not need to modify this file. */
#define _GNU_SOURCE
#include "memprobe.h"
#include <time.h>
#if defined(__linux__)
#  include <sched.h>
#endif

double now_seconds(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec * 1e-9;
}

void pin_to_first_core(void) {
#if defined(__linux__)
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(0, &set);
    sched_setaffinity(0, sizeof(set), &set);   /* best effort */
#endif
}
