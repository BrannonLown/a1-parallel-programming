import csv
import matplotlib.pyplot as plt

rows = []
with open('data/part3_matmul.csv', 'r') as f:
    reader = csv.reader(f)
    next(reader)
    for row in reader:
        rows.append(row)

shapes = {}
for row in rows:
    order, M, K, N, avg, median, mn, mx, rate, total = row
    shape_label= f"{M}x{K}x{N}"
    if shape_label not in shapes:
        shapes[shape_label] = {}
    shapes[shape_label][order] = float(rate)

order_list = ["ijk", "ikj", "jik", "jki", "kij", "kji"]

for shape_label, order_rates in shapes.items():
    rates = [order_rates[o] for o in order_list]

    plt.figure(figsize=(8, 5))
    plt.bar(order_list, rates)
    plt.xlabel('Loop order')
    plt.ylabel('Rate (FLOPs/second)')
    plt.title(f'Matrix Multiplication Rate by Loop Order ({shape_label})')
    plt.grid(True, axis='y', ls='--', alpha=0.5)
    filename = f'data/matmul_rate_{shape_label}.png'
    plt.savefig(filename)
    plt.close()
    print(f"Plot saved to {filename}")