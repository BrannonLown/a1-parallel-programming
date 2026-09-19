# ITCS 4145 Assignment 1 -- Brannon Lown

## Plotting

    Requires matplotlib: 
        pip3 install --user matplotlib
    Then:
        python3 scripts/plot_bandwidth.py
        python3 scripts/plot_rate.py
        python3 scripts/plot_matmul.py

## Build
make

builds: array_max, prefix_sum_O0/O2/O3, matmul, merg_sort, bfs.

## Run

run through salloc/sbatch on Centaurus partition, not the head node.

    saloc --partition=Centaurus --time=00:30:00

    ./array_max
    ./prefix_sum_O0
    ./prefix_sum_O2
    ./prefix_sum_O3
    ./matmul
    ./mergesort

BFS needs graphs generate first:

    python3 scripts/gen_graphs.py er   --bits 20 --degree 16 --out data/er.mtx
    python3 scripts/gen_graphs.py rmat --bits 20 --degree 16 --out data/rmat.mtx
    ./bfs data/er.mtx   > data/bfs_er_results.csv
    ./bfs data/rmat.mtx > data/bfs_rmat_results.csv

sbatch scripts for each part are in scripts/.

## Clean

    make clean