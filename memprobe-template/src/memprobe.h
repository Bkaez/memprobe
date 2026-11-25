#ifndef MEMPROBE_H
#define MEMPROBE_H
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

/* timing.c -- provided, nothing to change here */
double now_seconds(void);
void   pin_to_first_core(void);

/* chase.c -- YOU IMPLEMENT */
void  *build_chain(void *buf, size_t n_nodes, size_t stride);
double chase_ns_per_access(void *start, size_t n_accesses);

/* subcommands */
int cmd_line(const char *csv_path);
int cmd_levels(const char *csv_path);
int cmd_verify(void);

/* util.c -- provided */
void *alloc_aligned(size_t alignment, size_t size);
void  free_aligned(void *p);
FILE *open_csv(const char *path, const char *header);
#endif
