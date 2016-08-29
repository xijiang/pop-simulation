#!/usr/bin/env bash
#    _                    _       _     _                   _                   
#   / \     ___  ___ _ __(_)_ __ | |_  (_)___    __ _   ___| |_ ___  _ __ _   _ 
#  / _ \   / __|/ __| '__| | '_ \| __| | / __|  / _` | / __| __/ _ \| '__| | | |
# / ___ \  \__ \ (__| |  | | |_) | |_  | \__ \ | (_| | \__ \ || (_) | |  | |_| |
#/_/   \_\ |___/\___|_|  |_| .__/ \__| |_|___/  \__,_| |___/\__\___/|_|   \__, |
#                          |_|                                            |___/ 
# Create 3 chromosomes for each ID in a pedigree.
#                                                  by Xijiang Yu
#                                                  Aug. 29, 2016

make -f gsimu.makefile

nchr=3
ne=10
ng=10000
chrLen=1
mr=1
nqtl=10
h2=.9999

# prepare the pedigree
./pedsort < lnw139.ped |
    gawk '{print $2, $3}' > st.ped
./extped $ne < st.ped > et.ped

# Be careful below if system thread number is less than nchr
# then the speed will be much slow.
for i in `seq $nchr`; do
    ./pgsnp $ne $ng $chrLen $mr |
	./pdrop et.ped $ne > $i.snp &
done
wait
./cmerge ?.snp > all.snp

paste \
    <( gawk '{print $2, $3}' lnw139.ped ) \
    <( cat all.snp | ./ggtpt $nqtl $h2 ) \
    > gsimu.txt
