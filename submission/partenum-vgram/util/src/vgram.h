/*
  $Id: gram.h 5713 2010-09-09 03:11:22Z abehm $

  Copyright (C) 2010 by The Regents of the University of California

  Redistribution of this file is permitted under the terms of the 
  BSD license.
  
  Date: 01/30/2007
  Author: Rares Vernica <rares (at) ics.uci.edu>
*/

#ifndef _vgram_h_
#define _vgram_h_

#include <map>
#include <set>
#include <string>
#include <vector>

#include <tr1/functional>
#include <tr1/unordered_map>

#include "array.h"

using namespace std;
using namespace tr1;

typedef unordered_map <string, unsigned> GramHash;
typedef unordered_map <unsigned, Array<unsigned>*> GramHashInfo; //gram-> [dataID,pos,freq]
// get gram list: string -> pick longest first... whether ti - tio - tion (4 vs 3 would require a higher freq compared to 3 vs 2)
// I guess we don't need to prune.
// string -> (grams,pos)
typedef unordered_map <unsigned, Array<unsigned>*> GramListMap;
typedef unordered_map <unsigned, unsigned> GramLengthMap;
typedef unordered_map <string, vector<vector<unsigned> > > StringGramPos;
typedef unordered_map <string, vector<unsigned> > NagMap;
// typedef map <unsigned, Array<unsigned>*> vGramListMap;

const unsigned char PREFIXCHAR = 156; // pound
const unsigned char SUFFIXCHAR = 190; // yen

extern hash<string> hashString;

// convert a string to a BAG of grams
void str2grams(const string &s, vector<string> &res,
               unsigned q = 3,
               unsigned char st = PREFIXCHAR, unsigned char en = SUFFIXCHAR);

// convert a string to a BAG of hashed grams
void str2grams(const string &s, vector<unsigned> &res, 
               unsigned q = 3,
               unsigned char st = PREFIXCHAR, unsigned char en = SUFFIXCHAR); 

// convert a string to a BAG (multiset) of grams
void str2grams(const string &s, multiset<string> &res, 
               unsigned q = 3,
               unsigned char st = PREFIXCHAR, unsigned char en = SUFFIXCHAR);

// convert a string to a BAG (multiset) of hashed grams
void str2grams(const string &s, multiset<unsigned> &res, 
               unsigned q = 3,
               unsigned char st = PREFIXCHAR, unsigned char en = SUFFIXCHAR);

// convert a string to a SET of grams
void str2grams(const string &s, set<string> &res, 
               unsigned q = 3,
               unsigned char st = PREFIXCHAR, unsigned char en = SUFFIXCHAR);

// convert a string to a SET of hashed grams
void str2grams(const string &s, set<unsigned> &res, 
               unsigned q = 3,
               unsigned char st = PREFIXCHAR, unsigned char en = SUFFIXCHAR);

// convert a string to a SET of hashed grams with count
void str2grams(const string &s, map<unsigned, unsigned> &res,
               unsigned q = 3, 
               unsigned char st = PREFIXCHAR, unsigned char en = SUFFIXCHAR);

// convert a string to a BAG of hashed grams without prefix and postfix 
void str2gramsNoPrePost(const string &s, vector<unsigned> &res,  unsigned q);

// convert a string to a SET of grams without prefix and postfix
void str2gramsNoPrePost(const string &s, set<string> &res, unsigned q);

// convert a string to a SET of hashed grams without prefix and postfix 
void str2gramsNoPrePost(const string &s, set<unsigned> &res,  unsigned q);

// in the future, if we want to add positional information, we can 
// just change the type of "string" to "pair<string, unsigned>"

//convert strings to inverted lists with id and position information
// Please do not forget to delete space in map in your own code
// If create grams without prefix and suffic, please set addStEn = false
void createIdPosInvertedLists(const vector<string> data, bool addStEn,
                              GramListMap &idLists, //StringGramPos &posLists, 
                              GramListMap &posLists,
                              GramLengthMap &lLists,
                              unsigned qmin = 3,
                              unsigned qmax = 3,
                              unsigned char st = PREFIXCHAR, 
                              unsigned char en = SUFFIXCHAR); 


// Get special grams which contains in "ch" set
// this function is used in synonym work
void getSpecialGrams(const string &s, unsigned qmin, unsigned qmax, const vector<char> ch,
                     set<unsigned> &res); 

// convert a list of grams to the corresponding string
void grams2str(const vector<string> &v, string &s, const unsigned qmin = 3, unsigned qmax = 3);

unsigned gram2id(const string &gram); // get ID from gram
void id2gram(unsigned id, string &res,
             const unsigned qmin = 3, unsigned qmax = 3); // get ID from unsigned

// extern hash<string> hashString;

class VGramID                    // grams as IDs in a vector with all possible grams
{
public:
  VGramID(unsigned qmin = 3,
         unsigned qmax = 3,
         unsigned rqf = 1,
         char st = PREFIXCHAR,
         char en = SUFFIXCHAR, 
         const string &charset = charsetEn,
         bool withPerm = true
         );
  VGramID(const string &filenamePreffix);

  void saveData(const string &filenamePreffix) const;

  unsigned getQ() const { return q; }
  unsigned getCharsetLen() const { return charsetLen; }
  unsigned getN() const { return n; }

  unsigned getId(const string &gram) const; // get ID from gram
  string getGram(unsigned id) const; // get gram from ID

  void pruneGetIds(const string &s, vector<unsigned> &ids, 
  GramListMap &idLists, StringGramPos &posLists, GramListMap &freqLenLists);

  void getIds(const string &s, vector<unsigned> &ids, vector<unsigned> &pids
  ,GramListMap &idLists, GramListMap &posLists);
  // convert string to list of gram IDs
  void getGrams(const vector<unsigned> &ids, vector<string> &grams) const;
  // convert list of gram IDs to list of grams

  bool consistData(const string &filenamePrefix, const string &filenameExt) const;

  bool operator==(const VGramID& g) const;

  static const string charsetEn; // English character

private:
  unsigned q;
  unsigned qmin;                   // length of grams
  unsigned qmax; 
  unsigned rareqminFreq;
  char st, en;                  // start and end char for grams
                                // (e.g., PREFIXCHAR and SUFFIXCHAR)
  string charset;               // possible characters
  unsigned charsetLen;
  unsigned n;                   // length of vector with all possible grams
  vector<unsigned> perm;        // permutation for gram IDs

  static const unsigned charsetLenMax; // max length of the charset
  static const string gramidSuffix;

  void loadData(const string &filenamePrefix);

  unsigned invPerm(unsigned id) const;
};

// convert a string to a list of words
void str2words(const string &s, vector<string> &res, const string &delims = " \t");

// Word Index
typedef set<unsigned> Ids;
typedef pair<string, Ids>  WordEntry;

// version 1
typedef unordered_map<string, Ids> WordHash;

// version 2
typedef vector<WordEntry> WordIds;
typedef unordered_map<string, unsigned> WordKey;

class WordIndex 
{
public:
  static void build(const vector<string> &data, WordHash &wordHash);
  static void build(const string &filenameDataset, WordHash &wordHash);

  static void build(const vector<string> &data,
                    WordIds &wordIds, WordKey &wordKey);
  static void build(const string &filenameDataset,
                    WordIds &wordIds, WordKey &wordKey);

  static void save(const string &filenameWords, 
                   const string &filenameIds,
                   const WordHash &wordHash);
  static void load(const string &filenameWords, 
                   const string &filenameIds, 
                   WordHash &wordHash);

  static void save(const string &filenameWids, 
                   const string &filenameWkey,
                   const WordIds &wordIds, const WordKey &wordKey);
  static void load(const string &filenameWids, 
                   const string &filenameWkey, 
                   WordIds &wordIds, WordKey &wordKey);
  static bool exist(const string &filename1, const string &filename2);


  static void build(const vector<string> &data,
                    vector<string> &wordVect, vector<Ids> &idsVect,
                    WordKey &wordPosMap);
  static void save(const string &filename, const vector<string> &wordVect,
                   const vector<Ids> &idsVect, const WordKey &wordPosMap);
  static void load(const string &filename, vector<string> &wordVect,
                   vector<Ids> &idsVect, WordKey &wordPosMap);
};

#endif
