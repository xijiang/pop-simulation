/**
 * When multiple chromosomes are simulated by either pgsnp or pdrop, this 
 * program can merge them into one file.
 * Columns are ID, rows are genotypes.
 *                                                  by Xijiang Yu
 *                                                  Aug. 02, 2016
 */
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
  if(argc==1){
    cerr << "Usage: "<<argv[0]<<" 1.gt 2.gt ... >merged.gt";
    return 1;
  }
  
  int      nch(argc-1);
  ifstream fin[nch];
  int      i, j, nid, nlc[nch], freq, tlc{0};
  string   ts;

  for(i=0; i<nch; ++i){
    fin[i].open(argv[i+1]);
    fin[i]>>nid>>nlc[i];
    tlc +=nlc[i];
  }
  cout<<nid<<' '<<tlc<<'\n';
  
  for(i=0; i<nch; ++i)
    for(j=0; j<nlc[i]; ++j) {
      fin[i]>>freq;
      cout<<freq<<'\n';
    }
  for(i=0; i<nch; ++i)
    for(j=0; j<nlc[i]; ++j){
      fin[i]>>ts;
      cout<<ts<<'\n';
    }
  return 0;
}
