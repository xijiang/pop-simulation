/*******************************************************************************
 * Given SNP genotypes of a population and number of QTL, this program samples
 * QTL from the SNP given, assigns QTL from a Laplacian distribution.  It then
 * outputs: pa ma genotype- phenotype-value
 * Sampled QTL and values are output to sampled.qtl (reserved name).
 *                                                  by Xijiang Yu
 *                                                  Aug. 29, 2016
 */
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <random>
#include <algorithm>
#include <cmath>

typedef std::uniform_int_distribution<int>    UIDI;
typedef std::exponential_distribution<double> EDD;
typedef std::normal_distribution<double>      NDD;
const   std::string QTLfile{"sampled.qtl"};

int main(int argc, char *argv[])
{
  if(argc!=3){
    std::cerr<<"Usage: cat gt.snp | "<<argv[0]<<" N_QTL h2\n";
    return 1;
  }
  
  int    nqtl(atoi(argv[1]));
  double h2  (atof(argv[2]));
  int    nlc, nid;

  std::cin>>nid>>nlc;
  if(nlc<nqtl){
    std::clog<<"Number of QTL changed from "<<nqtl<<" to "<<nlc<<'\n';
    nqtl = nlc;
  }

  // random number generators
  std::mt19937 rng;
  UIDI         uni(0, nlc-1);
  EDD          expd(1.);
  NDD          nrmd(0., 1.);
  {// baking the random generator
    std::random_device rdv;
    int seeds[624];  {for(auto&x:seeds) x = rdv();}
    std::seed_seq seq(seeds, seeds + 624);
    rng.seed(seq);
  }
  
  int lqtl[nlc];
  { //Sample QTL
    for(auto i=0; i<nlc; ++i) lqtl[i] = i;
    std::shuffle(lqtl, lqtl+nlc, rng);
    std::sort   (lqtl, lqtl+nqtl);
  }

  double eqtl[nqtl];
  { //QTL effects
    for(auto&p:eqtl) p = expd(rng)-expd(rng);
    std::ofstream foo(QTLfile); //write QTL info to QTLfile
    for(auto i=0; i<nqtl; ++i)  foo<<lqtl[i]<<' '<<eqtl[i]<<'\n';
  }
  
  double gt[nid]{0};
  { //Accumulate genotype values accross QTL
    std::string line;
    int         i, j, k;
    
    for(i=0; i<nlc; ++i) std::cin>>j>>j; //Skip the frequencies
    i=j=0;
    while(j<nqtl){
      std::cin>>line;
      if(i==lqtl[j]){
	for(k=0; k<nid; ++k) gt[k] += (line[k]-'0')*eqtl[j];
	++j;
      }
      ++i;
    }
  }

  { //Geno- and pheno-type for every body
    double ave, var, sde;
    ave = var = 0;
    for(auto&p:gt){
      ave+=p;
      var+=p*p;
    }
    var = (var-ave*ave/nid)/(nid-1);
    sde = std::sqrt(var*(1-h2));
    for(auto&p:gt) std::cout<<p<<' '<<p+nrmd(rng)*sde<<'\n';
  }
  return 0;
}
