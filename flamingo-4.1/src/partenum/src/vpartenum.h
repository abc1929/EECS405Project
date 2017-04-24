/*
  $Id: VPartEnum.h 5769 2010-10-19 06:17:00Z abehm $

  Copyright (C) 2007 by The Regents of the University of California

  The implementation of the VPartEnum algorithm invented by Microsoft
  researchers is limited to non commercial use, which would be covered
  under the royalty free covenant that Microsoft made public.

  Date: 01/31/2007
  Author: Rares Vernica <rares (at) ics.uci.edu>
*/

#ifndef _VPartEnum_h_
#define _VPartEnum_h_

#include <map>
#include <utility>
#include <vector>

#include <tr1/unordered_map>

#include "util/src/appsearch.h"
#include "util/src/vgram.h"

using namespace std;
using namespace tr1;

typedef unordered_map<unsigned, unsigned*> SigsBucket;

bool operator==(const SigsBucket &b1, const SigsBucket &b2);

class VGramID;

class VPartEnum: public AppSearch
{
public:
  VPartEnum(const vector<string> &data, 
           unsigned qmin, 
           unsigned qmax,
           unsigned editdist, 
           unsigned n1, 
           unsigned n2, 
           unsigned rqf);

  VPartEnum(const vector<string> &data, 
           const string &filename);
  // filename is used as a prefix for multiple filenames
  VGramID vGramID;
  GramListMap idL;
  StringGramPos posL;
  GramListMap freqLenL;
  NagMap nag;

  void build();
  void saveIndex(const string &filename) const;
  // filename is used as a prefix for multiple filenames

  void search(const string &query, vector<unsigned> &results);
  void search(const string &query, const unsigned editdist, vector<unsigned> &results);
  void vsearch(const string &query, vector<unsigned> &results);
  void vsearch(const string &query, const unsigned editdist, vector<unsigned> &results);

  void NAG(const string &s, unsigned maxk, NagMap &nag); 
  bool VGRAMDistance(const string &s1, const string &s2, unsigned threshold);

  ~VPartEnum();

  unsigned getQ() const { return vGramID.getQ(); }
  unsigned getEditdist() const { return k / vGramID.getQ() / 2; }
  unsigned getN1() const { return n1; }
  unsigned getN2() const { return n2; }
  unsigned getK() const { return k; }
  unsigned getK2() const { return k2; }
  unsigned getSignlen() const { return siglen; }


  void buildsign(const string &s, vector<unsigned> &sig);
  void buildsign(const string &s, unsigned *sig); 
  void bhash(vector<unsigned> &sg, unsigned *sig, unsigned k) const;
  void sign(const string &s, vector<unsigned> &sig);
  void sign(const string &s, unsigned *sig);

  bool operator==(const VPartEnum &h) const;
  unsigned qmin;
  unsigned qmax;

private:
  const vector<string> *data;
  unsigned k, k2, n1, n2;


  vector<vector<unsigned> > subs;
  unsigned datalen, siglen;
  SigsBucket *buckets;
  
  static const unsigned siglenMax;
  static const string paramSuffix, signSuffix;

  void loadIndex(const string &filenamePrefix);
  bool consistIndex(const string &filenamePrefix) const;

  unsigned begin(unsigned i, unsigned j) const { 
    return vGramID.getN() * (n2 * i + j) / n1 / n2; }
  unsigned end(unsigned i, unsigned j) const { 
    return vGramID.getN() * (n2 * i + j + 1) / n1 / n2; }
};

#endif
