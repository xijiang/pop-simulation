/**
 * The Tabular Method
 *   (attributable to Henderson and perhaps to Wright before him).
 *
 * This program calculates the numerical relationship matrix, A, given a sorted
 * pedigree.

 * Note:
 *   It is possible to use half storage.  But current following up algorithms
 *   all use full storage.  Also half storage is error-prone, and also hard to
 *   read later.  So it is avoided here.
 *   To be accurate and to save storage, output is in binary. Use viewg to view.
 *
 *   If to use single precision, typedef float as FLTT.
 *
 *                                                  by Xijiang Yu
 *                                                  Aug. 04, 2016
 */

#include "pedigree.hpp"

void amatrix(std::vector<parent>&ped, double*A){ //the tabular method
  int    n(ped.size());
  int    i, j, pa, ma;
  double td;

  for(i=0; i<n; ++i){
    pa = ped[i].pa;
    ma = ped[i].ma;
    if(!pa || !ma) A[i*n+i] = 1;
    else           A[i*n+i] = 1 + A[(pa-1)*n+ma-1]*.5;
    for(j=i+1; j<n; ++j){
      td = 0;
      pa = ped[j].pa;
      ma = ped[j].ma;
      if(pa) td += A[i*n+pa-1]*.5;
      if(ma) td += A[i*n+ma-1]*.5;
      A[i*n+j] = A[j*n+i] = td;
    }
  }
}
