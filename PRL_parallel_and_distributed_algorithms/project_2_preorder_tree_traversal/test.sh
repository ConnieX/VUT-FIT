#!/bin/bash

#preklad cpp zdrojaku
mpic++ --prefix /usr/local/share/OpenMPI -o oems oems.cpp

input=$1

input_length=$(echo -n $input | wc -m)


#spusteni
proc_count=$((2*$input_length-1))
mpirun --oversubscribe --prefix /usr/local/share/OpenMPI -np $proc_count oems $input

#uklid
rm -f oems numbers