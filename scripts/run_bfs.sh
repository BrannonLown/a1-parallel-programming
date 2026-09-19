#!/bin/bash
#SBATCH --job-name=bfs
#SBATCH --partition=Centaurus
#SBATCH --time=00:30:00
#SBATCH --output=slurm-%j.out

cd ~/a1_blown

gcc -O3 src/bfs.c -o bfs

./bfs data/er.mtx   > data/bfs_er_results.csv
./bfs data/rmat.mtx > data/bfs_rmat_results.csv