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
    echo "$EXE:"

    echo "  Single Socket:"
    # Single socket
    srun --ntasks=1 --cpus-per-task=12 --cpu_bind=sockets ./bin/$EXE

    echo "  Dual Socket:"
    # Dual socket
    srun --ntasks=1 --cpus-per-task=24 ./bin/$EXE
done
