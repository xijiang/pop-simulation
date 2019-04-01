#!/usr/bin/env bash
if [ $# != 1]; then
    echo Usage: ./run.sh option
    echo Options:
    echo -e "\t"psim: simulate a pedigree, the traditional way
    echo -e "\t"gsim: simulate 3 chromosomes of SNP of an ideal population
    echo -e "\t"drop: drop chromosomes into a pedigree.
else
    case $1 in
	prepare)
	    echo Prepare environment
	    mkdir -p bin work
	    cd src
	    make
	    make mv
	    ;;
	psim)
	    fnc/psimu.sh
	    ;;
	gsim)
	    fnc/gsimu.sh
	    ;;
	drop)
	    fnc/drop.sh
	    ;;
    esac
fi
