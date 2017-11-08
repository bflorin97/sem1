#!/bin/bash

clear
make
if [ -e result ]; then
    rm result -rf;
fi

touch result;

input=input1;
output=output1;

export OMP_NUM_THREADS=2
bash run.sh big_input big_output >> result;

export OMP_NUM_THREADS=4
bash run.sh big_input big_output >> result;

export OMP_NUM_THREADS=6
bash run.sh big_input big_output >> result;

export OMP_NUM_THREADS=8
bash run.sh big_input big_output >> result;

cat result
