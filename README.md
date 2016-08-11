# Population simulation project
==
## C++ codes
Note, if the compilations don't work, please use the -std=c++11 flag.


### pedsort.cpp
This program sort a pedigree, so that,
1. an offspring appears after its parents
2. id appear in pa and ma column only will be given a name in id column.
3. new id name is the line number, which starts from 1.

Description:
* Input: id pa ma other-info
* Output: id pa ma original-id-name other-info
* Compilation: g++ -O2 -o pedsort pedsort.cpp

Example:

./pedsort < unsorted-pedigree > sorted-pedigree


### extped.cpp
This program add ideal ID number in front of the sorted pedigree.  Old id numbers are added by Ne-1. Hence ID in extended pedigree starts from 0.

Description:
* Input: pa ma
* Output: pa ma
* Argument: Ne
* Compilation: g++ -O2 -o extped extped.cpp

Example:

gawk '{print $2, $3}' sorted-pedigree | ./extped 10 > extd.ped


### pgsnp.cpp
This program is a forward simulator to generate a Fisher-Wright balanced population.

Description:
* Input: none
* Output: nID nLocus (snps-by-haplotype) (bp freq)
* Arguments: Ne nGeneration chromosome-length-in-Morgan mutation-rate/meiossis/Morgan
* Compilation: g++ -O2 -o pgsnp pgsnp.cpp

Example:

./pgsnp 100 10000 1 1 > ideal.pop


## Data files
### lnw139.ped
* This is a sample pedigree was adapted from Lynch and Walsh 1996, pp139.

## Bash drivers