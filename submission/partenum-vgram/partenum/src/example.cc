
#include <iostream>

#include "partenum.h"
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
    filenameData = "dataset/actor.txt", 
    // filenameData = "dataset/gene.txt", 
    filenamePartEnum = "partenum";

  vector<string> data;
  readString(data, filenameData);

  double start_time;
  

  const unsigned
    editdist = 2, 
    q = 2, 
    n1 = 3, 
    n2 = 6;

  cout << " Parameters: ed="<< editdist <<" q="<< q <<" n1="<< n1 <<" n2="<< n2 << endl;
  start_time = getT();

  PartEnum h(data, q, editdist, n1, n2);

  // 1. pre-processing (generate signatures)

  h.build();

  // 2. save

  h.saveIndex(filenamePartEnum);


  cout << " Build and save time: " << (getT()-start_time) << endl;
  start_time = getT();

  // 3. use

  vector<unsigned> results;
  // h.search("dsfnk", editdist, results);
  h.search("edie marsen", editdist, results);
  
  cout << " Search time: " << (getT()-start_time) << endl;

  // cout << " Strings similar to haunted: " << endl << endl;
  for (vector<unsigned>::const_iterator i = results.begin(); i != results.end(); ++i)
    cout << data[*i] << endl;
}
