import csv
import matplotlib.pyplot as plt

rows = []
with open('data/array_max_results.csv','r') as f:
    reader = csv.reader(f)
    for row in reader:
        rows.append(row)

sizes_bytes = []
rates = []
for row in rows:
    type_label, version, order, n, avg, median, mn, mx, rate, total = row
    if order != "bandwidth_sweep":
        continue
    n = int(n)
    bytes_size = n * 8
    sizes_bytes.append(bytes_size)
    rates.append(float(rate))

paired = sorted(zip(sizes_bytes, rates))
sizes_bytes = [p[0] for p in paired]
rates = [p[1] for p in paired]

plt.figure(figsize=(9, 6))
plt.plot(sizes_bytes, rates, marker='o', label='Measured bandwidth')

l1_bytes = 31 * 1024
l2_bytes = 1 * 1024 * 1024
l3_bytes = int(24.75 * 1024 * 1024)

plt.axvline(l1_bytes, color='red', linestyle='--', label='L1d (~31 KB)')
plt.axvline(l2_bytes, color='green', linestyle='--', label='L2 (1 MB)')
plt.axvline(l3_bytes, color='purple', linestyle='--', label='L3 (24.75 MB)')

plt.xscale('log')
plt.xlabel('Array size (bytes)')
plt.ylabel('Bandwidth (bytes/second)')
plt.title('Memory Bandwidth vs Array Size (double, branchless)')
plt.legend()
plt.grid(True, which="both", ls="--", alpha=0.5)
plt.savefig('data/bandwidth_vs_size.png')
print("Plot saved to data/bandwidth_vs_size.png")