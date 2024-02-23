#!/bin/bash

#pocet cisel pevne nastaveny na 8
numbers=8

#preklad cpp zdrojaku
mpic++ --prefix /usr/local/share/OpenMPI -o oems oems.cpp


#vyrobeni souboru s random cisly
dd if=/dev/random bs=1 count=$numbers of=numbers status=none

#spusteni
mpirun --oversubscribe --prefix /usr/local/share/OpenMPI -np 19 oems

#uklid
rm -f oems numbers