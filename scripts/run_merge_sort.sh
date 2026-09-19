#!/bin/bash
#SBATCH --job-name=mergesort
#SBATCH --partition=Centaurus
#SBATCH --time=00:30:00
#SBATCH --output=slurm-%j.out

cd ~/a1_blown

gcc -O3 src/merge_sort.c -o mergesort

./mergesort > data/mergesort_results.csv