# Population simulation project

## Bash drivers
### drop.sh
Simulate 3 chromosomes in parallel, drop the simulated genotypes through the founders of lnw139.ped into the pedigree.  Then merge the 3 files into one.


## C++ codes
Note, if the compilations don't work, please use the -std=c++11 flag.  Commercial libraries were avoided here.


### pedsort.cpp
This program sort a pedigree, so that,

1. an offspring appears after its parents
2. id appear in pa and ma column only will be given a name in id column.
3. new id name is the line number, which starts from 1.

|Item|Description|
|---:|:---|
| Input | id pa ma other-info|
| Output | id pa ma original-id-name other-info|
| Compilation | g++ -O2 -o pedsort pedsort.cpp|

Example:

* ./pedsort < unsorted-pedigree > sorted-pedigree


### extped.cpp
This program add ideal ID number in front of the sorted pedigree.  Old id numbers are added by Ne-1. Hence ID in extended pedigree starts from 0.

|Item|Description|
|---:|:---|
| Input | pa ma |
| Output | pa ma |
| Argument | Ne |
| Compilation | g++ -O2 -o extped extped.cpp|

Example:

* gawk '{print $2, $3}' sorted-pedigree | ./extped 10 > extd.ped


### pgsnp.cpp
This program is a forward simulator to generate a Fisher-Wright balanced population.

|Item|Description|
|---:|:---|
| Input | none |
| Output | nID nLocus (snps-by-haplotype) (bp freq) |
| Arguments | Ne nGeneration chromosome-length-in-Morgan mutation-rate/meiossis/Morgan|
| Compilation | g++ -O2 -o pgsnp pgsnp.cpp |

Example:

* ./pgsnp 100 10000 1 1 > ideal.pop


### pdrop.cpp
Drop the genotypes from an ideal population through the founders of a pedigree.

|Item|Description|
|---:|:---|
| Input | file from pgsnp. |
| Output | nID nLocus (bp freq) (genotypes by locus) |
| Arguments | extended-pedigree Ne |
| Compilation | g++ -O2 -o pdrop pdrop.cpp |

Example:

* ./pgsnp 100 10000 1 1 | ./pdrop extended-pedigree Ne


### cmerge.cpp
Merge several chromosomes of genotypes from pdrop into one file.

|Item|Description|
|---:|:---|
| Input | none |
| Output | nID nLocus (bp freq) (genotypes by locus)|
| Arguments | all the genotype file names|
| Compilation | g++ -O2 -o cmerge cmerge.cpp|

Example:

* ./cmerge 1.gt 2.gt > a.gt


### amatrix.cpp
Calculate the numerical relationship matrix of a pedigree.

|Item|Description|
|---:|:---|
| Input | pa ma |
| Output | nid (lower triangle of A elements in binary) |
| Argument | none |
| Compilation | g++ -O2 -o amatrix amatrix.cpp|

Example:

* gawk '{print $2, $3}' lnw139.ped | ./amatrix > A


### viewg.cpp
View the results of an A or G matrix.

|Item|Description|
|---:|:---|
| Input | A or G |
| Output | decimal of binary |
| Argument | [precision] |
| Compilation | g++ -O2 -o amatrix amatrix.cpp|

Example:

* gawk '{print $2, $3}' lnw139.ped | ./amatrix | ./viewg 4 | head



## Data files
### lnw139.ped
* This is a sample pedigree was adapted from Lynch and Walsh 1996, pp139.



## Bug history