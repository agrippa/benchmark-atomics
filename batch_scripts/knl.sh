#!/bin/bash -l

ulimit -c unlimited

for EXE in $(ls bin/); do
    echo "$EXE:"

    export OMP_NUM_THREADS=16
    echo "  $OMP_NUM_THREADS threads"
    taskset 0xFFFF ./bin/$EXE

    export OMP_NUM_THREADS=32
    echo "  $OMP_NUM_THREADS threads"
    taskset 0xFFFFFFFF ./bin/$EXE

    export OMP_NUM_THREADS=64
    echo "  $OMP_NUM_THREADS threads"
    taskset 0xFFFFFFFFFFFFFFFF ./bin/$EXE

    export OMP_NUM_THREADS=128
    echo "  $OMP_NUM_THREADS threads"
    taskset 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF ./bin/$EXE

    export OMP_NUM_THREADS=256
    echo "  $OMP_NUM_THREADS threads"
    taskset 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF ./bin/$EXE
done
