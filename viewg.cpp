/**
 * This is to view my G matrix output, which is in binary format.
 *                                                  by Xijiang Yu
 *                                                  Jul. 15, 2015
 */
#include <iostream>
#include <iomanip>
#include <cstdlib>

using namespace std;

union db_bin_io {
  char   ch[sizeof(double)];
  double db; //when compiled in the same environment, sizeof(double) is the same
};

int main(int argc, char *argv[])
{
  int  i, j, k, nid, w{6}, p{2};
  char ch;
  db_bin_io v;

  if(argc==2){
    p = atoi(argv[1]);
    w = 4+p;
  }
  cin>>nid;
  cin.get(ch);
  clog<<"Dimension: "<<nid<<'\n';
  
  cout.precision(p);
  cout<<fixed;
  for(i=0; i<nid; ++i){
    for(j=0; j<=i; ++j){
      for(k=0; k<8; ++k) cin.get(v.ch[k]);
      cout<<setw(w)<<v.db;
    }
    cout<<'\n';
  }
  
  return 0;
}
