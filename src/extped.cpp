/**
 * This is to assign parents sampled from an ideal population to the '0' named
 *   parents.  It takes only 2 columns, pa and ma. The line number is the ID
 *   number.  ID from the ideal population are inserted before the pedigree.
 *   The ID starts from 0 now.
 *                                                  by Xijiang Yu
 *                                                  Aug. 08, 2016
 */
#include <iostream>
#include <random>
#include <cstdlib>
#include <algorithm>

using namespace std;

int main(int argc, char *argv[])
{
  if(argc != 2){
    cerr<<"Usage: "<<argv[0]<<" Ne < sorted_pedigree\n";
    return 1;
  }
  
  //Bake the random number
  const int          _len{624};
  std::random_device rdv;
  int                seeds[_len]; {for(auto&p:seeds) p=rdv();}
  std::seed_seq      seq(seeds, seeds+_len);
  std::mt19937       rng(seq);

  int ne(atoi(argv[1]));
  int np(ne/2), pp[ne], pa, ma, ip, im;
  for(auto i=0; i<ne; ++i){
    pp[i] = i;
    cout<<"0 0\n";  //the ideal ID first
  }
  shuffle(pp, pp+np, rng);     //papa
  shuffle(pp+np, pp+ne, rng);  //mama
  ip = 0;                 //next pa
  im = np;                //next ma
  while(cin>>pa>>ma){
    if(pa) cout<<pa+ne-1;
    else{
      cout<<pp[ip++];
      if(ip==np){
	shuffle(pp, pp+np, rng);
	ip=0;
      }
    }
    cout<<' ';
    if(ma) cout<<ma+ne-1;
    else{
      cout<<pp[im++];
      if(im==ne){
	shuffle(pp+np, pp+ne, rng);
	im=np;
      }
    }
    cout<<'\n';
  }
  return 0;
}
