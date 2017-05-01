/*
  $Id: PartEnum.cc 5769 2010-10-19 06:17:00Z abehm $

  Copyright (C) 2007 by The Regents of the University of California
 
  The implementation of the PartEnum algorithm invented by Microsoft
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

const unsigned VPartEnum::siglenMax = 100;
const string VPartEnum::paramSuffix = ".pe.param.bin";
const string VPartEnum::signSuffix = ".pe.sign.bin";

VPartEnum::VPartEnum(const vector<string> &data, 
                   unsigned qmin, unsigned qmax, unsigned editdist, unsigned n1, unsigned n2, unsigned rqf):
  data(&data),
  vGramID(VGramID(qmin,qmax,rqf)),
  k(2 * editdist *  qmin),
  edi(editdist),
  k2((k + 1) / n1 - 1),
  n1(n1),
  n2(n2),
  subs(subsets(n2, n2 - k2)), 
  datalen(this->data->size()), 
  siglen(subs.size() * n1), 
  buckets(new SigsBucket[siglen]),
  idL(*new GramListMap()),
  posL(*new GramListMap()),
  lL(*new GramLengthMap()),
  nag(*new NagMap()),
  qmin(qmin),
  qmax(qmax)
{
  cerr << "test1";
  if (siglen > siglenMax) {
    cerr << "siglen " << siglen << " greater than siglenMax " << siglenMax << endl;
    exit(EXIT_FAILURE);
  }
}

VPartEnum::VPartEnum(const vector<string> &data, const string &filenamePrefix):
  data(&data),
  vGramID(VGramID(filenamePrefix)),
  datalen(this->data->size())
{
  loadIndex(filenamePrefix);
}

VPartEnum::~VPartEnum() 
{
  for (unsigned i = 0; i < siglen; i++)
    for (SigsBucket::const_iterator it = buckets[i].begin();
         it != buckets[i].end(); ++it)
      delete[] it->second;
  delete[] buckets;
}

void VPartEnum::build() 
{
  // sign
  cerr << "sign";
  unsigned *sigs = new unsigned[datalen * siglen];
  unsigned i = 0;

  // building grams dictionary 
  createIdPosInvertedLists(*data, 0, idL, posL, lL, qmin, qmax);

  for (vector<string>::const_iterator it = data->begin(); 
       it != data->end(); ++it) {
    if (i % 1000 == 0) {
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
    if (j % siglen / 10000 == 0) {
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
  // delete[] &idL;
  // delete[] &posL;
  // delete[] &freqLenL;
}

void VPartEnum::saveIndex(const string &filenamePrefix) const
{
  // save gramid
  this->vGramID.saveData(filenamePrefix);

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

void VPartEnum::loadIndex(const string &filenamePerfix)
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

bool VPartEnum::consistIndex(const string &filenamePrefix) const
{
  string filenameParam = filenamePrefix + paramSuffix;
  string filenameSign = filenamePrefix + signSuffix;

  if (!this->vGramID.consistData(filenamePrefix, filenameParam))
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


void VPartEnum::vsearch(const string &query, vector<unsigned> &results) 
{
  vsearch(query, getEditdist(), results);
}

void VPartEnum::vsearch(const string &query, const unsigned editdist, 
                      vector<unsigned> &results)
{
  if (editdist > getEditdist()) {
    cerr << "VPartEnum::search editdist greater than editdist in constructor"
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
          
          if (ed(query, (*data)[id], editdist))
          // if (VGRAMDistance(query, (*data)[id], getEditdist()))
            results.push_back(id);
          
            
        }
      }
    }
  }
}


void VPartEnum::search(const string &query, vector<unsigned> &results) 
{
  search(query, getEditdist(), results);
}

void VPartEnum::search(const string &query, const unsigned editdist, 
                      vector<unsigned> &results)
{
  if (editdist > getEditdist()) {
    cerr << "VPartEnum::search editdist greater than editdist in constructor"
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
          if (ed(query, (*data)[id], editdist))
            results.push_back(id);
        }
      }
    }
  }
}

//----------------------------------Search Ends-----------------------------------//


//----------------------------------VGRAM specific stuff Starts-----------------------------------//


// loose bound calculating NAG, not correct
void VPartEnum::NAG(const string &s, unsigned maxk, NagMap &nag) 
{
  if(nag.find(s) == nag.end()){
    vector<unsigned> ids; // [g1,g2,g3..]
    vector<unsigned> pids; // [0,2,3..]
    vector<unsigned> nagsi;

    vGramID.getIds(s, ids ,pids ,idL, posL);
    nagsi.resize(s.length() * 2 + 1);

    // for(unsigned j = 0; j < pids.size(); j++){
    //   unsigned x = pids[j]; //
    //   unsigned y = 0;
    //   if(j == pids.size()-1){
    //      for(unsigned i = x * 2 ; i < nagsi.size(); i++){
    //       if(i > s.length() * 2 + 1){
    //         break;
    //       }
    //       nagsi[i]++;
    //     }
    //   } 
    //   else {
        //  y = pids[j+1];
        //  for(unsigned i = x * 2 ; i <= (y) * 2 + 2; i++){
        //   if(i > s.length() * 2 + 1){
        //     break;
        //   }
        //   nagsi[i]++;
        // }
    //   }
    // }

    if(s.length()<=qmin){
      for(unsigned i = 0; i < nagsi.size(); i++){
        if(i == 0 || i == nagsi.size()-1){
          nagsi[i] = 0;
        } else {
          nagsi[i] = 1;
        }
      }
    } else {
      for(unsigned i = 0; i < nagsi.size(); i++){
        nagsi[i] = 0;
      }
      // span of pids[j], lL[ids[j]] length
           
      for(unsigned j = 0; j < pids.size(); j++){
        unsigned x = pids[j];
        // for(unsigned i = 1; i < nagsi.size()-1; i++){
          unsigned lenx = 0;
          if(lL.find(ids[j]) == lL.end()){
            lenx = qmin;
          } else {lenx = lL[ids[j]];}
          unsigned y = x+lenx-1;
          for(unsigned i = x * 2 + 1; i <= (y) * 2 + 1; i++){
            if(i > s.length() * 2 + 1){
              break;
            }
            nagsi[i]++;
          }
        
        // }
      }

      cerr << "test nag ";
      for(unsigned x = 0; x < nagsi.size(); x++){
        cerr << nagsi[x];
      }
      cerr << endl;


      // for(unsigned i = 1; i < nagsi.size()-1; i++){
      //   nagsi[i] = 1;
      //   unsigned count = 0;
      //   for(unsigned j = 0; j < pids.size(); j++){
      //     // span of pids[j]

      //     if( j * 2 + 1  )
          
          

      //   }
      // }


    }



    sort(nagsi.begin(),nagsi.end(), greater<unsigned>()); //[](unsigned a, unsigned b) { return a > b; });
    for(unsigned i = 1; i <= maxk; i++){
      unsigned temp = 0;
      for(unsigned j = 1; j <= i; j++){
        temp+=nagsi[j-1];
      }
      // vector<unsigned> aa = new vector<unsigned>();
      // aa.push_back(temp);
      nag[s].push_back(temp);
    }
  }
  
}

bool VPartEnum::VGRAMDistance(const string &s1, const string &s2, unsigned threshold)
{
  vector<unsigned> ids1;
  vector<unsigned> ids2;
  vector<unsigned> nu;


  vGramID.getIds(s1, ids1, nu ,idL, posL);
  vGramID.getIds(s2, ids2, nu ,idL, posL);
  // this->vGramID.pruneGetIds(s1, ids1 ,idL, posL, freqLenL);
  // this->vGramID.pruneGetIds(s2, ids2 ,idL, posL, freqLenL);

  unsigned l1 = ids1.size();
  unsigned l2 = ids2.size();
  unsigned diff = l1 >= l2 ? l1 - l2 : l2 - l1;
  // cerr << "l1 size: " << l1 << endl;
  // cerr << "l2 size: " << l2 << endl;
  vector<unsigned> res(l1+l2);

  sort(ids1.begin(),ids1.end());
  sort(ids2.begin(),ids2.end());

  vector<unsigned>::iterator it;
  it = set_intersection (ids1.begin(),ids1.end(), ids2.begin(),ids2.end(), res.begin());
  res.resize(it-res.begin());
  unsigned prc;
  // NAG(s1, threshold, nag);
  // for (vector<unsigned>::const_iterator i = nag[s1].begin(); i != nag[s1].end(); ++i)
    // cout << nag["delmare"][0] << endl;
  prc = nag[s1][(threshold-1)]+nag[s2][(threshold-1)];

  unsigned te1 = l1>=nag[s1][(threshold-1)] ? l1-nag[s1][(threshold-1)] : 0;
  unsigned te2 = l2>=nag[s2][(threshold-1)] ? l2-nag[s2][(threshold-1)] : 0;
  unsigned prd;

  // cerr<< endl << "te1: " << te1 << " te2: " << te2 << " " << endl << endl;

  prd = (te1 >= te2) ? te1 : te2;
  if(prd <= 0){prd = 0;}  

  // cerr << "l1: " << l1 << " l2: " << l2 << "diff: "  << diff << " ";;
  // cerr << "nag1: " << nag[s1][(threshold-1)] << " nag2: " << nag[s2][(threshold-1)] ;
  // cerr << " prc: " << prc << " ";
  // cerr << " hamming: " << 2 * diff + (l1+l2-2*res.size()) << "  string: " << s2  << endl;
  // cerr << " common grams: " << res.size() << " ";
  // cerr << " lower bound: " << prd << " " << endl;
  // return (res.size()-(l1 + l2 - prc)/2 >= 0);

  //# of common grams >= 

  return (prc >= (2 * diff + l1+l2-2*res.size()) && res.size() > prd);

}


//----------------------------------VGRAM specific stuff Ends-----------------------------------//


//----------------------------------Signs Starts-----------------------------------//

void VPartEnum::buildsign(const string &s, vector<unsigned> &sig) 
{
  unsigned sigP[siglen];
  buildsign(s, sigP);
  for (unsigned i = 0; i < siglen; i++)
    sig.push_back(sigP[i]);
}

void VPartEnum::bhash(vector<unsigned> &sg, unsigned *sig, unsigned k) const{
  boost::hash<vector<unsigned> > vectorHash;
  sig[k++] = vectorHash(sg);
}

void VPartEnum::buildsign(const string &s, unsigned *sig) 
{
  vector<unsigned> ids;
  vector<unsigned> nu; 
  vGramID.getIds(s, ids, nu, idL, posL); // us
  // cerr << "ecksdee \n"; 
  NAG(s, getEditdist(), nag);
  // cerr << "buildsign test \n";  
  // VGramID.getIds(s, ids);
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
      // bhash(sg,sig,k);
    }
}



// void VPartEnum::sign(const string &s, vector<unsigned> &sig) 
// {
//   unsigned sigP[siglen];
//   sign(s, sigP);
//   for (unsigned i = 0; i < siglen; i++)
//     sig.push_back(sigP[i]);
// }

// void VPartEnum::sign(const string &s, unsigned *sig)
// {
//   vector<unsigned> ids;
//   this->vGramID.pruneGetIds(s, ids ,idL, posL, freqLenL);
//   vector<unsigned> sg;
//   set<unsigned> p1;
//   unsigned k = 0;
  
//   for (unsigned i = 0; i < n1; i++)
//     for (vector<vector<unsigned> >::const_iterator sub = subs.begin();
//          sub != subs.end(); ++sub) {
//       p1.clear();
//       for (vector<unsigned> ::const_iterator j = sub->begin();
//            j != sub->end(); ++j)
//         for (vector<unsigned>::const_iterator id = ids.begin();
//              id != ids.end(); ++id) 
//           if (*id >= begin(i, *j) && *id < end(i, *j))
//             p1.insert(*id);
//       sg.clear();
//       sg.push_back(i);
//       sg.insert(sg.end(), sub->begin(), sub->end());
//       sg.insert(sg.end(), p1.begin(), p1.end());
//       bhash(sg,sig,k);
//     }
// }


//----------------------------------Signs Ends-----------------------------------//



bool VPartEnum::operator==(const VPartEnum &h) const 
{
  if (this == &h) 
    return true;
  if (this->vGramID == h.vGramID && 
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
