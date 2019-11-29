#!/bin/bash

mpicc $1 -o executable
mpirun -q -n 4 ./executable
