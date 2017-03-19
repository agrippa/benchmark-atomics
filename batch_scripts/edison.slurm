#!/bin/bash -l

#SBATCH -p debug
#SBATCH -N 1
#SBATCH -t 00:03:00
#SBATCH -J benchmark-atomics
#SBATCH --exclusive
#SBATCH --mail-type=ALL

ulimit -c unlimited

# 2 sockets x 12-core CPUs

cd $SLURM_SUBMIT_DIR

for EXE in $(ls bin/); do
    echo "===== $EXE ====="
    # Stay in one socket
    srun --ntasks=1 --cpus-per-task=12 --cpu_bind=verbose,sockets ./bin/$EXE
done
