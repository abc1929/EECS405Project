#include <iostream>

#include "vpartenum.h"
#include "util/src/input.h"
#include "util/src/misc.h"

using namespace std;

int main() 
{
  const string 
    // filenameData = "dataset.txt",
    filenameData = "dataset/actor.txt", 
    // filenameData = "dataset/gene.txt", 
    filenamePartEnum = "vpartenum";

  vector<string> data;
  readString(data, filenameData);

  const unsigned
    editdist = 5, 
    qmin = 2,
    qmax = 4, 
    n1 = 3,//2, 
    n2 = 8,//5,
    rqf = 1;

  

  VPartEnum h(data, qmin, qmax, editdist, n1, n2 ,rqf);
  cerr << "testtest";
  // 1. pre-processing (generate signatures)

  h.build();

  // 2. save

  h.saveIndex(filenamePartEnum);

  // 3. use

  vector<unsigned> results;
  
  // h.vsearch("poice", editdist, results);
  // h.vsearch("dsfnk", editdist, results);
  h.vsearch("edie marsen", editdist, results);
  
  for (vector<unsigned>::const_iterator i = results.begin(); i != results.end(); ++i)
    cout << data[*i] << endl;
    // cout << "TF" << endl;

  cout << "sdtest " << results.size() << endl;
}