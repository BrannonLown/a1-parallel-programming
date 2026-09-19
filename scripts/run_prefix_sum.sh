#!/bin/bash
#SBATCH --job-name=prefix_sum
#SBATCH --partition=Centaurus
#SBATCH --time=00:15:00
#SBATCH --mem=8G

cd ~/a1_blown

gcc -O0 src/prefix_sum.c -o prefix_sum_O0
gcc -O2 src/prefix_sum.c -o prefix_sum_O2
gcc -O3 src/prefix_sum.c -o prefix_sum_O3

./prefix_sum_O0 -O0 > data/prefix_sum_results.csv
./prefix_sum_O2 -O2 >> data/prefix_sum_results.csv
./prefix_sum_O3 -O3 >> data/prefix_sum_results.csv
