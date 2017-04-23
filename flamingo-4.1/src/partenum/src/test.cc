#include <iostream>

#include "vpartenum.h"
#include "util/src/input.h"
#include "util/src/misc.h"

using namespace std;

int main() 
{
  const string 
    filenameData = "dataset.txt", 
    filenamePartEnum = "vpartenum";

  vector<string> data;
  readString(data, filenameData);

  const unsigned
    editdist = 4, 
    qmin = 2,
    qmax = 4, 
    n1 = 2, 
    n2 = 6,
    rqf = 1;



  vPartEnum h(data, qmin, qmax, editdist, n1, n2 ,rqf);

  // 1. pre-processing (generate signatures)

  h.build();

  // 2. save

  h.saveIndex(filenamePartEnum);

  // 3. use

  vector<unsigned> results;
  h.vsearch("velxfye", editdist, results);
  
  for (vector<unsigned>::const_iterator i = results.begin(); i != results.end(); ++i)
    cout << data[*i] << endl;
}