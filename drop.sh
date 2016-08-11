#!/usr/bin/env bash
#    _                    _       _     _                   _                   
#   / \     ___  ___ _ __(_)_ __ | |_  (_)___    __ _   ___| |_ ___  _ __ _   _ 
#  / _ \   / __|/ __| '__| | '_ \| __| | / __|  / _` | / __| __/ _ \| '__| | | |
# / ___ \  \__ \ (__| |  | | |_) | |_  | \__ \ | (_| | \__ \ || (_) | |  | |_| |
#/_/   \_\ |___/\___|_|  |_| .__/ \__| |_|___/  \__,_| |___/\__\___/|_|   \__, |
#                          |_|                                            |___/ 
# Cross-validation with sequence mutations
#                                                  by Xijiang Yu
#                                                  Aug. 11, 2016

cp makefile.drop Makefile
make
nchr=3
ne=10
ng=10000
chrLen=1
mr=1

# prepare the pedigree
./pedsort < lnw139.ped |
    gawk '{print $2, $3}' > st.ped
./extped $ne < st.ped > et.ped

for i in `seq $nchr`; do
    ./pgsnp $ne $ng $chrLen $mr |
	./pdrop et.ped $ne > $i.gt &
done
wait
./cmerge ?.gt > all.gt
