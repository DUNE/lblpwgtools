#!/usr/bin/env python3
"""Plot p-value vs POT for each statistical test.

Input: text file with a header line and 4 columns (tab or space separated):
    Nominal POT | KS p-value | KS normalized | Anderson-Darling p-value

Usage:
    python plot_pvalues.py [input_file] [output_dir]
"""
import sys
from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np

infile = Path(sys.argv[1]) if len(sys.argv) > 1 else Path("pvalues.txt")
outdir = Path(sys.argv[2]) if len(sys.argv) > 2 else Path(".")
outdir.mkdir(parents=True, exist_ok=True)

# skiprows=1 skips the header; delimiter=None splits on any whitespace (tabs or spaces)
data = np.loadtxt(infile, skiprows=1)
data = data[np.argsort(data[:, 0])]  # sort by POT

pot = data[:, 0]
tests = [
    (r"KS p-value (reco $p_{\mu}$ )", data[:, 1], "ks_pvalue.png"),
    (r"KS normalized p-value (reco $p_{\mu}$)", data[:, 2], "ks_normalized_pvalue.png"),
    (r"Anderson-Darling p-value (reco $p_{\mu}$)", data[:, 3], "ad_pvalue.png"),
]

for label, pval, fname in tests:
    fig, ax = plt.subplots(figsize=(6, 4.5))
    ax.plot(pot, pval, "o-", lw=1.5, ms=6, label=label)
    ax.axhline(0.05, color="red", ls="--", lw=1, label="p = 0.05")
    ax.axhline(0.01, color="green", ls="--", lw=1, label="p = 0.01")
    ax.set_xlabel("Nominal POT")
    ax.set_ylabel("p-value")
    ax.set_yscale("log")
    ax.set_title(label)
    ax.grid(True, which="both", alpha=0.3)
    ax.legend()
    fig.tight_layout()
    fig.savefig(outdir / fname, dpi=150)
    plt.close(fig)
    print(f"saved {outdir / fname}")