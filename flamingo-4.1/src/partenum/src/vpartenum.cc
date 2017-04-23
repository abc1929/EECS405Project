/*
  $Id: vPartEnum.cc 5769 2010-10-19 06:17:00Z abehm $

  Copyright (C) 2007 by The Regents of the University of California
 
  The implementation of the vPartEnum algorithm invented by Microsoft
  researchers is limited to non commercial use, which would be covered
  under the royalty free covenant that Microsoft made public.

  Date: 01/31/2007
  Author: Rares Vernica <rares (at) ics.uci.edu>

  Uses: http://www.boost.org/doc/html/hash.html
*/

#include "vpartenum.h"

#include <sys/time.h>
#include <sys/stat.h>

#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <set>

#include <boost/functional/hash.hpp>

#include "util/src/simfuncs.h"
#include "util/src/input.h"
#include "util/src/misc.h"

using namespace std;

const unsigned vPartEnum::siglenMax = 100;
const string vPartEnum::paramSuffix = ".pe.param.bin";
const string vPartEnum::signSuffix = ".pe.sign.bin";

vPartEnum::vPartEnum(const vector<string> &data, 
                   unsigned qmin, unsigned qmax, unsigned editdist, unsigned n1, unsigned n2, unsigned rqf):
  data(&data),
  vgramId(vGramId(qmin,qmax,rqf)),
  k(2 * editdist *  qmax),
  k2((k + 1) / n1 - 1),
  n1(n1),
  n2(n2),
  subs(subsets(n2, n2 - k2)), 
  datalen(this->data->size()), 
  siglen(subs.size() * n1), 
  buckets(new SigsBucket[siglen]),
  idL(new GramListMap[]),
  posL(new GramListMap[]),
  freqLenL(new GramListMap[]),
  nag(new unordered_map <string, vector<unsigned>>)
{
  if (siglen > siglenMax) {
    cerr << "siglen " << siglen << " greater than siglenMax " << siglenMax << endl;
    exit(EXIT_FAILURE);
  }
}

vPartEnum::vPartEnum(const vector<string> &data, const string &filenamePrefix):
  data(&data),
  vgramId(vGramId(filenamePrefix)),
  datalen(this->data->size())
{
  loadIndex(filenamePrefix);
}

vPartEnum::~vPartEnum() 
{
  for (unsigned i = 0; i < siglen; i++)
    for (SigsBucket::const_iterator it = buckets[i].begin();
         it != buckets[i].end(); ++it)
      delete[] it->second;
  delete[] buckets;
}

void vPartEnum::build() 
{
  // sign
  cerr << "sign";
  unsigned *sigs = new unsigned[datalen * siglen];
  unsigned i = 0;
  // building grams dictionary 
  createIdPosInvertedLists(*data, 1, idL, posL, freqLenL, qmin, qmax);
  for (vector<string>::const_iterator it = data->begin(); 
       it != data->end(); ++it) {
    if (i % 10000 == 0) {
      cerr << '.'; cerr.flush();
    }

    buildsign(*it, sigs + i);
    i += siglen;    
  }
  cerr << "OK" << endl;

  // bucket
  cerr << "bucket";

  // time efficient
  for (unsigned j = 0; j < siglen; j++) {
    if (j % siglen / 10 == 0) {
      cerr << '.'; cerr.flush();
    }
    map<unsigned, vector<unsigned> > bucketsVec;

    for (unsigned i = 0; i < datalen; i++)
      bucketsVec[sigs[i * siglen + j]].push_back(i);

    for (map<unsigned, vector<unsigned> >::const_iterator
           it = bucketsVec.begin(); it != bucketsVec.end(); ++it) {
      unsigned sz = 1 + it->second.size();
      unsigned *bucket = new unsigned[sz];
      unsigned i = 0;
      bucket[i++] = sz;
      for (vector<unsigned>::const_iterator jt = it->second.begin();
           jt != it->second.end(); ++jt)
        bucket[i++] = *jt;
      buckets[j][it->first] = bucket;
    }
    buckets[j].rehash(static_cast<size_t>(buckets[j].load_factor()));
  }

  // memory efficient
  /*
    for (i = 0; i < siglen; i++) {
    cerr << '.'; cerr.flush();

    map<unsigned, unsigned> bucketSz;
    for (unsigned j = 0; j < datalen; j++)
    bucketSz[sigs[j * siglen + i]]++;
    
    for (map<unsigned, unsigned>::const_iterator sz = bucketSz.begin();
    sz != bucketSz.end(); ++sz) {
    unsigned *bucket = new unsigned[sz->second + 1];
    unsigned k = 0;
    bucket[k++] = sz->second + 1;
    for (unsigned j = 0; j < datalen; j++)
    if (sigs[j * siglen + i] == sz->first)
    bucket[k++] = j;
      
    buckets[i][sz->first] = bucket;
    }
    bucketSz.clear();
    }
  */

  cerr << "OK" << endl;
  
  delete[] sigs;
  delete[] idL;
  delete[] posL;
  delete[] freqLenL;
}

void vPartEnum::saveIndex(const string &filenamePrefix) const
{
  // save gramid
  vgramId.saveData(filenamePrefix);

  // save param
  string filenameParam = filenamePrefix + paramSuffix;
 
  cerr << "write to \"" << filenameParam << "\"...";
  cerr.flush();

  ofstream fileParam(filenameParam.c_str(), ios::out | ios::binary);
  if (!fileParam) {
    cerr << "can't open output file \"" << filenameParam << "\"" << endl;
    exit(EXIT_FAILURE);
  }

  fileParam.write(reinterpret_cast<const char*>(&k), sizeof(unsigned));
  if (fileParam.fail()) writeerror(filenameParam);

  fileParam.write(reinterpret_cast<const char*>(&k2), sizeof(unsigned));
  if (fileParam.fail()) writeerror(filenameParam);
  
  fileParam.write(reinterpret_cast<const char*>(&n1), sizeof(unsigned));
  if (fileParam.fail()) writeerror(filenameParam);

  fileParam.write(reinterpret_cast<const char*>(&n2), sizeof(unsigned));
  if (fileParam.fail()) writeerror(filenameParam);

  fileParam.close();

  cerr << "OK" << endl;

  // save sigs
  string filenameSigs = filenamePrefix + signSuffix;

  cerr << "write to \"" << filenameSigs << "\"...";
  cerr.flush();

  ofstream fileSigs(filenameSigs.c_str(), ios::out | ios::binary);
  if (!fileSigs) {
    cerr << "can't open output file \"" << filenameSigs << "\"" << endl;
    exit(EXIT_FAILURE);
  }
  for (unsigned i = 0; i < siglen; i++) {
    unsigned sz = buckets[i].size();
    fileSigs.write(reinterpret_cast<const char*>(&sz), 
                   sizeof(unsigned));
    if (fileSigs.fail()) writeerror(filenameSigs);

    for (SigsBucket::const_iterator bucket = buckets[i].begin();
         bucket != buckets[i].end(); ++bucket) {

      fileSigs.write(reinterpret_cast<const char*>(&bucket->first), sizeof(unsigned));
      if (fileSigs.fail()) writeerror(filenameSigs);

      unsigned *sigs = bucket->second;

      fileSigs.write(reinterpret_cast<const char*>(&sigs[0]), sizeof(unsigned));
      if (fileSigs.fail()) writeerror(filenameSigs);
    
      for (unsigned j = 1; j < bucket->second[0]; j++) {
        fileSigs.write(reinterpret_cast<const char*>(&sigs[j]),
                       sizeof(unsigned));
        if (fileSigs.fail()) writeerror(filenameSigs);
      }
    }
  }
  fileSigs.close();

  cerr << "OK" << endl;
}

void vPartEnum::loadIndex(const string &filenamePerfix)
{
  if (!consistIndex(filenamePerfix)) {
    cerr << "index files are not consistent" << endl;
    exit(EXIT_FAILURE);
  }

  // load param
  string filenameParam = filenamePerfix + paramSuffix;

  cerr << "read from \"" << filenameParam << "\"...";
  cerr.flush();
 
  ifstream fileParam(filenameParam.c_str(), ios::in | ios::binary);
  if (!fileParam) {
    cerr << "can't open input file \"" << filenameParam << "\"" << endl;
    exit(EXIT_FAILURE);
  }

  fileParam.read(reinterpret_cast<char*>(&k), sizeof(unsigned));
  if (fileParam.eof()) readerror(filenameParam);

  fileParam.read(reinterpret_cast<char*>(&k2), sizeof(unsigned));
  if (fileParam.eof()) readerror(filenameParam);
  
  fileParam.read(reinterpret_cast<char*>(&n1), sizeof(unsigned));
  if (fileParam.eof()) readerror(filenameParam);

  fileParam.read(reinterpret_cast<char*>(&n2), sizeof(unsigned));
  if (fileParam.eof()) readerror(filenameParam);

  fileParam.close();

  cerr << "OK" << endl;

  subs = subsets(n2, n2 - k2);
  siglen = subs.size() * n1;  
  buckets = new SigsBucket[siglen];

  // load sigs
  string filenameSigs = filenamePerfix + signSuffix;

  cerr << "read from \"" << filenameSigs << "\"...";
  cerr.flush();

  ifstream fileSigs(filenameSigs.c_str(), ios::in | ios::binary);
  if (!fileSigs) {
    cerr << "can't open input file \"" << filenameSigs << "\"" << endl;
    exit(EXIT_FAILURE);
  }

  for (unsigned i = 0; i < siglen; i++) {
    unsigned bucketNo;

    fileSigs.read(reinterpret_cast<char*>(&bucketNo), sizeof(unsigned));
    if (fileSigs.eof()) readerror(filenameSigs);

    for (unsigned j = 0; j < bucketNo ; j++) {
      unsigned key;

      fileSigs.read(reinterpret_cast<char*>(&key), sizeof(unsigned));
      if (fileSigs.eof()) readerror(filenameSigs);

      unsigned sz;

      fileSigs.read(reinterpret_cast<char*>(&sz), sizeof(unsigned));
      if (fileSigs.eof()) readerror(filenameSigs);
      
      unsigned *bucket = new unsigned[sz];
      bucket[0] = sz;
      
      for (unsigned k = 1; k < sz; k++) {
        fileSigs.read(reinterpret_cast<char*>(bucket + k), sizeof(unsigned));
        if (fileSigs.eof()) readerror(filenameSigs);
      }
      
      buckets[i][key] = bucket;
    }
  }

  fileSigs.close();

  cerr << "OK" << endl;
}

bool vPartEnum::consistIndex(const string &filenamePrefix) const
{
  string filenameParam = filenamePrefix + paramSuffix;
  string filenameSign = filenamePrefix + signSuffix;

  if (!vgramId.consistData(filenamePrefix, filenameParam))
    return false;

  struct stat attribParam, attribSign;      
  if (stat(filenameParam.c_str(), &attribParam)) {
    cerr << "can't stat file \"" << filenameParam << "\"" << endl;
    exit(EXIT_FAILURE);
  }
  if (stat(filenameSign.c_str(), &attribSign)) {
    cerr << "can't stat file \"" << filenameSign << "\"" << endl;
    exit(EXIT_FAILURE);
  }

  if (!(attribParam.st_mtime <= attribSign.st_mtime))
    return false;

  return true;  
}

//----------------------------------Search Starts-----------------------------------//


void vPartEnum::vsearch(const string &query, vector<unsigned> &results) 
{
  vsearch(query, getEditdist(), results);
}

void vPartEnum::vsearch(const string &query, const unsigned editdist, 
                      vector<unsigned> &results)
{
  if (editdist > getEditdist()) {
    cerr << "vPartEnum::search editdist greater than editdist in constructor"
         << endl;
    exit(EXIT_FAILURE);
  }

  unsigned sig[siglen], *sigs;
  unsigned i, j, id;
  unsigned char checksBit[datalen];

  memset(checksBit, 0, datalen);
  buildsign(query, sig);
  for (j = 0; j < siglen; j++) {
    SigsBucket::const_iterator pos = buckets[j].find(sig[j]);
    if (pos != buckets[j].end()) {
      sigs = pos->second;
      for (i = 1; i < sigs[0]; i++) {
        id = sigs[i];
        if (!checksBit[id]) {
          checksBit[id] = 1;
          if (VGRAMDistance(query, (*data)[id], k))
            results.push_back(id);
        }
      }
    }
  }
}


void vPartEnum::search(const string &query, vector<unsigned> &results) 
{
  search(query, getEditdist(), results);
}

void vPartEnum::search(const string &query, const unsigned editdist, 
                      vector<unsigned> &results)
{
  if (editdist > getEditdist()) {
    cerr << "vPartEnum::search editdist greater than editdist in constructor"
         << endl;
    exit(EXIT_FAILURE);
  }

  unsigned sig[siglen], *sigs;
  unsigned i, j, id;
  unsigned char checksBit[datalen];

  memset(checksBit, 0, datalen);
  sign(query, sig);
  for (j = 0; j < siglen; j++) {
    SigsBucket::const_iterator pos = buckets[j].find(sig[j]);
    if (pos != buckets[j].end()) {
      sigs = pos->second;
      for (i = 1; i < sigs[0]; i++) {
        id = sigs[i];
        if (!checksBit[id]) {
          checksBit[id] = 1;
          if (ed(query, (*data)[id], editdist))
            results.push_back(id);
        }
      }
    }
  }
}

//----------------------------------Search Ends-----------------------------------//


//----------------------------------VGRAM specific stuff Starts-----------------------------------//

void vPartEnum::NAG(const string &s, unsigned maxk, unordered_map <string, vector<unsigned>> &nag) 
{
  vector<unsigned> ids;
  vector<unsigned> nagsi;
  nagsi.resize(s.length() * 2 + 1);

  if(nag.find(s) == nag.end()){
      for(j = 0; j < s.length(); j++){
      auto x = posL[s][j][1]; //position
      for(auto i = x * 2 ; i <= (x + freqLenL[posL[s][j]].at(0)) * 2 + 2; i++){
        if(i>s.length() * 2 + 1)
          break;
        nagsi[i]++;
      }
    }
  }
  sort(nagsi.begin(),nagsi.end(),[](unsigned a, unsigned b) { return a > b; });
  for(auto i = 1; i <= maxk; i++){
    unsigned temp = 0;
    for(auto j = 1; j <= i){
      temp+=nagsi[j-1]
    }
    nag[s].push_back[temp];
  }
  
}

bool vPartEnum::VGRAMDistance(const string &s1, const string &s2, unsigned threshold)
{
  vector<unsigned> ids1;
  vector<unsigned> ids2;

  vgramId.pruneGetIds(s1, ids1 ,idL, posL, freqLenL);
  vgramId.pruneGetIds(s2, ids2 ,idL, posL, freqLenL);

  auto l1 = ids1.size();
  auto l2 = ids2.size();
  vector<unsigned> res(l1+l2);

  sort(ids1.begin(),ids1.end());
  sort(ids2.begin(),ids2.end());

  vector<unsigned>::iterator it;
  it = set_intersection (ids1.begin(),ids1.end(), ids2.begin(),ids2.end(), res.begin());
  res.resize(it-res.begin());

  return res.size()-(l1+l2-threshold)/2 >= 0;

}


//----------------------------------VGRAM specific stuff Ends-----------------------------------//


//----------------------------------Signs Starts-----------------------------------//

void vPartEnum::buildsign(const string &s, vector<unsigned> &sig) 
{
  unsigned sigP[siglen];
  buildsign(s, sigP);
  for (unsigned i = 0; i < siglen; i++)
    sig.push_back(sigP[i]);
}

void vPartEnum::buildsign(const string &s, unsigned *sig) 
{
  vector<unsigned> ids;
  vgramId.pruneGetIds(s, ids, idL, posL, freqLenL); 
  // vgramId.getIds(s, ids);
  vector<unsigned> sg;
  set<unsigned> p1;
  boost::hash<vector<unsigned> > vectorHash;
  unsigned k = 0;

  
  for (unsigned i = 0; i < n1; i++)
    for (vector<vector<unsigned> >::const_iterator sub = subs.begin();
         sub != subs.end(); ++sub) {
      p1.clear();
      for (vector<unsigned> ::const_iterator j = sub->begin();
           j != sub->end(); ++j)
        for (vector<unsigned>::const_iterator id = ids.begin();
             id != ids.end(); ++id) 
          if (*id >= begin(i, *j) && *id < end(i, *j))
            p1.insert(*id);
      sg.clear();
      sg.push_back(i);
      sg.insert(sg.end(), sub->begin(), sub->end());
      sg.insert(sg.end(), p1.begin(), p1.end());
      sig[k++] = vectorHash(sg);
    }
}



void vPartEnum::sign(const string &s, vector<unsigned> &sig) const 
{
  unsigned sigP[siglen];
  sign(s, sigP);
  for (unsigned i = 0; i < siglen; i++)
    sig.push_back(sigP[i]);
}

void vPartEnum::sign(const string &s, unsigned *sig) const 
{
  vector<unsigned> ids;
  vgramId.getIds(s, ids ,idL, posL, freqLenL);
  vector<unsigned> sg;
  set<unsigned> p1;
  boost::hash<vector<unsigned> > vectorHash;
  unsigned k = 0;
  
  for (unsigned i = 0; i < n1; i++)
    for (vector<vector<unsigned> >::const_iterator sub = subs.begin();
         sub != subs.end(); ++sub) {
      p1.clear();
      for (vector<unsigned> ::const_iterator j = sub->begin();
           j != sub->end(); ++j)
        for (vector<unsigned>::const_iterator id = ids.begin();
             id != ids.end(); ++id) 
          if (*id >= begin(i, *j) && *id < end(i, *j))
            p1.insert(*id);
      sg.clear();
      sg.push_back(i);
      sg.insert(sg.end(), sub->begin(), sub->end());
      sg.insert(sg.end(), p1.begin(), p1.end());
      sig[k++] = vectorHash(sg);
    }
}


//----------------------------------Signs Ends-----------------------------------//



bool vPartEnum::operator==(const vPartEnum &h) const 
{
  if (this == &h) 
    return true;
  if (vgramId == h.vgramId && 
      k == h.k && 
      k2 == h.k2 && 
      n1 == h.n1 && 
      n2 == h.n2 && 
      data == h.data && 
      subs == h.subs && 
      datalen == h.datalen && 
      siglen == h.siglen && 
      *buckets == *h.buckets)
    return true;
  return false;
}

bool operator==(const SigsBucket &b1, const SigsBucket &b2) 
{
  if (b1.size() != b2.size())
    return false;

  for (SigsBucket::const_iterator it = b1.begin();
       it != b1.end(); ++it) {
    SigsBucket::const_iterator jt = b2.equal_range(it->first).first;
    if (jt == b2.end())
      return false;
    if (it->second[0] != jt->second[0])
      return false;
    for (unsigned i = 1; i < it->second[0]; i++)
      if (it->second[i] != jt->second[i])
        return false;
  }

  for (SigsBucket::const_iterator it = b2.begin();
       it != b2.end(); ++it)
    if (b1.equal_range(it->first).first == b1.end())
      return false;
  
  return true;  
}
