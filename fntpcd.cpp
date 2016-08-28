/**
 * Phenotype simulation with the A matrix.
 * Requires:
 * 1) A sorted pedigree, with two columns: pa & ma.
 * 2) h2
 *                                                  by Xijiang Yu
 *                                                  Aug. 28, 2016
 */
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <random>
#include <cmath>
#include "pedigree.hpp"

long    chol(double*, double*, long);
typedef std::normal_distribution<double> Gauss;
const   int LEN{624};

int main(int argc, char *argv[])
{
  if(argc!=3){
    std::cerr<<"Usage:\n   cat pedigree | "<<argv[0]<<" V_p h2 > result\n";
    return 1;
  }
  std::vector<parent>ped;
  {//Read the pedigree
    int  pa, ma;
    while(std::cin>>pa>>ma) ped.push_back(parent(pa,ma));
  }
  long   dim(ped.size());
  long   len(dim*dim);
  double A[len], D[dim];
  amatrix(ped, A); //the A matrix
  chol(A, D, dim); //its Cholesky factor

  double vp(atof(argv[1])), h2(atof(argv[2]));
  double sdg(std::sqrt(vp*h2)), sde(std::sqrt(vp*(1-h2)));
  double rg[dim], td;
  long   i, j, k(dim), w{1};

  while(k){//format control
    ++w;
    k/=10;
  }
  
  // baking the random generator
  std::random_device rdv;
  int                seeds[LEN];  {for(auto&x:seeds) x = rdv();}
  std::seed_seq      seq(seeds, seeds + LEN);
  std::mt19937       rng(seq);
  Gauss              gg(0, sdg), ee(0, sde);

  std::cout<<std::fixed;
  std::cout.precision(3);
  for(auto&p:rg) p=gg(rng);
  for(i=0; i<dim; ++i){
    td = 0.;
    for(j=0; j<=i; ++j) td+=A[j*dim+i]*rg[j];
    std::cout<<std::setw(w)<<ped[i].pa<<std::setw(w)<<ped[i].ma<<' ';
    std::cout<<std::setw(10)<<td+ee(rng)<<'\n';
  }
  return 0;
}
