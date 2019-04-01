/**
 * Below Cholesky factorization subroutine is my quick and dirty one.
 * Row majored.
 * One-dimension array is used to store a matrix.
 * Lower triangle and D store the original matrix.
 * Upper triangle is the factor.
 * Rank is returned as long.
 * No range check.
 *                                                  by Xijiang Yu
 *                                                  Aug. 28, 2016
 */
#include <cmath>
long chol(double*A, double*D, long dim){//A: the matrix; D: diagonals
  long   i, j, k, r, s; //long, so that dim can be up to 2^31
  double t;
  
  if(A[0] <= 0) return 0;
  for(i=0; i<dim; ++i) D[i] = A[i*dim+i]; //Diagonal backup

  //The first column
  A[0] = std::sqrt(D[0]);
  for(i=1; i<dim; ++i) A[i] = A[i*dim]/A[0];

  //The rest
  for(i=1; i<dim; ++i){
    s = i*dim + i;
    for(j=i; j<i*dim; j+=dim) A[s] -= A[j]*A[j];
    if(A[s] <= 0) return i;
    A[s] = std::sqrt(A[s]);  //the i-th diagonal

    //A(i,j), for i!=j
    for(j=i+1; j<dim; ++j){
      t = A[j*dim + i];
      r = j-i;
      for(k=j; k<i*dim; k+=dim) t -= A[k]*A[k-r];
      A[s+r] = t/A[s];
    }
  }
  return dim;
}
