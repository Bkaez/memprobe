#!/usr/bin/env python3
"""
Turn everything under data/ into figures and a summary table.

Runs on GitHub, not on your computer. You never install anything.

WHY IT EXISTS: you will run the measure workflow several times. These are
shared virtual machines, so no single run is authoritative. This script
pools your runs, reports the MEDIAN with the observed SPREAD, and draws the
comparison across processor architectures.

Reporting a spread is not an admission of weakness. A number without an
uncertainty is not a measurement.

Usage:  python3 scripts/analyze.py data site
"""

import re
import sys
from pathlib import Path

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import pandas as pd

COLOURS = {"x86_64-linux": "#00553A",
           "arm64-linux":  "#9A6B00",
           "arm64-apple":  "#2E6DA4"}


def human(n):
    for unit, size in (("GiB", 2**30), ("MiB", 2**20), ("KiB", 2**10)):
        if n >= size:
            v = n / size
            return f"{v:.0f} {unit}" if v == int(v) else f"{v:.1f} {unit}"
    return f"{int(n)} B"


def load(root: Path):
    """data/<arch>/run-<n>/{line,levels}.csv -> {arch: {kind: [df, ...]}}"""
    out = {}
    for arch_dir in sorted(p for p in root.iterdir() if p.is_dir()):
        runs = sorted(arch_dir.glob("run-*"))
        if not runs:
            continue
        acc = {"line": [], "levels": []}
        for r in runs:
            for kind in acc:
                f = r / f"{kind}.csv"
                if f.exists():
                    try:
                        df = pd.read_csv(f)
                        if len(df):
                            df["run"] = int(re.sub(r"\D", "", r.name) or 0)
                            acc[kind].append(df)
                    except Exception as e:
                        print(f"  skipping {f}: {e}")
        if acc["line"] or acc["levels"]:
            out[arch_dir.name] = acc
    return out


def pooled(frames, key, value):
    """Median and spread of `value` at each `key`, across runs."""
    allf = pd.concat(frames, ignore_index=True)
    g = allf.groupby(key)[value]
    return pd.DataFrame({"med": g.median(), "lo": g.min(),
                         "hi": g.max(), "n": g.count()}).reset_index()


def knee(df):
    """Largest jump between consecutive plateaus: a crude cache boundary."""
    if len(df) < 3:
        return None
    d = df.sort_values("working_set_bytes").reset_index(drop=True)
    best, where = 0.0, None
    for i in range(1, len(d)):
        prev = d.med[i - 1]
        if prev <= 0:
            continue
        ratio = d.med[i] / prev
        if ratio > best:
            best, where = ratio, d.working_set_bytes[i - 1]
    return where if best > 1.4 else None


def main(argv):
    root = Path(argv[1] if len(argv) > 1 else "data")
    site = Path(argv[2] if len(argv) > 2 else "site")
    plots = site / "plots"
    plots.mkdir(parents=True, exist_ok=True)

    if not root.is_dir():
        print(f"error: no {root}/ directory. Run the measure workflow first.")
        return 1
    data = load(root)
    if not data:
        print(f"error: no runs found under {root}/.\n"
              "Go to the Actions tab and run the 'measure' workflow.")
        return 1

    rows = []

    # --- per architecture -------------------------------------------------
    for arch, kinds in data.items():
        col = COLOURS.get(arch, "#555555")

        if kinds["levels"]:
            p = pooled(kinds["levels"], "working_set_bytes", "ns_per_access")
            fig, ax = plt.subplots(figsize=(7.5, 4))
            ax.fill_between(p.working_set_bytes, p.lo, p.hi, color=col, alpha=.18)
            ax.semilogx(p.working_set_bytes, p.med, color=col, lw=1.6, marker="o", ms=2.5)
            ax.set_xlabel("working set (bytes, log scale)")
            ax.set_ylabel("latency (ns per access)")
            ax.set_title(f"{arch} — median of {int(p.n.max())} run(s), shaded = observed range")
            ax.grid(alpha=.3, which="both", ls=":")
            fig.tight_layout()
            fig.savefig(plots / f"{arch}-levels.png", dpi=150)
            plt.close(fig)

            k = knee(p)
            spread = (p.hi / p.lo.replace(0, float("nan"))).max()
            rows.append({
                "architecture": arch,
                "runs": int(p.n.max()),
                "fastest ns": round(p.med.min(), 2),
                "slowest ns": round(p.med.max(), 1),
                "ratio": round(p.med.max() / max(p.med.min(), 1e-9), 1),
                "first knee": human(k) if k else "not resolved",
                "worst run-to-run spread": f"{spread:.1f}x" if spread == spread else "n/a",
            })

        if kinds["line"]:
            p = pooled(kinds["line"], "stride_bytes", "ns_per_access")
            fig, ax = plt.subplots(figsize=(7.5, 3.4))
            ax.fill_between(p.stride_bytes, p.lo, p.hi, color=col, alpha=.18)
            ax.semilogx(p.stride_bytes, p.med, base=2, color=col, lw=1.6, marker="s", ms=4)
            ax.set_xlabel("stride (bytes, log scale)")
            ax.set_ylabel("latency (ns per access)")
            ax.set_title(f"{arch} — stride sweep; the knee is the cache line size")
            ax.grid(alpha=.3, which="both", ls=":")
            fig.tight_layout()
            fig.savefig(plots / f"{arch}-line.png", dpi=150)
            plt.close(fig)

    # --- the comparison: the point of the whole project -------------------
    have = [(a, k) for a, k in data.items() if k["levels"]]
    if len(have) > 1:
        fig, ax = plt.subplots(figsize=(7.5, 4.2))
        for arch, kinds in have:
            p = pooled(kinds["levels"], "working_set_bytes", "ns_per_access")
            c = COLOURS.get(arch, "#555555")
            ax.fill_between(p.working_set_bytes, p.lo, p.hi, color=c, alpha=.14)
            ax.semilogx(p.working_set_bytes, p.med, lw=1.8, color=c, label=arch)
        ax.set_xlabel("working set (bytes, log scale)")
        ax.set_ylabel("latency (ns per access)")
        ax.set_title("Memory hierarchies across processor architectures")
        ax.legend(fontsize=9)
        ax.grid(alpha=.3, which="both", ls=":")
        fig.tight_layout()
        fig.savefig(plots / "comparison.png", dpi=150)
        plt.close(fig)
        print("  wrote comparison.png")

    # --- summary table ----------------------------------------------------
    if rows:
        df = pd.DataFrame(rows)
        md = site / "summary.md"
        md.write_text("# Measurement summary\n\n"
                      "Pooled across every committed run. Latencies from shared\n"
                      "virtual machines: treat the spread as real uncertainty.\n\n"
                      + df.to_markdown(index=False) + "\n")
        print("\n" + df.to_string(index=False))
        print(f"\n  wrote {md}")

    n = len(list(plots.glob('*.png')))
    print(f"\n{n} figure(s) in {plots}/")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
