#include <iostream>

#include "vpartenum.h"
#include "util/src/input.h"
#include "util/src/misc.h"
#include <time.h>


using namespace std;

double getT(){
  struct timespec tv;
  if(clock_gettime(CLOCK_REALTIME, &tv) != 0) return 0;
  return (tv.tv_sec + (tv.tv_nsec / 1000000000.0));
}

int main() 
{
  const string 
    // filenameData = "dataset.txt",
    // filenameData = "dataset/actor.txt", 
    filenameData = "dataset/gene.txt", 
    filenamePartEnum = "vpartenum";

  vector<string> data;
  readString(data, filenameData);
  double start_time;


  const unsigned
    editdist = 2, 
    qmin = 2,
    qmax = 2, 
    n1 = 3,//2, 
    n2 = 6,//5,
    rqf = 1;

  
  cout << " Parameters: ed="<< editdist <<" qmin="<< qmin<<" qmax="<< qmax <<" n1="<< n1 <<" n2="<< n2 << endl;
  start_time = getT();

  VPartEnum h(data, qmin, qmax, editdist, n1, n2 ,rqf);
  // cerr << "testtest";
  // 1. pre-processing (generate signatures)

  h.build();

  // 2. save

  h.saveIndex(filenamePartEnum);

  cout << " Build and save time: " << (getT()-start_time) << endl;
  start_time = getT();

  vector<unsigned> results;
  
  // h.vsearch("poice", editdist, results);
  h.vsearch("dsfnk", editdist, results);
  // h.vsearch("edie marsen", editdist, results);
  
  cout << " Search time: " << (getT()-start_time) << endl;

  for (vector<unsigned>::const_iterator i = results.begin(); i != results.end(); ++i)
    cout << data[*i] << endl;
    // cout << "TF" << endl;

  cout << "Total results of " << results.size() << endl;
}