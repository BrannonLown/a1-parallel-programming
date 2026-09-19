#!/bin/bash
#SBATCH --job-name=gen_graphs
#SBATCH --partition=Centaurus
#SBATCH --time=00:30:00
#SBATCH --output=slurm-%j.out

cd ~/a1_blown

python3 scripts/gen_graphs.py er --bits 20 --degree 16 --out data/er.mtx --seed 1
python3 scripts/gen_graphs.py rmat --bits 20 --degree 16 --out data/rmat.mtx --seed 1