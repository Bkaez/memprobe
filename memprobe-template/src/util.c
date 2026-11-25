/* Provided. Aligned allocation and CSV helpers. */
#include "memprobe.h"
#include <stdlib.h>

void *alloc_aligned(size_t alignment, size_t size) {
    if (size % alignment) size += alignment - (size % alignment);
#if defined(_WIN32)
    return _aligned_malloc(size, alignment);
#else
    return aligned_alloc(alignment, size);
#endif
}

void free_aligned(void *p) {
#if defined(_WIN32)
    _aligned_free(p);
#else
    free(p);
#endif
}

FILE *open_csv(const char *path, const char *header) {
    if (!path) return NULL;
    FILE *f = fopen(path, "w");
    if (!f) { perror(path); return NULL; }
    fprintf(f, "%s\n", header);
    return f;
}
