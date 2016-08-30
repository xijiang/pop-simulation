# Population simulation project

* [Version history](#version-history)
* [Bash drivers](#bash-drivers)
  + [drop.sh](#dropsh)
  + [gsimu.sh](#gsimush)
  + [psimu.sh](#psimush)
* [C++ codes](#c---codes)
  + [pedsort.cpp](#pedsortcpp)
  + [extped.cpp](#extpedcpp)
  + [pgsnp.cpp](#pgsnpcpp)
  + [pdrop.cpp](#pdropcpp)
  + [cmerge.cpp](#cmergecpp)
  + [amatrix.cpp](#amatrixcpp)
  + [viewg.cpp](#viewgcpp)
  + [trdgpt.cpp](#trdgptcpp)
  + [chol.cpp](#cholcpp)
  + [fntpcd.cpp](#fntpcdcpp)
  + [ggtpt.cpp](#ggtptcpp)
* [Data files](#data-files)
  + [lnw139.ped](#lnw139ped)
* [Example files](#example-files)
  + [xamat.cpp](#xamatcpp)
* [Makefiles](#makefiles)
  + [drop.makefile](#dropmakefile)
  + [psimu.makefile](#psimumakefile)
  + [gsimu.makefile](#gsimumakefile)
* [Bug history](#bug-history)
* [ToDo:](#todo-)


## Version history

*Version 0.9*, before implementation.  Will be named 1.0 if survived several projects.

* For bugs and issues.

 1. Contact qtl.aas at gmail for bugs.
 2. Or the github way.

You can use my codes freely, but take your own risks.

## Bash drivers
### drop.sh
Simulate 3 chromosomes in parallel, drop the simulated genotypes through the founders of lnw139.ped into the pedigree.  Then merge the 3 files into one.


### gsimu.sh
Simulation a population through the QTL way.


### psimu.sh
Simulation a population with Cholesky decompostion


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


### trdgpt.cpp
Given a sorted pedigre, phenotype variance and heritability (h^2), this program simulates the genotypes and phenotypes for everybody in the pedigree.

|Item|Description|
|---:|:---|
| Input | (id pa ma) |
| Output | (id pa ma genotype phenotype) |
| Argument | V_p h2 |
| Compilation | g++ -O2 -o trdgpt trdgpt.cpp|

* Note: this need to be rewrited as it didn't consider inbreeding, where the Mendelian sampling error is smaller by a factor of (1 - \bar{F}).
* Using A matrix Cholesky decomposition is safer.
* Using QTLs can also avoid such problems.


### chol.cpp
* Cholesky decomposition
* Row majored.
* One-dimension array is used to store a matrix.
* Lower triangle and D store the original matrix.
* Upper triangle is the factor.
* Rank is returned as long.
* No dimension range check.
* Optimization will be done later.


### fntpcd.cpp
* phenotype simulation with an A matrix.
* Output: pa ma genotype phenotype


### ggtpt.cpp
* Sample QTL loci and effects.
* Simulate genotypes and phenotypes with Laplacian distribution.



## Data files
### lnw139.ped
* This is a sample pedigree was adapted from Lynch and Walsh 1996, pp139.


## Example files
### xamat.cpp
* Construct an A matrix with a given pedigree


## Makefiles
### drop.makefile
* Simulation of SNP from ideal population


### psimu.makefile
* Phenotype simulation with A matrix.


### gsimu.makefile
* Phenotype simulation with QTL.


## Bug history

* trdgpt.cpp is depricated as it ignored inbreeding.


## ToDo:
 - Cholesky decomposition with AVX/AVX2.
 - Maybe more detailed descriptions.