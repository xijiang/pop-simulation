/*******************************************************************************
 * Notes:
 * 0) this program is to simulate SNPs with no effect throught generations to
 *    reach drift-mutation equilibrium.
 * 1) mmut is mean number of mutations (mean of a poisson distribution) plus
 *    10 times of the SD.  When lambda is large, poisson distribution approx.
 *    normal distribution.
 * 2) using a comb sub to comb out fixed loci.  This greatly speeded the prog.
 * 3) boundary is checked all the time, but only at the end of splicing or 
 *    insertion, easing coding and gaining speed.
 * 4) SNP output is haplotype majored, i.e., it's haplotype by haplotype
 * 5) Modification to include float mutation rate (2011-.
 * 6) Exclude MKL from Intel compilers, 2016. ===>2.0
 *
 *                                       by Xijiang Yu, Dec. 5, 2010
 ******************************************************************************/
#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <cstdlib>
#include <algorithm>
#include <ctime>
#include <cmath>
#include <random>
#include <array>
#include <stdexcept>

using namespace std;

const int max_mutations (1.2e5);
const int tmp_vec_len   (2.4e5);
const int bp_per_morgan (1e8);
const int method_p      (0); //(VSL_RNG_METHOD_POISSON_PTPE); //back compatible
const int method_iu     (0); //(VSL_RNG_METHOD_UNIFORM_STD);
const int comb_period   (1000);
const int timer_period  (500);
const int return_period (timer_period*10);
int       tvec[tmp_vec_len];
typedef uniform_int_distribution<int> UID;

int insert(int *a, int m, int *b, int n){
//Insert b into a.  a is of length max_mutations, current length m;
// b of length n holds new mutations
// This sorting program will remove mutations that's same as an existing one.
// Since the uniform range is [0, 1e8], it's not easy to get repeats in new muts
  int i, j, k;
  i=j=k=0;

  while(j<n && i<m) {
    if(a[i]==b[j]) {
      ++j;
      continue;
    }
    tvec[k++]=(a[i]<b[j])?a[i++]:b[j++];
  }
  while(i<m) tvec[k++]=a[i++];
  while(j<n) tvec[k++]=b[j++];
  if(k>=max_mutations)
    throw runtime_error("Please increase max_mutations in the source code.");
  for(i=0; i<k; ++i) a[i]=tvec[i];
  return k;  //new haplotype length
}

int splice(int seg, int *a, int la, int *b, int lb, int *c, int lc, int *d){
//splice a of length la and  (a, b, c, & d are all sorted in ascending order)
//       b .. ...... lb into
//       d .. ...... ld afterworth, with splicing information
//       c .. ...... lc, ld is returned
//  seg is the initial haplotpye, either 0 for a or 1 for b.
  int ia, ib, ic, ld, i;
  ia=ib=ic=ld=0;
  while(ic<lc) {
    if(seg) {
      while(ia<la && a[ia]<c[ic]) ++ia;
      while(ib<lb && b[ib]<c[ic]) tvec[ld++]=b[ib++];
    }
    else {
      while(ia<la && a[ia]<c[ic]) tvec[ld++]=a[ia++];
      while(ib<lb && b[ib]<c[ic]) ++ib;
    }
    ++ic;
    seg=seg?0:1;
  }
  if(seg) while(ib<lb) tvec[ld++]=b[ib++];
  else    while(ia<la) tvec[ld++]=a[ia++];
  if(ld>=max_mutations)
    throw runtime_error("Please increase max_mutations in the source code.");
  for(i=0; i<ld; ++i) d[i]=tvec[i];
  return ld;  //spliced size
}

//comb out fixed loci
int comb(int *v, int n, int th, map<int, int> &ref){
  int i, c[n], m(0);
  // modified 2011.Oct.13, 0 freq loci are also removed
  for(i=0; i<n; ++i) if(ref[v[i]]<th && ref[v[i]]>0) c[m++]=v[i];
  for(i=0; i<m; ++i) v[i]=c[i];
  return m;
}

//output SNPs
void ooput(ostream &oo, int *vec, map<int,int> ref) {
  map<int,int>::iterator it;
  int i;
  for(i=0, it=ref.begin(); it!=ref.end(); ++it) {
    if(vec[i]!=it->first) oo<<0;
    else {
      oo<<1;
      ++i;
    }
  }
  oo<<'\n';
}

int main(int argc, char*argv[]) {
  if(argc!=5) {
    cerr<<"Usage: "<<argv[0]<<" ne ng chr mr"                  <<endl;
    cerr<<endl;
    cerr<<"    ne : integer, effective population size."       <<endl;
    cerr<<"    ng : integer, number of evolving generations."  <<endl;
    cerr<<"    chr: double, chromosome length (in M[organ])."  <<endl;
    cerr<<"    mr : double, mutation rate/meiosis/Morgan."     <<endl;
    cerr<<"    Results will be piped to pgqtl or pdrop"        <<endl;
    cerr<<endl;
    return 1;
  }
  //parameters
  int      ne (atoi(argv[1]));
  int      ng (atoi(argv[2]));
  double   chr(atof(argv[3]));
  double   mr (atof(argv[4]));

  //simulation storage
  int    SNPs[2][ne][2][max_mutations], *ptr;
  int    parent(0), offspring(1);
  int    igrt, iid, ihap, nhap, isnp, status(0);
  int    pama[ne], phap[ne]; //phap is the initial parent haplotype
  int    nSNP[2][ne][2];     // # mutation on each haplotype
  int    poisson[ne];
  int    mxpoint;            // max # and position of mutations/cross-overs
  int    ttbp;               // total # of base pairs
  int    i, k, mv, ml;
  double lambda_m, memory;

  // baking the random generator
  random_device   rdv;
  array<int, 624> seed_data;
  for(auto&x:seed_data) x=rdv();
  seed_seq seq(seed_data.begin(), seed_data.end());
  mt19937  rng(seq);
 
  //Different loci index
  map<int,int>           loci;
  map<int,int>::iterator ig;

  //check parameters:
  if(ne<4){
    clog<<"Population: [10, inf].\n";
    ++status;
  }
  if(ng<100) {
    clog<<"Generations: [100, integer limit].\n";
    ++status;
  }
  if(chr<0.00001 || chr>4) {
    clog<<"Chromosome length: [.00001, 4].\n";
    ++status;
  }
  if(mr<.1 || mr>10) {
    clog<<"Mutation rate: [.1, 10].\n";
    ++status;
  }
  if(status) return 2;

  clog<<"\n>>>>> Generating Ideal Population <<<<<\n";
  //Program initialization
  clog<<"\n--> Initializing of parameters & storage ... "<<flush;
  lambda_m = chr*mr;  //parameter for mutation, which follows poisson distr.
  if(mr<1)  mxpoint = int(ne*2*chr      + 10*sqrt(ne*2*chr));
  else      mxpoint = int(ne*2*lambda_m + 10*sqrt(ne*2*lambda_m));
  int point[mxpoint];

  ttbp=chr*bp_per_morgan;
  for(i=0; i<2; ++i)
    for(iid=0; iid<ne; ++iid)
      for(ihap=0; ihap<2; ++ihap)
	  nSNP[i][iid][ihap]=0;
  nhap=ne*2;
  clog<<"done."<<endl;

  //Memory usage
  memory  = ne * max_mutations * 2 * 2 * sizeof(int); //mutation storage
  memory += ne * 2 * sizeof(int);                     //sample pa & ma
  memory += ne * 2 * 2 * sizeof(int) * 2 * 2;         //# of mutations
  memory += ne * sizeof(int);                         //# cross-over/muts
  memory += mxpoint * sizeof(int);                    //points storage
  memory += max_mutations * 2 * sizeof(int);     //for insertion and combing
  memory /= (1024*1024);
  memory += 67;                       //Rest memory usage, checked with `top`
  clog<<"--------------> Approximate memory usage ... ";
  clog<<ceil(memory)<<" M\n"<<endl;
  
  //Let's create ...
  UID unifA(0, ttbp), unifB(0, ne/2-1), unifC(ne/2, ne-1), unifD(0,1);
  poisson_distribution<int>     pssn(lambda_m), pchr(chr);
  clog<<"----------------------------> Simulating ..."<<endl;
  //  clog<<"     ";
  for(igrt=0; igrt<ng; ++igrt) {
    //create mutations in parents
    for(ihap=0; ihap<2; ++ihap) {
      //how many mutations
      for(auto&p:poisson) p = pssn(rng);
      //where are they
      for(auto&p:point) p = unifA(rng);
      //insert the mutations
      for(iid=i=0; iid<ne; ++iid) {
	sort(&point[i], &point[i]+poisson[iid]);
	try {
	  nSNP[parent][iid][ihap]
	    =insert(SNPs[parent][iid][ihap],
		    nSNP[parent][iid][ihap],
		    &point[i], poisson[iid]);
	}
	catch(runtime_error e) {
	  cerr<<e.what()<<endl;
	  return 3;
	}
	i+=poisson[iid];
      }
    }
    //creating next generation
    //splicing
    for(ihap=0; ihap<2; ++ihap) {
      //sample pa/ma from parent generation
      if(ihap) for(auto&p:pama) p = unifC(rng);
      else     for(auto&p:pama) p = unifB(rng);
      //the first segment to drop
      for(auto&p:phap) p = unifD(rng);
      //number of cross-overs
      for(auto&p:poisson) p=pchr(rng);
      //where are they
      for(auto&p:point) p = unifA(rng);
      
      for(iid=i=0; iid<ne; ++iid) {
	sort(&point[i], &point[i]+poisson[iid]);
	try {
	  nSNP[offspring][iid][ihap]
	    =splice(phap[iid],
		    SNPs[parent][pama[iid]][0],
		    nSNP[parent][pama[iid]][0],
		    SNPs[parent][pama[iid]][1],
		    nSNP[parent][pama[iid]][1],
		    &point[i], poisson[iid],
		    SNPs[offspring][iid][ihap]);
	}
	catch(runtime_error e) {
	  cerr<<e.what()<<endl;
	  return 3;
	}
	i+=poisson[iid];
      }
    }
    //switch generation
    status    = parent;
    parent    = offspring;
    offspring = status;

    //comb the loci, i.e. remove fixed loci.  500 can be changed to speed up
    if(!((igrt+1)%comb_period)||igrt==ng-1) {
      for(iid=0; iid<ne; ++iid)
	for(ihap=0; ihap<2; ++ihap) {
	  ptr = SNPs[parent][iid][ihap];
	  k   = nSNP[parent][iid][ihap];
	  for(isnp=0; isnp<k; ++isnp)
	    ++loci[ptr[isnp]];
	}
      for(iid=0; iid<ne; ++iid)
	for(ihap=0; ihap<2; ++ihap)
	  nSNP[parent][iid][ihap] = comb(
					 SNPs[parent][iid][ihap],
					 nSNP[parent][iid][ihap],
					 nhap,loci);
      loci.clear();
    }
    // program timer
    if((igrt+1)%timer_period  == 0)
      clog<<setw(6)<<igrt+1;
    if((igrt+1)%return_period == 0)
      clog<<endl;
  }
  clog<<endl;

  //post simulation
  for(iid=mv=0; iid<ne; ++iid)
    for(ihap=0; ihap<2; ++ihap) {
      ptr = SNPs[parent][iid][ihap];
      k   = nSNP[parent][iid][ihap];
      for(isnp=0; isnp<k; ++isnp)
	++loci[ptr[isnp]];
      if(mv<k) mv=k;
    }
  clog<<"---> Max haplotype storage vector length ... "<<mv<<endl;

  //parameters to be piped to next program
  cout<<ne<<endl<<loci.size()<<endl;
  // modified <2011.Oct.13>, output the genotypes first
  //create genotype file
  for(iid=0; iid<ne; ++iid)
    for(ihap=0; ihap<2; ++ihap)
      ooput(cout, SNPs[parent][iid][ihap], loci);

  //creating linkage map
  for(ml=0, ig=loci.begin(); ig!=loci.end(); ++ig)
    if(ig->second<nhap)
      cout<<setw(9)<<ig->first<<setw(5)<<ig->second<<'\n';

  clog<<" done.\n"<<endl;

  return 0;
}
