#!/bin/bash
#SBATCH --job-name=matmul
#SBATCH --partition=Centaurus
#SBATCH --time=00:30:00
#SBATCH --output=slurm-%j.out

cd ~/a1_blown
gcc -O3 src/matmul.c -o matmul
echo "order,M,K,N,avg,median,min,max,rate,running_total" > data/part3_matmul.csv
./matmul >> data/part3_matmul.csv
python3 scripts/plot_matmul.py