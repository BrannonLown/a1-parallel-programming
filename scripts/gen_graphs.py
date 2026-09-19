import argparse
import numpy as np
def gen_er(n, m, rng):
    edges = set()
    while len(edges) < m:
        need = m - len(edges)
        batch = int(need * 1.2) + 16
        u = rng.integers(0, n, size=batch)
        v = rng.integers(0, n, size=batch)
        mask = u != v
        u, v = u[mask], v[mask]
        lo = np.minimum(u, v)
        hi = np.maximum(u, v)
        for a, b in zip(lo.tolist(), hi.tolist()):
            edges.add((a,b))
            if len(edges) >= m:
                break
    return edges


def gen_rmat(n_bits, m, rng, a=0.57, b=0.19, c=0.19, d=0.05):
    u = np.zeros(m, dtype=np.int64)
    v = np.zeros(m, dtype=np.int64)
    for _ in range(n_bits):
        r = rng.random(m)
        bit_u = (r >= a + b).astype(np.int64)
        bit_v = ((r >= a) & (r < a + b + c)).astype(np.int64)
        u = (u << 1) | bit_u
        v = (v << 1) | bit_v
    mask = u != v
    u, v = u[mask], v[mask]
    lo = np.minimum(u, v)
    hi = np.maximum(u, v)
    return set(zip(lo.tolist(), hi.tolist()))

def write_mtx(path, n, edges):
    with open(path, "w") as f:
        f.write("%%MatrixMarket matrix coordinate pattern symmetric\n")
        f.write(f"{n} {n} {len(edges)}\n")
        for a, b in edges:
            f.write(f"{a + 1} {b + 1}\n")
    print(f"wrote {path}: n={n} m={len(edges)}")

if __name__ == "__main__":
    ap = argparse.ArgumentParser()
    ap.add_argument("model", choices=["er", "rmat"])
    ap.add_argument("--bits", type=int, default=20)
    ap.add_argument("--degree", type=float, default=16.0)
    ap.add_argument("--out", required=True)
    ap.add_argument("--seed", type=int, default=1)
    args = ap.parse_args()

    n = 1 << args.bits
    m = int(n * args.degree / 2)
    rng = np.random.default_rng(args.seed)

    edges = gen_er(n, m, rng) if args.model == "er" else gen_rmat(args.bits, m, rng)
    write_mtx(args.out, n, edges)