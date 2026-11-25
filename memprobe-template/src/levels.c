/* ===========================================================================
 *  levels.c -- cache capacities and latencies.  YOU IMPLEMENT cmd_levels.
 *
 *  IDEA: fix the node spacing at one cache line. Sweep the WORKING SET from
 *  about 1 KiB up to 256 MiB, multiplying by 1.25 each step (doubling is too
 *  coarse to locate a 32 KiB L1). Plot ns per access against working set.
 *
 *  WHAT YOU EXPECT TO SEE: flat PLATEAUS where the working set still fits in
 *  a level, separated by KNEES where it stops fitting. Plateau height is
 *  that level's latency; knee position is roughly that level's capacity.
 *
 *  A REAL HIERARCHY SPANS A LARGE RANGE -- roughly 1 ns in L1 up to 80-100 ns
 *  in DRAM. If your fastest and slowest differ by less than about 3x, the
 *  prefetcher is still winning and your chain is probably not random.
 *
 *  WRITE:  working_set_bytes,ns_per_access,cycles_per_access,reps
 *  (cycles = ns * assumed_GHz; state the frequency you assumed in REPORT.md)
 * ======================================================================== */
#include "memprobe.h"
#include <stdlib.h>

int cmd_levels(const char *csv_path) {
    FILE *csv = open_csv(csv_path,
        "working_set_bytes,ns_per_access,cycles_per_access,reps");
    if (csv_path && !csv) return 1;

    /* TODO: for ws = 1024; ws <= 256MiB; ws *= 1.25 ... */
    (void)csv;

    fprintf(stderr, "cmd_levels: not implemented yet\n");
    if (csv) fclose(csv);
    return 1;
}
