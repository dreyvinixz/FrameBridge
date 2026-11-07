#!/usr/bin/env python3
"""Compare real timing samples from two CSV captures (standard library only).

Example: python scripts/compare_latency_csv.py baseline.csv optimized.csv \
    --column MsBetweenDisplayChange --json-output results.json

MsBetweenDisplayChange is display cadence, NOT absolute input-to-photon latency.
For input-to-photon, supply an actual measured latency column from LDAT or
another physical input-to-display measurement source. Never use synthetic GPU
times from internal frame reports.
"""

import argparse
import csv
import json
import math
import statistics
from pathlib import Path


def percentile(sorted_values: list[float], percent: float) -> float:
    """Linear interpolation (R7) between sorted observations."""
    if not sorted_values:
        raise ValueError("Cannot summarize zero samples")
    position = (len(sorted_values) - 1) * percent / 100.0
    left = int(position)
    right = min(left + 1, len(sorted_values) - 1)
    return sorted_values[left] + (sorted_values[right] - sorted_values[left]) * (
        position - left
    )


def load_samples(path: Path, column: str) -> list[float]:
    values = []
    with path.open("r", encoding="utf-8-sig", newline="") as stream:
        reader = csv.DictReader(stream)
        if not reader.fieldnames or column not in reader.fieldnames:
            raise ValueError(
                f"{path}: missing column '{column}'; "
                f"available: {reader.fieldnames or []}"
            )
        for row in reader:
            raw = row.get(column)
            if not raw or not raw.strip():
                continue
            try:
                value = float(raw)
            except ValueError:
                continue  # N/A, unavailable, or unsupported data
            if math.isfinite(value) and value > 0:
                values.append(value)
    return values


def summarize(samples: list[float]) -> dict:
    if not samples:
        raise ValueError("No valid, positive samples were found")
    ordered = sorted(samples)
    return {
        "count": len(ordered),
        "mean_ms": round(statistics.fmean(ordered), 4),
        "p50_ms": round(percentile(ordered, 50), 4),
        "p95_ms": round(percentile(ordered, 95), 4),
        "p99_ms": round(percentile(ordered, 99), 4),
        "max_ms": round(ordered[-1], 4),
    }


def compare(baseline: list[float], candidate: list[float]) -> dict:
    before = summarize(baseline)
    after = summarize(candidate)
    changes = {}
    for metric in ("mean_ms", "p50_ms", "p95_ms", "p99_ms", "max_ms"):
        changes[metric] = round(
            100.0 * (after[metric] - before[metric]) / before[metric], 3
        ) if before[metric] > 0 else None
    return {"baseline": before, "candidate": after, "change_percent": changes}


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("baseline", type=Path, help="CSV captured with optimization OFF")
    parser.add_argument("candidate", type=Path, help="CSV captured with optimization ON")
    parser.add_argument(
        "--column",
        default="MsBetweenDisplayChange",
        help="Measured, positive millisecond column in both files",
    )
    parser.add_argument(
        "--min-samples", type=int, default=100, help="Minimum valid rows in each capture"
    )
    parser.add_argument(
        "--json-output", type=Path, help="Optional path for a machine-readable result"
    )
    args = parser.parse_args()
    if args.min_samples < 1:
        parser.error("--min-samples must be at least 1")

    try:
        baseline = load_samples(args.baseline, args.column)
        candidate = load_samples(args.candidate, args.column)
        if min(len(baseline), len(candidate)) < args.min_samples:
            raise ValueError(
                f"Not enough valid observations: baseline={len(baseline)}, "
                f"candidate={len(candidate)}, required={args.min_samples}"
            )
        result = compare(baseline, candidate)
    except (OSError, ValueError) as exc:
        parser.error(str(exc))

    result["column"] = args.column
    result["unit"] = "milliseconds"
    result["interpretation"] = (
        "Lower values are better; negative change means a reduction. "
        "This comparison does not establish causality or input-to-photon latency "
        "unless the source metric directly measures it."
    )
    output = json.dumps(result, indent=2, ensure_ascii=False)
    print(output)
    if args.json_output:
        args.json_output.write_text(output + "\n", encoding="utf-8")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
