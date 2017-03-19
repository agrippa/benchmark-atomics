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
    export OMP_NUM_THREADS=12
    # Single socket
    srun --ntasks=1 --cpus-per-task=$OMP_NUM_THREADS --cpu_bind=sockets ./bin/$EXE

    echo "  Dual Socket:"
    export OMP_NUM_THREADS=24
    # Dual socket
    srun --ntasks=1 --cpus-per-task=$OMP_NUM_THREADS ./bin/$EXE
done
