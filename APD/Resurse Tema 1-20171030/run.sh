#!/bin/bash

if [ $# -ne 0 ] && [ $# -ne 2 ]; then
    echo "usage:"
    echo "   ---   bash run.sh"
    echo "   ---   bash run.sh input_file output_file"
    exit
fi

if [ $# == 2 ]; then
    input=$1
    output=$2
else
    input=big_input;
    output=big_output;
fi

my_output=output
steps=20000
TIME=$(./parallel_snake $input $my_output $steps);

if [ -z "$(diff $output $my_output)" ]; then
    echo "$TIME"
else
    echo "Wrong Result"
    diff $output $my_output
fi
