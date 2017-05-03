

#include <cassert>
#include <iostream>
#include <vector>

#include "partenum.h"
#include "util/src/output.h"

using namespace std;

void testPartEnum() 
{
  unsigned t = 0;

  vector<string> data;
  data.push_back("abc");
  data.push_back("ac");
  data.push_back("xyz");

  PartEnum pSave(data, 1, 1, 3, 2);
  pSave.build();
  pSave.saveIndex("partenum"); // save
  PartEnum pLoad(data, "partenum"); // load
  assert(pSave == pLoad); t++;

  vector<unsigned> searchCor, searchRes;
  searchCor.push_back(0);
  searchCor.push_back(1);
  pSave.search("ab", searchRes); // search
  assert(searchCor == searchRes); t++;

  cout << "partenum (" << t << ")" << endl;
}

int main()
{
  cout << "test..." << endl;

  testPartEnum();

  cout << "OK" << endl;
}
