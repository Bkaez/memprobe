/* ===========================================================================
 *  chase.c  --  THE CORE OF THE PROJECT. THIS FILE IS YOURS TO WRITE.
 *
 *  Two functions. Everything else in the repository already works.
 * ======================================================================== */
#include "memprobe.h"
#include <stdlib.h>

/* -----------------------------------------------------------------------
 *  TODO 1 -- build_chain
 *
 *  Turn `buf` into ONE cycle of pointers that visits all n_nodes exactly
 *  once, in RANDOM order. Node i lives at (char*)buf + i*stride.
 *  Each node stores the address of the next node.
 *
 *  WHY RANDOM: a sequential chain is a constant stride, which is exactly
 *  what a hardware prefetcher recognises. It will fetch ahead, hide the
 *  misses, and your latency curve will come out flat and useless.
 *
 *  WHY ONE CYCLE: several short cycles would revisit the same few lines
 *  forever. They stay in L1 no matter how big the buffer is, so every
 *  level looks like L1.
 *
 *  HOW: Sattolo's algorithm. Look it up, understand it, cite it in your
 *  README. It differs from Fisher-Yates by one character, and that one
 *  character is what guarantees a single cycle instead of a permutation.
 *
 *  RETURNS: a pointer to any node in the cycle (the starting point).
 * -------------------------------------------------------------------- */
void *build_chain(void *buf, size_t n_nodes, size_t stride) {
    (void)buf; (void)n_nodes; (void)stride;
    return NULL;   /* <-- replace */
}

/* -----------------------------------------------------------------------
 *  TODO 2 -- chase_ns_per_access
 *
 *  Walk the cycle n_accesses times and return nanoseconds per access.
 *
 *      while (n--) { p = *(void **)p; }
 *
 *  WHY THIS LOOP: each load depends on the previous one, so the CPU
 *  cannot overlap them. You measure true latency, not throughput.
 *
 *  THREE THINGS THAT WILL BITE YOU:
 *
 *  1. WARM UP. Touch every node once before timing, or your first pass
 *     measures page faults instead of cache misses.
 *
 *  2. STOP THE OPTIMISER. At -O2 a loop whose result is unused is dead
 *     code and will be deleted. You will then measure an empty loop and
 *     report an L1 latency of about 0.05 ns, which is impossible. Use:
 *
 *         __asm__ volatile("" : : "r"(p) : "memory");
 *
 *     after the loop, so the final pointer is considered "used".
 *     (The CI data check rejects anything under 0.2 ns for this reason.)
 *
 *  3. TAKE THE MINIMUM, NOT THE MEAN. Repeat the whole measurement at
 *     least 5 times and return the SMALLEST result. Interrupts and
 *     scheduling only ever ADD time, so the minimum is the closest you
 *     get to the true cost. An average measures the noise too.
 * -------------------------------------------------------------------- */
double chase_ns_per_access(void *start, size_t n_accesses) {
    (void)start; (void)n_accesses;
    return 0.0;   /* <-- replace */
}
