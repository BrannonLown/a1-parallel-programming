import csv
import matplotlib.pyplot as plt

rows = []
with open('data/array_max_results.csv','r') as f:
    reader = csv.reader(f)
    for row in reader:
        rows.append(row)

combos = {}
for row in rows:
    type_label, version, order, n, avg, median, mn, mx, rate, total = row
    if order != "random":
        continue
    key = f"{type_label}-{version}"
    if key not in combos:
        combos[key] = {"n": [], "rate": []}
    combos[key]["n"].append(int(n))
    combos[key]["rate"].append(float(rate))

plt.figure(figsize=(8, 6))
for key, data in combos.items():
    paired = sorted(zip(data["n"], data["rate"]))
    ns = [p[0] for p in paired]
    rates = [p[1] for p in paired]
    plt.plot(ns, rates, marker='o', label=key)

plt.xscale('log')
plt.xlabel('n (number of elements)')
plt.ylabel('Rate (bytes/second)')
plt.title('Array Maximum: Rate vs n (random input order)')
plt.legend()
plt.grid(True, which="both", ls="--", alpha=0.5)
plt.savefig('data/rate_vs_n.png')
print("Plot saved to data/rate_vs_n.png")
