/**
 * Given a sorted pedigree, V_p and h^2, this program simulates geno- and 
 * phenotypes for al the individuals.
 * Or, this is the traditional way to simulate geno- and phenotypes.
 * This is depricated as no inbreeding is considered.
 *
 *                                                  by Xijiang Yu
 *                                                  Aug. 02, 2016.
 */

#include <iostream>
#include <random>
#include <cstdlib>
#include <map>
#include <string>
#include <cmath>

typedef std::map<std::string, double>    TMSD;
typedef std::normal_distribution<double> Gauss;
const int LEN{624};

int main(int argc, char *argv[])
{
  if(argc!=3){
    std::cerr<<" Usage: cat pedigree | "<<argv[0]<<" Var_P, h^2 > ofile\n";
    return 1;
  }
  double vp(atof(argv[1])), h2(atof(argv[2]));
  if(vp<=0 || h2<0 || h2>1){
    std::cerr<<"Invalid parameter: V_p = "<<vp<<";  h^2 = "<<h2<<'\n';
    return 2;
  }
  std::string id, pa, ma, unknown{"0"}; //all unknowns must be translated to 0
  TMSD   gt;
  double sg(std::sqrt(vp*h2));
  double ms(std::sqrt(vp*h2*.5));
  double se(std::sqrt(vp*(1-h2)));
  double tp, tm, to; //tp, tm, to: genotypes of pa, ma, offspring
  /**
   * ms is the standard error for Mendelian sampling error combined pa + ma.
   *     Vp = V(pa/2 + ma/2) + Vms + Ve
   * Vms = Vg/2.  So ms = above.
   */
						  
  // baking the random generator
  std::random_device rdv;
  int                seeds[LEN];  {for(auto&x:seeds) x = rdv();}
  std::seed_seq      seq(seeds, seeds + LEN);
  std::mt19937       rng(seq);
  Gauss              gg(0, sg), ee(0, se), mg(0, ms);

  while(std::cin>>id>>pa>>ma){
    if(pa == unknown) tp = gg(rng);                 // An unknown parent?
    else {
      if(gt.find(pa) == gt.end()) gt[pa] = gg(rng); // A parent not used before?
      tp = gt[pa];                                  // Store the pa genotype
    }
    if(ma == unknown) tm = gg(rng);
    else {
      if(gt.find(ma) == gt.end()) gt[ma] = gg(rng);
      tm = gt[ma];
    }
    to = gt[id] = (tp + tm)*.5 + mg(rng);
    std::cout<<id<<' '<<pa<<' '<<ma<<' '<<to<<' '<<to+ee(rng)<<'\n';
  }
  return 0;
}
