/* Provided. Command dispatch. Nothing here needs changing. */
#include "memprobe.h"
#include <string.h>
#include <stdlib.h>

static const char *flag(int argc, char **argv, const char *name) {
    for (int i = 2; i + 1 < argc; i++)
        if (!strcmp(argv[i], name)) return argv[i + 1];
    return NULL;
}

static void usage(void) {
    fprintf(stderr,
      "usage: memprobe <command> [--csv PATH]\n"
      "  line     cache line size, by stride sweep\n"
      "  levels   cache capacities and latencies, by working-set sweep\n"
      "  verify   what the operating system reports\n"
      "  sizes    type sizes on this machine\n");
}

int main(int argc, char **argv) {
    if (argc < 2) { usage(); return 2; }
    pin_to_first_core();
    const char *csv = flag(argc, argv, "--csv");

    if (!strcmp(argv[1], "line"))   return cmd_line(csv);
    if (!strcmp(argv[1], "levels")) return cmd_levels(csv);
    if (!strcmp(argv[1], "verify")) return cmd_verify();
    if (!strcmp(argv[1], "sizes")) {
        printf("char=%zu short=%zu int=%zu long=%zu ptr=%zu\n",
               sizeof(char), sizeof(short), sizeof(int),
               sizeof(long), sizeof(void *));
        return 0;
    }
    usage();
    return 2;
}
