#!/bin/bash
#SBATCH --job-name=array_max
#SBATCH --partition=Centaurus
#SBATCH --time=00:15:00
#SBATCH --mem=8G

cd ~/a1_blown
gcc -O2 -Wall src/array_max.c -o array_max
./array_max > data/array_max_results.csv