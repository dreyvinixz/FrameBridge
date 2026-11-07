"""Portable tests: python -m unittest discover -s tests -p '*latency_csv_test.py'"""

import csv
import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "scripts"))
from compare_latency_csv import compare, load_samples, percentile, summarize  # noqa: E402


class LatencyCsvTest(unittest.TestCase):
    def test_percentile_and_change(self):
        self.assertEqual(percentile([10.0, 20.0, 30.0], 95), 29.0)
        stats = summarize([10.0, 20.0, 30.0])
        self.assertEqual(stats["p50_ms"], 20.0)
        changed = compare([10.0, 20.0], [5.0, 10.0])
        self.assertEqual(changed["change_percent"]["p95_ms"], -50.0)

    def test_csv_missing_and_invalid_values(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "capture.csv"
            with path.open("w", encoding="utf-8", newline="") as stream:
                writer = csv.DictWriter(stream, fieldnames=["MsBetweenDisplayChange"])
                writer.writeheader()
                for value in ("12.5", "n/a", "", "-2.0", "0", "inf", "8.5"):
                    writer.writerow({"MsBetweenDisplayChange": value})
            self.assertEqual(load_samples(path, "MsBetweenDisplayChange"), [12.5, 8.5])
            with self.assertRaisesRegex(ValueError, "missing column"):
                load_samples(path, "Nonexistent")

    def test_reject_empty_or_insufficient_data(self):
        with self.assertRaises(ValueError):
            summarize([])
        with self.assertRaises(ValueError):
            percentile([], 95)


if __name__ == "__main__":
    unittest.main()
