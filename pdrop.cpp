/******************************************************************************
 * Drop alleles from ideal population into a pedigree
 * No new mutation happens during this process.
 *
 * The pedigree file must be sorted and recoded so that line number is the ID
 *     number.  It contains 2 columns: pa ma.
 *
 *                                             Dec. 28, 2010
 *                                             by Xijiang Yu
 * Removed mkl parts. July 23, 2016
 * Totally rewritten on Aug. 09, 2016
 ******************************************************************************/
#include <iostream>
#include <fstream>
#include <random>
#include <cstdlib>
#include <vector>
#include <algorithm>

typedef std::uniform_int_distribution<int> UID;
typedef std::poisson_distribution<int>     PSD;

class parent{
public:
  int p[2];
  parent(int a, int b){
    p[0]=a;
    p[1]=b;
  }
};
  

int main(int argc, char *argv[])
{
  if(argc!=3){
    std::cerr<<"Usage: cat genotypes | "<<argv[0];
    std::cerr<<" pedigree chr-len >a-file\n\n";
    std::cerr<<"  genotypes are from pgsnp.\n";
    std::cerr<<"  pedigree were sorted with pedsort.\n";
    std::cerr<<"  chr-len is in Morgan, typically 1.\n";
    return 1;
  }
  
  int chr(std::atof(argv[2]));
  std::vector<parent> id;

  {
    std::clog<<"Reading the pedigree ...\n";
    std::ifstream fin(argv[1]);
    int pa, ma;
    while(fin>>pa>>ma) id.push_back(parent(pa, ma));
  }
  
  int  ne, nlc;  {std::cin>>ne>>nlc; }
  int  tid(id.size()), bp[nlc];
  char snp[tid][2][nlc];

  {
    int i, j, k;
    std::clog<<"Reading genotypes ...\n";
    for(i=0; i<ne; ++i)
      for(j=0; j<2; ++j)
	for(k=0; k<nlc; ++k) std::cin>>snp[i][j][k];
    for(i=0; i<nlc; ++i)     std::cin>>bp[i]>>k;
  }

  //random number related
  std::random_device rdv;
  const int     _len_{624};  //size random seed buffer
  int           seeds[_len_]; {for(auto&x:seeds) x=rdv();}
  std::seed_seq seq(seeds, seeds+_len_);
  std::mt19937  rng(seq);  //baked random number generator
  
  std::clog<<"Dropping the SNP into the pedigree...\n";
  
  UID rhp(0,1); //for the initial haplotide
  UID rco(1, nlc-1);
  PSD psn(chr);
  {
    int i, j, k, m, pp, ihp, nco;
    int pco[100];  //100 is an big enough for a cross-over number
    
    for(i=ne; i<tid; ++i){
      for(j=0; j<2; ++j){
	pp  = id[i].p[j];
	ihp = rhp(rng);
	nco = psn(rng);
	for(k=0; k<nco; ++k) pco[k] = rco(rng);
	pco[k] = nlc;
	++nco;
	std::sort(pco, pco+nco);
	for(m=k=0; k<nco; ++k){
	  while(m<pco[k]){
	    snp[i][j][m] = snp[pp][ihp][m];
	    ++m;
	  }
	  ihp = 1 - ihp;
	}
      }
    }
  }

  std::clog<<"Writing the results ...\n";
  int frq[nlc];
  bool oo[nlc];
  {
    int i, j, k, olc{0};
    for(auto&p:frq) p=0;
    for(i=ne; i<tid; ++i) for(j=0; j<2; ++j)
			    for(k=0; k<nlc; ++k)
			      frq[k] += int(snp[i][j][k] - '0');
    k = (tid - ne) * 2;
    for(i=0; i<nlc; ++i){
      oo[i] = (frq[i]!=0 && frq[i]!=k);
      if(oo[i]) ++olc;
    }
    std::cout<<tid-ne<<'\n'<<olc<<'\n';
    for(i=0; i<nlc; ++i) if(oo[i]) std::cout<<bp[i]<<' '<<frq[i]<<'\n';
    for(i=0; i<nlc; ++i)
      if(oo[i]){
	for(j=ne; j<tid; ++j)
	  std::cout<<int(snp[j][0][i] + snp[j][1][i] - '0' - '0');
	std::cout<<'\n';
      }
  }
  std::clog<<"... Done.";
  return 0;
}
