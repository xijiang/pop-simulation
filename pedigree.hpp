#ifndef _xy_pedigree_
#define _xy_pedigree_ 1
#include <vector>

class parent{
public:
  int pa;
  int ma;
  parent(int a, int b):pa(a),ma(b){};
};

void amatrix(std::vector<parent>&, double*);
#endif
