# memprobe

Measures the memory hierarchy of the machine it runs on: cache line size,
cache level capacities, and access latency as the working set grows.

**I did not run this on my own computer.** All measurements come from GitHub
Actions runners across three processor architectures. See `REPORT.md` for the
analysis and `data/` for the raw results.

## How to reproduce

Nothing to install. Fork this repository (keep it public), then:

1. **Actions → measure → Run workflow** — builds and measures on x86-64,
   Arm64 Linux and Apple Silicon, then commits results to `data/`.
2. **Actions → analyze → Run workflow** — regenerates `site/plots/` from
   everything in `data/`.

## What I wrote

`src/chase.c` — the pointer-chain builder and the timed loop.
Everything else came with the template.

<!-- TODO: cite Sattolo's algorithm; disclose any AI use per the syllabus. -->
