/**
 * Usage:
 * g++ -O2 -o xamat amatrix.cpp xamat.cpp
 */
#include <iostream>
#include "pedigree.hpp"

int main(int argc, char *argv[])
{
  std::vector<parent>ped;
  int  pa, ma, n, i, j, k;

  while(std::cin>>pa>>ma) ped.push_back(parent(pa,ma));

  n = ped.size();
  
  double A[n*n];
  amatrix(ped, A);
  std::cout<<n<<'\n';
  for(i=0; i<n; ++i)
    for(j=0; j<=i; ++j)
      for(k=0; k<sizeof(double); ++k) std::cout<<((char*)&A[i*n+j])[k];
  
  return 0;
}
