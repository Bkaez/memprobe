/* ===========================================================================
 *  line.c -- cache line size by stride sweep.   YOU IMPLEMENT cmd_line.
 *
 *  IDEA: allocate a buffer bigger than the last-level cache. Chase it with
 *  node spacing (stride) of 4, 8, 16 ... 1024 bytes. While the stride is
 *  SMALLER than a cache line, several nodes share one line and one miss, so
 *  the cost per access is low. Once the stride reaches the line size, every
 *  access needs its own line. The curve flattens there.
 *
 *  WHAT YOU EXPECT TO SEE: a rise that levels off at 64 bytes on x86-64 and
 *  on most Arm server parts, and at 128 bytes on Apple Silicon. Do not
 *  assume -- measure it, then compare with what `memprobe verify` reports.
 *
 *  WRITE one CSV row per stride:   stride_bytes,ns_per_access,reps
 * ======================================================================== */
#include "memprobe.h"
#include <stdlib.h>

int cmd_line(const char *csv_path) {
    FILE *csv = open_csv(csv_path, "stride_bytes,ns_per_access,reps");
    if (csv_path && !csv) return 1;

    /* TODO: for stride in 4,8,...,1024 -- allocate, build_chain,
     *       chase_ns_per_access, print and write a row.           */
    (void)csv;

    fprintf(stderr, "cmd_line: not implemented yet\n");
    if (csv) fclose(csv);
    return 1;
}
