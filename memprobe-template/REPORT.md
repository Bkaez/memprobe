# memprobe: Measuring Memory Hierarchies Across Three Processor Architectures

**Your name** · CS 321 · Fall 2026 · [repository link]

> Delete these instructions as you write. Every number you state must come
> from a file committed under `data/`. Aim for 1500–2500 words.

## 1. What I measured and how

Describe the two measurements (stride sweep, working-set sweep). Explain
*why* the pointer chain is random and *why* you take the minimum rather than
the mean. Say how many runs you collected and when.

## 2. Results

For each architecture, include the figure and state what you found.

![Comparison](site/plots/comparison.png)

| Architecture | Line size | First knee | Fastest | Slowest |
|---|---|---|---|---|
| x86_64-linux | | | | |
| arm64-linux  | | | | |
| arm64-apple  | | | | |

## 3. Comparison

Which hierarchy is deeper? Which has the lowest latency at each level? Where
do the three differ in a way that reflects a genuine design decision rather
than measurement noise? Argue from your numbers.

## 4. Average memory access time

Compute AMAT from your measured latencies with a stated hit-rate assumption
(Null §6.4). Say how sensitive the answer is to that assumption.

## 5. What these measurements cannot support

**This section carries real marks.** These are shared virtual machines.
Which of your conclusions are solid, which are suggestive, and which would
need hardware you do not have? Use your own shaded bands as evidence.

## 6. References

Null; CS:APP; Sattolo's algorithm; anything else you consulted.
