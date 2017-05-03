/*
  $Id: gram.cc 6132 2012-02-22 21:53:14Z abehm $

  Copyright (C) 2010 by The Regents of the University of California

  Redistribution of this file is permitted under the terms of the 
  BSD license.

  Date: 01/30/2007
  Author: Rares Vernica <rares (at) ics.uci.edu>
*/

#include "vgram.h"

#include <sys/stat.h>

#include "input.h"
#include "misc.h"

hash<string> hashString;

//now vgram ,we have multiple sets of q-grams 
//this convert s to grams, but we need a trie
void str2grams(const string &s, vector<string> &res, unsigned q,
               unsigned char st, unsigned char en)
{
  const string sPad = string(q - 1, st) + s + string(q - 1, en);
  
  for (unsigned i = 0; i < s.length() + q - 1; i++)
    res.push_back(sPad.substr(i, q));  
}

void str2grams(const string &s, vector<unsigned> &res, 
               unsigned q, unsigned char st, unsigned char en) 
{
  string sPad = string(q - 1, st) + s + string(q - 1, en);
  
  for (unsigned i = 0; i < s.length() + q - 1; i++)
    res.push_back(hashString(sPad.substr(i, q)));
}

void str2grams(const string &s, multiset<string> &res, unsigned q, 
               unsigned char st, unsigned char en)
{
  const string sPad = string(q - 1, st) + s + string(q - 1, en);

  for (unsigned i = 0; i < s.length() + q - 1; i++)
    res.insert(sPad.substr(i, q));  
}

void str2grams(const string &s, multiset<unsigned> &res, 
               unsigned q, unsigned char st, unsigned char en) 
{
  string sPad = string(q - 1, st) + s + string(q - 1, en);
  
  for (unsigned i = 0; i < s.length() + q - 1; i++)
    res.insert(hashString(sPad.substr(i, q)));
}

void str2grams(const string &s, set<string> &res, unsigned q, 
               unsigned char st, unsigned char en)
{
  const string sPad = string(q - 1, st) + s + string(q - 1, en);

  for (unsigned i = 0; i < s.length() + q - 1; i++)
    res.insert(sPad.substr(i, q));  
}

void str2grams(const string &s, set<unsigned> &res, 
               unsigned q, unsigned char st, unsigned char en) 
{
  string sPad = string(q - 1, st) + s + string(q - 1, en);
  
  for (unsigned i = 0; i < s.length() + q - 1; i++)
    res.insert(hashString(sPad.substr(i, q)));
}

void str2grams(const string &s, map<unsigned, unsigned> &res, unsigned q, 
               unsigned char st, unsigned char en)
{
  const string sPad = string(q - 1, st) + s + string(q - 1, en);

  for (unsigned i = 0; i < s.length() + q - 1; i++)
    res[hashString(sPad.substr(i, q))]++;  
}

void str2gramsNoPrePost(const string &s, vector<unsigned> &res,  unsigned q) 
{
  for (unsigned i = 0; i < s.length() - q + 1; i++) {
    //ignore gram with space 
    string substring = s.substr(i, q);
    // string::size_type loc = substring.find(' ', 0);
    // if (loc == string::npos) 
    res.push_back(hashString(substring));
      
  }//end for
}

void str2gramsNoPrePost(const string &s, set<string> &res, unsigned q)
{
  if (s.length() < q) {
    cerr << "string length (" << s.length()
         << ") less than q (" << q << ")" << endl;
    exit(1);
  }
  
  for (unsigned i = 0; i < s.length() - q + 1; i++)
    res.insert(s.substr(i, q));  
}

void str2gramsNoPrePost(const string &s, set<unsigned> &res,  unsigned q) 
{
  for (unsigned i = 0; i < s.length() - q + 1; i++) {
    //ignore gram with space 
    string substring = s.substr(i, q);
    string::size_type loc = substring.find(' ', 0);
    if (loc == string::npos) 
      res.insert(hashString(substring));
  }//end for
}

//Get special grams which contains in "ch" set
//this function is used in synonym work

void getSpecialGrams(const string &s, const unsigned q, const vector<char> ch,
                     set<unsigned> &res) 
{
  for (unsigned i = 0; i < s.length() - q + 1; i++) {
    string substring = s.substr(i, q);
    for(unsigned i=0;i<ch.size();i++) {
      string::size_type loc = substring.find(ch.at(i), 0 );
      if( loc != string::npos ) 
        res.insert(hashString(substring));
    }//end for
  }//end for
}// getSpecialGrams


// Temporary structure for VGRAM


//convert strings to inverted lists with id and position information
//Please remember to release memory space in map in your own code!
// If create grams without prefix and suffic, please set addStEn = false
// void createIdPosInvertedLists(const vector<string> data, bool addStEn,
//                               GramListMap &idLists, //StringGramPos &posLists, 
//                               GramListMap &freqLenLists, 
//                               unsigned qmin, unsigned qmax,
//                               unsigned char st, unsigned char en)
// {
//   for(unsigned i=0;i<data.size();i++) {
//     // cerr << "data: " << i << "\n";
//     for (unsigned z=qmin;z<=qmax;z++) {
//       // cerr << "z is " << z;
//       vector<unsigned> gramCodes;
//       if (addStEn)
//         str2grams(data.at(i),gramCodes,z,st,en);
//       else
//         str2gramsNoPrePost(data.at(i),gramCodes,z);
      
//       for(unsigned j=0;j<gramCodes.size();j++) {
//         unsigned gram = gramCodes.at(j);
      
//         if (idLists.find(gram) == idLists.end()) {
//           // a new gram
//           Array<unsigned> *arrayGram = new Array<unsigned>();
//           arrayGram->append(i);
//           idLists[gram] = arrayGram;
          
//           // Array<vector<unsigned>> *arrayPos = new Array<vector<unsigned>>();
//           // vector<unsigned> tempq;
//           // tempq.push_back(gram);
//           // tempq.push_back(j);
//           // arrayPos->append(tempq);
//           // posLists[data.at(i)] = arrayPos;

//           Array<unsigned> *arrayFreqLen = new Array<unsigned>();
//           arrayFreqLen->append(z);
//           arrayFreqLen->append(1);
//           freqLenLists[gram] = arrayFreqLen;
//         }//end if
//         else {
//           Array<unsigned> *arrayGram = idLists[gram];
//           arrayGram->append(i);
          
//           // Array<unsigned> *arrayPos = posLists[gram];
//           // arrayPos->append(j);

//           Array<unsigned> *arrayFreqLen = freqLenLists[gram];
//           freqLenLists[gram]->at(1)++;
//         }//end else
//       }//end gram array loop  
//     }//end various length gram arrays loop
//   }//end for
// }//end  createIdPosInvertedLists

void createIdPosInvertedLists(const vector<string> data, bool addStEn,
                              GramListMap &idLists, GramListMap &posLists, GramLengthMap &lLists,
                              unsigned qmin, unsigned qmax,
                              unsigned char st, unsigned char en)
{
  for(unsigned i=0;i<data.size();i++) {
    for(unsigned z=qmin; z<=qmax; z++){
      

      vector<unsigned> gramCodes;
      // if (addStEn && z == qmin)
      if (addStEn){
        str2grams(data.at(i),gramCodes,z,st,en); 
      }
      else{
        if(data.at(i).length()<qmin){
          const string padded = string(qmin - data.at(i).length(), st) + data.at(i); 
          str2gramsNoPrePost(padded,gramCodes,qmin);
        } else {
          if(data.at(i).length()<z){
            break;
          }
          str2gramsNoPrePost(data.at(i),gramCodes,z);
        }

      }
        
        
      for(unsigned j=0;j<gramCodes.size();j++) {
        unsigned gram = gramCodes.at(j);
      
        if (idLists.find(gram) == idLists.end()) {
          // a new gram
          Array<unsigned> *arrayGram = new Array<unsigned>();
          arrayGram->append(i);
          idLists[gram] = arrayGram;
          
          Array<unsigned> *arrayPos = new Array<unsigned>();
          arrayPos->append(j);
          posLists[gram] = arrayPos;

          lLists[gram] = z;
        }//end if
        else {
          Array<unsigned> *arrayGram = idLists[gram];
          arrayGram->append(i);
          
          Array<unsigned> *arrayPos = posLists[gram];
          arrayPos->append(j); 
          
        }//end else
      }//end for  
    }
  }//end for

  //start pruning
  // for(unsigned i=0;i<data.size();i++) {
    
  //   vector<vector<unsigned> > gramsCodes; // [2 grams] [3 grams] [4 grams]  
    
  //   for(unsigned z=qmin; z<=qmax; z++){
  //     if (addStEn)
  //       str2grams(data.at(i),gramsCodes.at(z-qmin),z,st,en);
  //     else
  //       str2gramsNoPrePost(data.at(i),gramsCodes.at(z-qmin),z);
  //   }

  //     for(unsigned j=0;j<gramsCodes.at(z-qmin).size();j++) {
  //       unsigned gram = gramsCodes.at(z-qmin).at(j);

  //       Array<unsigned> *arrayGram = idLists[gram];
  //       arrayGram->append(i);
        
  //       Array<unsigned> *arrayPos = posLists[gram];
  //       arrayPos->append(j);
      
  //     //end for  
  //   }
  // }







}//end  createIdPosInvertedLists



void grams2str(const vector<string> &v, string &res, const unsigned q)
{
  res = "";
  
  for (unsigned i = 0; i < v.size() - q + 1; i++)
    res += v[i].substr(q - 1, 1);
}

unsigned gram2id(const string &gram) 
{
  unsigned
    id = 0, 
    gramLen = gram.length();
  string::size_type pos;
  for (unsigned i = 0; i < gramLen; i++) {
    pos = VGramID::charsetEn.find(gram[i]);
    if (pos == string::npos) {
      cerr << "can't find character '" << gram[i] << "'(" 
           << static_cast<unsigned>(gram[i]) << ") of gram \"" << gram
           << "\" in charset" << endl;
      exit(EXIT_FAILURE);
    }
    id += pow(VGramID::charsetEn.size(), gramLen - i - 1) * pos;
  }
  return id;
}

void id2gram(unsigned id, string &res, const unsigned qmin, const unsigned qmax)
{
  res = "";
  while (id > 0) {
    res = string(1, VGramID::charsetEn[id % VGramID::charsetEn.size()]) + res;
    id = id / VGramID::charsetEn.size();
  }
  while (res.length() < qmin)
    res = string(1, VGramID::charsetEn[0]) + res;
}

// GramId
const unsigned VGramID::charsetLenMax = 500;
const string VGramID::charsetEn = 
  " !\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~0123456789abcdefghijklmnopqrstuvwxyz";
const string VGramID::gramidSuffix = ".gid.bin";

VGramID::VGramID(unsigned qmin, unsigned qmax, unsigned rqf, char st, char en, 
               const string &charset, bool withPerm):
  q(qmin),
  qmin(qmin),
  qmax(qmax),
  st(st),
  en(en),
  charset(string(1, st) + charset + string(1, en)), 
  charsetLen(this->charset.size()), 
  n(pow(charsetLen, qmin+2)), 
  perm(vector<unsigned>(n)),
  rareqminFreq(rqf)
{
  for (unsigned i = 0; i < n; i++)
    perm[i] = i;
  if (withPerm)
    random_shuffle(perm.begin(), perm.end());
  
  
}

VGramID::VGramID(const string &filenamePrefix) 
{
  loadData(filenamePrefix);
}

unsigned VGramID::getId(const string &gram) const
{
  unsigned
    id = 0, 
    gramLen = gram.length();
  string::size_type pos;
  for (unsigned i = 0; i < gramLen; i++) {
    pos = charset.find(gram[i]);
    if (pos == string::npos) {
      cerr << "can't find character '" << gram[i] << "'(" 
           << static_cast<unsigned>(gram[i]) << ") of gram \"" << gram
           << "\" in charset" << endl;
      exit(EXIT_FAILURE);
    }
    id += pow(charsetLen, gramLen - i - 1) * pos;
  }
  return id;
}

string VGramID::getGram(unsigned id) const
{
  string gram = "";
  while (id > 0) {
    gram = string(1, charset[id % charsetLen]) + gram;
    id = id / charsetLen;
  }
  while (gram.length() < q)
    gram = string(1, charset[0]) + gram;
  return gram;
}


//Threshold T (implemented differently)
// void VGramID::pruneGetIds(const string &s, vector<unsigned> &ids, 
// GramListMap &idLists, StringGramPos &posLists, GramListMap &freqLenLists){
//   vector<vector<unsigned> > gs;
//   vector<unsigned> grms;
//   for(unsigned z = qmin; z <= qmax; z++) {
//     vector<unsigned> temp;
//     str2grams(s, temp, z);
//     gs.push_back(temp);
//   }
//   grms.resize(gs.at(0).size());

//   for(unsigned w = 0; w < gs.at(0).size(); w++){

//   // priority for longest gram
//     for(unsigned z = qmax-qmin; z >= 0; z--) {

//     // if(freqLenLists.find(gs.at(z)[w]) == freqLenLists.end()
//     // ) {
       
//     // } else {
//       if(freqLenLists[gs.at(0)[w]]->at(1) <= rareqminFreq || z == 0) { //not sure how good rarefreq is, make it small just to be safe
//         grms.push_back(gs.at(0)[w]);
         
//         vector<vector<unsigned> > arrayPos; 
//         vector<unsigned> tempq;
//         tempq.push_back(gs.at(0)[w]); //gram
//         tempq.push_back(w); //pos
//         arrayPos.push_back(tempq);
//         posLists[s] = arrayPos;
//       }
//       else if(freqLenLists[gs.at(z)[w]]->at(1) / freqLenLists[gs.at(z-1)[w]]->at(1) >= pow((z+qmin-1)/(z+qmin),2) 
//       && freqLenLists[gs.at(z)[w]]->at(1) >= rareqminFreq
//       ) { // some random threshold bound
//         grms.push_back(gs.at(z)[w]);

//         vector<vector<unsigned> > arrayPos; 
//         vector<unsigned> tempq;
//         tempq.push_back(gs.at(z)[w]); //gram
//         tempq.push_back(w); //pos
//         arrayPos.push_back(tempq);
//         posLists[s] = arrayPos;

//         w = w + z;
//         break;
//       }
//     // }
//     }
//   }

//   for (vector<unsigned>::const_iterator it = grms.begin(); it != grms.end(); it++)
//     ids.push_back(*it);
// }




void VGramID::getIds(const string &s, vector<unsigned> &ids, vector<unsigned> &pids, 
GramListMap &idLists, GramListMap &posLists) 
{
  string padded;
  if(s.length()<qmin){
    padded = string(qmin - s.length(), PREFIXCHAR) + s; 
  } else {padded = s;}

  vector<vector<unsigned> > gs;
  vector<unsigned> grms;
  vector<unsigned> pos;
  unsigned qqmax = qmax > padded.length() ? padded.length() : qmax;
  // cerr << "  txx " << qmax << endl;
  for(unsigned z = qmin; z <= qqmax; z++) {
    vector<unsigned> temp;
    // if(z == qmin){
    //   str2grams(s, temp, z);
    // }
    // else {
    // if(s.length()<z){
    //     gs.push_back(temp); // empty?
    //   } else {
        str2gramsNoPrePost(padded,temp,z);
        gs.push_back(temp);
      // }
    
    // }

  } // gs: [[2 grams] [3 grams] [4 grams]] 
  // grms.resize(gs.at(0).size());

  unsigned currentcoverage = 0;
  for(unsigned w = 0; w < gs.at(0).size(); w++){ //qmin gram size   
  // priority for longest gram  

    if(idLists.find(gs.at(0)[w]) == idLists.end()){
      grms.push_back(gs.at(0)[w]); // query string somehow doesn't have some qmin gram.
      pos.push_back(w);
      currentcoverage = w + qmin;
      continue;
    }

    // cerr << "qqmax: " << qqmax << " ";
        
    for(unsigned z = qqmax-qmin; z >= 0; z--) {
      if(idLists[gs.at(0).at(w)]->size() <= rareqminFreq || z == 0) { //not sure how good rarefreq is, make it small just to be safe
        // cerr << "currentcoverage: " << currentcoverage << " w + qmin :"  << w + qmin << endl;
        // if(1){
        if(w + qmin > currentcoverage){
          grms.push_back(gs.at(0)[w]);
          pos.push_back(w);
          currentcoverage = w + qmin;
          
        }
        break;
      }

      // if(w - qmin + 1 < 0) { //real current location is negative
      //   continue;
      // }

      if(w > gs.at(z).size()-1) { //current location doesnt have gram as long as z
        // cerr << "toot1 "; 
        continue;
      }
      
      if(idLists.find(gs.at(z)[w]) == idLists.end()){
        // cerr << "toot2 ";
        continue;
      }


      // cerr << "test " << idLists[gs.at(z).at(w)]->size() << endl;
      if(idLists[gs.at(z).at(w)]->size() > 10// / idLists[gs.at(z-1).at(w)]->size() >= pow((z+qmin-1)/(z+qmin),4) 
      && idLists[gs.at(z).at(w)]->size() > rareqminFreq
      ) { // some random threshold bound

        if(w + qmin + z > currentcoverage){
          grms.push_back(gs.at(z)[w]);
          pos.push_back(w);
          currentcoverage = w + qmin + z;
          break; 
        }


      }
    }
    
  }

  for (vector<unsigned>::const_iterator it = grms.begin(); it != grms.end(); it++)
    ids.push_back(*it);
  
  if(&pids){
    for (vector<unsigned>::const_iterator it = pos.begin(); it != pos.end(); it++)
      pids.push_back(*it);  
  }
  
}

void VGramID::getGrams(const vector<unsigned> &ids, vector<string> &grams) const
{
  for (vector<unsigned>::const_iterator it = ids.begin(); it != ids.end(); it++)
    grams.push_back(getGram(*it));
}

void VGramID::saveData(const string &filenamePrefix) const
{
  string filename = filenamePrefix + gramidSuffix;

  cerr << "write to \"" << filename << "\"...";
  cerr.flush();

  ofstream file(filename.c_str(), ios::out | ios::binary);
  if (!file) {
    cerr << "can't open output file \"" << filename << "\"" << endl;
    exit(EXIT_FAILURE);
  }

  file.write(reinterpret_cast<const char*>(&q), sizeof(unsigned));
  if (file.fail()) writeerror(filename);

  file.write(reinterpret_cast<const char*>(&qmin), sizeof(unsigned));
  if (file.fail()) writeerror(filename);

  file.write(reinterpret_cast<const char*>(&qmax), sizeof(unsigned));
  if (file.fail()) writeerror(filename);

  file.write(reinterpret_cast<const char*>(&st), sizeof(char));
  if (file.fail()) writeerror(filename);

  file.write(reinterpret_cast<const char*>(&en), sizeof(char));
  if (file.fail()) writeerror(filename);

  file.write(reinterpret_cast<const char*>(&charsetLen), sizeof(unsigned));
  if (file.fail()) writeerror(filename);

  for (unsigned i = 0; i < charsetLen; i++) {
    file.write(reinterpret_cast<const char*>(&charset[i]), sizeof(char));
    if (file.fail()) writeerror(filename);
  }
  
  file.write(reinterpret_cast<const char*>(&n), sizeof(unsigned));
  if (file.fail()) writeerror(filename);

  for (vector<unsigned>::const_iterator it = perm.begin(); it != perm.end(); ++it)
    file.write(reinterpret_cast<const char*>(&*it), sizeof(unsigned));

  file.close();

  cerr << "OK" << endl;
}

void VGramID::loadData(const string &filenamePerfix)
{
  string filename = filenamePerfix + gramidSuffix;
 
  cerr << "read from \"" << filename << "\"...";
  cerr.flush();

  ifstream file(filename.c_str(), ios::in | ios::binary);
  if (!file) {
    cerr << "can't open input file \"" << filename << "\"" << endl;
    exit(EXIT_FAILURE);
  }

  file.read(reinterpret_cast<char*>(&q), sizeof(unsigned));
  if (file.fail()) writeerror(filename);

  file.read(reinterpret_cast<char*>(&qmin), sizeof(unsigned));
  if (file.fail()) writeerror(filename);

  file.read(reinterpret_cast<char*>(&qmax), sizeof(unsigned));
  if (file.fail()) writeerror(filename);

  file.read(reinterpret_cast<char*>(&st), sizeof(char));
  if (file.fail()) writeerror(filename);

  file.read(reinterpret_cast<char*>(&en), sizeof(char));
  if (file.fail()) writeerror(filename);

  file.read(reinterpret_cast<char*>(&charsetLen), sizeof(unsigned));
  if (file.fail()) writeerror(filename);

  charset = "";
  for (unsigned i = 0; i < charsetLen; i++) {
    char ch;
    file.read(reinterpret_cast<char*>(&ch), sizeof(char));
    if (file.fail()) writeerror(filename);
    charset += string(1, ch);
  }
  
  file.read(reinterpret_cast<char*>(&n), sizeof(unsigned));
  if (file.fail()) writeerror(filename);

  for (unsigned i = 0; i < n; i++) {
    unsigned e;
    file.read(reinterpret_cast<char*>(&e), sizeof(unsigned));
    perm.push_back(e);
  }

  file.close();

  cerr << "OK" << endl;
}

unsigned VGramID::invPerm(unsigned id) const
{
  for (unsigned i = 0; i < perm.size(); i++)
    if (perm[i] == id)
      return i;
  cerr << "ID " << id << " out of range in permutation" << endl;
  exit(EXIT_FAILURE);
}

bool VGramID::consistData(const string &filenamePrefix, 
                         const string &filenameExt) const
{
  string filename = filenamePrefix + gramidSuffix;

  struct stat attrib, attribExt;
  if (stat(filename.c_str(), &attrib)) {
    cerr << "can't stat file \"" << filename << "\"" << endl;
    exit(EXIT_FAILURE);
  }
  if (stat(filenameExt.c_str(), &attribExt)) {
    cerr << "can't stat file \"" << filenameExt << "\"" << endl;
    exit(EXIT_FAILURE);
  }

  if (!(attribExt.st_mtime <= attrib.st_mtime))
    return false;

  return true;  
}

bool VGramID::operator==(const VGramID& g) const 
{
  if (this == &g)
    return true;
  if (q == g.q && 
      qmin == g.qmin && 
      qmax == g.qmax &&  
      st == g.st && 
      en == g.en && 
      charset == g.charset && 
      charsetLen == g.charsetLen && 
      n == g.n && 
      perm == g.perm)
    return true;
  return false;
}

// str2words
void str2words(const string &s, vector<string> &res, const string &delims) 
{
  string::size_type begIdx, endIdx;
  
  begIdx = s.find_first_not_of(delims);
  
  while (begIdx != string::npos) {
    endIdx = s.find_first_of(delims, begIdx);
    if (endIdx == string::npos)
      endIdx = s.length();
//     cout << begIdx << " " << endIdx << " " 
//          << s.substr(begIdx, endIdx - begIdx) << endl;
    res.push_back(s.substr(begIdx, endIdx - begIdx));
    begIdx = s.find_first_not_of(delims, endIdx);
  }
}

// WordIndex
void WordIndex::build(const vector<string> &data, WordHash &wordHash)
{
  cout << "WordIndex::build..."; cout.flush();
  
  vector<string> words;
  for (unsigned i = 0; i < data.size(); i++) {

    words.clear();
    str2words(data[i], words);
    for (vector<string>::const_iterator word = words.begin();
         word != words.end(); ++word)
      wordHash[*word].insert(i);
  } 

  cout << "OK" << endl;
}

void WordIndex::build(const string &filenameDataset, WordHash &wordHash)
{
  cout << "WordIndex::build..."; cout.flush();

  ifstream fileDataset(filenameDataset.c_str());
  if (!fileDataset) {
    cerr << "can't open input file \"" << filenameDataset << "\"" << endl;
    exit(EXIT_FAILURE);
  }
  
  vector<string> words;
  unsigned i = 0;
  const unsigned maxLineLen = 256;
  char line[maxLineLen];

  while (true) {
    fileDataset.getline(line, maxLineLen);
    if (fileDataset.eof())
      break;
    if (fileDataset.fail()) {
      cerr << "open reading input file \""
           << filenameDataset << "\"" << endl
           << "line length might exceed " << maxLineLen << " characters" << endl;
      exit(EXIT_FAILURE);
    }

    words.clear();
    str2words(string(line), words);
    for (vector<string>::const_iterator word = words.begin();
         word != words.end(); ++word)
      wordHash[*word].insert(i);

    i++;
  } 

  cout << "OK" << endl;
}

void WordIndex::build(const vector<string> &data,
                      WordIds &wordIds, WordKey &wordKey)
{
  cout << "WordIndex::build..."; cout.flush();
  
  vector<string> words;
  for (unsigned i = 0; i < data.size(); i++) {

    words.clear();
    str2words(data[i], words);
    for (vector<string>::const_iterator word = words.begin();
         word != words.end(); ++word) {

      pair<WordKey::iterator, bool> wordIns = 
        wordKey.insert(make_pair(*word, 0));

      unsigned wordPos;
      if (wordIns.second) {
        // word not in WordLevel
        wordPos = wordIds.size();
        wordIns.first->second = wordPos;
        Ids ids;
        ids.insert(i);
        wordIds.push_back(make_pair(*word, ids));
      }
      else {
        // word in WordLevel
        wordPos = wordIns.first->second;
        wordIds[wordPos].second.insert(i);
      }            
    }
  } 

  cout << "OK" << endl;
}

void WordIndex::build(const string &filenameDataset,
                      WordIds &wordIds, WordKey &wordKey)
{
  cout << "WordIndex::build..."; cout.flush();

  ifstream fileDataset(filenameDataset.c_str());
  if (!fileDataset) {
    cerr << "can't open input file \"" << filenameDataset << "\"" << endl;
    exit(EXIT_FAILURE);
  }
  
  vector<string> words;
  unsigned i = 0;
  const unsigned maxLineLen = 256;
  char line[maxLineLen];

  while (true) {
    fileDataset.getline(line, maxLineLen);
    if (fileDataset.eof())
      break;
    if (fileDataset.fail()) {
      cerr << "open reading input file \""
           << filenameDataset << "\"" << endl
           << "line length might exceed " << maxLineLen << " characters" << endl;
      exit(EXIT_FAILURE);
    }

    words.clear();
    str2words(string(line), words);
    for (vector<string>::const_iterator word = words.begin();
         word != words.end(); ++word) {

      pair<WordKey::iterator, bool> wordIns = 
        wordKey.insert(make_pair(*word, 0));

      unsigned wordPos;
      if (wordIns.second) {
        // word not in WordLevel
        wordPos = wordIds.size();
        wordIns.first->second = wordPos;
        Ids ids;
        ids.insert(i);
        wordIds.push_back(make_pair(*word, ids));
      }
      else {
        // word in WordLevel
        wordPos = wordIns.first->second;
        wordIds[wordPos].second.insert(i);
      }            
    }

    i++;
  } 

  cout << "OK" << endl;
}

void WordIndex::build(const vector<string> &data,
                      vector<string> &wordVect, vector<Ids> &idsVect, 
                      WordKey &wordPosMap) 
{
  cout << "WordIndex::build..."; cout.flush();

  vector<string> words;
  unsigned pos;
  
  for (unsigned i = 0; i < data.size(); i++) {

    words.clear();
    str2words(data[i], words);
    for (vector<string>::const_iterator word = words.begin();
         word != words.end(); ++word) {

      pair<WordKey::iterator, bool> ins = 
        wordPosMap.insert(make_pair(*word, 0));

      if (ins.second) {
        // word not in WordHash
        pos = wordVect.size();
        wordVect.push_back(*word);
        Ids ids;
        ids.insert(i);
        idsVect.push_back(ids);
        ins.first->second = pos;
      }
      else {
        // word in WordHash
        pos = ins.first->second;
        idsVect[pos].insert(i);
      } 
    }
  }

  cout << "OK" << endl;  
}

void WordIndex::save(const string &filenameWords, 
                     const string &filenameIds,
                     const WordHash &wordHash) 
{
  cerr << "write to \"" << filenameWords << "\" and \""
       << filenameIds << "\"...";

  ofstream fileWords(filenameWords.c_str(), ios::out);  
  if (!fileWords) {
    cerr << "can't open output file \"" << filenameWords << "\"" << endl;
    exit(EXIT_FAILURE);
  }

  ofstream fileIds(filenameIds.c_str(), ios::out | ios::binary);  
  if (!fileIds) {
    cerr << "can't open output file \"" << filenameIds << "\"" << endl;
    exit(EXIT_FAILURE);
  }

  unsigned size;
  for(WordHash::const_iterator it = wordHash.begin();
      it != wordHash.end(); ++it) {
    fileWords << it->first << endl;
    
    size = it->second.size();
    fileIds.write(reinterpret_cast<const char*>(&size), sizeof(unsigned));
    for (Ids::const_iterator jt = it->second.begin();
         jt != it->second.end(); ++jt)
      fileIds.write(reinterpret_cast<const char*>(&*jt), sizeof(unsigned));
  }
    
  fileWords.close();
  fileIds.close();

  cerr << "OK" << endl;
}

void WordIndex::load(const string &filenameWords, 
                     const string &filenameIds, 
                     WordHash &wordHash)
{
  cerr << "read from \"" << filenameWords << "\" and \""
       << filenameIds << "\"...";

  ifstream fileWords(filenameWords.c_str(), ios::in);  
  if (!fileWords) {
    cerr << "can't open input file \"" << filenameWords << "\"" << endl;
    exit(EXIT_FAILURE);
  }

  ifstream fileIds(filenameIds.c_str(), ios::in | ios::binary);  
  if (!fileIds) {
    cerr << "can't open input file \"" << filenameIds << "\"" << endl;
    exit(EXIT_FAILURE);
  }

  unsigned size, id;
  string word;
  Ids ids;

  while (true) {
    // hash
    fileWords >> word;
    if (fileWords.eof())
      break;

    ids.clear();
    fileIds.read(reinterpret_cast<char*>(&size), sizeof(unsigned));
    for (unsigned i = 0; i < size; i++) {
      fileIds.read(reinterpret_cast<char*>(&id), sizeof(unsigned));
      ids.insert(id);
    }
    
    wordHash[word] = ids;
  }
    
  fileWords.close();
  fileIds.close();

  cerr << "OK" << endl;
}

void WordIndex::save(const string &filenameWids, 
                     const string &filenameWkey,
                     const WordIds &wordIds, const WordKey &wordKey)
{
  cerr << "write to \"" << filenameWids << "\" and \""
       << filenameWkey << "\"...";

  ofstream fileWids(filenameWids.c_str(), ios::out | ios::binary);  
  if (!fileWids) {
    cerr << "can't open output file \"" << filenameWids << "\"" << endl;
    exit(EXIT_FAILURE);
  }

  unsigned size;
  for(WordIds::const_iterator it = wordIds.begin();
      it != wordIds.end(); ++it) {
    size = it->second.size();
    fileWids.write(reinterpret_cast<const char*>(&size), sizeof(unsigned));
    for (Ids::const_iterator jt = it->second.begin();
         jt != it->second.end(); ++jt)
      fileWids.write(reinterpret_cast<const char*>(&*jt), sizeof(unsigned));
  }
    
  fileWids.close();

  ofstream fileWkey(filenameWkey.c_str(), ios::out);  
  if (!fileWkey) {
    cerr << "can't open output file \"" << filenameWkey << "\"" << endl;
    exit(EXIT_FAILURE);
  }

  for (WordIds::const_iterator it = wordIds.begin();
       it != wordIds.end(); ++it)
    fileWkey << it->first<< " " << wordKey.find(it->first)->second << endl;

  fileWkey.close();

  cerr << "OK" << endl;
}

void WordIndex::load(const string &filenameWids, 
                     const string &filenameWkey, 
                     WordIds &wordIds, WordKey &wordKey)
{
  cerr << "read from \"" << filenameWids << "\" and \""
       << filenameWkey << "\"...";

  ifstream fileWids(filenameWids.c_str(), ios::in | ios::binary);  
  if (!fileWids) {
    cerr << "can't open input file \"" << filenameWids << "\"" << endl;
    exit(EXIT_FAILURE);
  }

  ifstream fileWkey(filenameWkey.c_str(), ios::in);  
  if (!fileWkey) {
    cerr << "can't open input file \"" << filenameWkey << "\"" << endl;
    exit(EXIT_FAILURE);
  }

  string word;
  unsigned size, id;
  Ids ids;

  while (true) {
    fileWids.read(reinterpret_cast<char*>(&size), sizeof(unsigned));
    if (fileWids.eof())
      break;
    ids.clear();
    for (unsigned i = 0; i < size; i++) {
      fileWids.read(reinterpret_cast<char*>(&id), sizeof(unsigned));
      ids.insert(id);
    }

    fileWkey >> word >> id;
    if (fileWkey.eof()) {
      cerr << "inconsistency in input file \"" << filenameWkey << "\"" << endl;
      exit(EXIT_FAILURE);
    }
    
    wordIds.push_back(make_pair(word, ids));
    wordKey[word] = id;
  }

  fileWids.close();  
  fileWkey.close();

  cerr << "OK" << endl;
}

bool WordIndex::exist(const string &filename1, const string &filename2) 
{
  ifstream file1(filename1.c_str(), ios::in | ios::binary);  
  if (!file1) 
    return false;
  file1.close();

  ifstream file2(filename2.c_str(), ios::in);  
  if (!file2)
    return false;
  file2.close();
  
  return true;
}

const string
filenameExtWordVect = ".wi.wv.txt", 
                  filenameExtIdsVect = ".wi.idv.bin";


void WordIndex::save(const string &filename, const vector<string> &wordVect,
                     const vector<Ids> &idsVect, const WordKey &wordPosMap) 
{
  const string
    filenameWordVect = filename + filenameExtWordVect, 
    filenameIdsVect = filename + filenameExtIdsVect;
  
  cerr << "write to \"" << filenameWordVect << "\" and" << endl
       << "         \"" << filenameIdsVect  << "\"...";
  cerr.flush();

  ofstream fileWordVect(filenameWordVect.c_str(), ios::out);  
  if (!fileWordVect) {
    cerr << "can't open output file \"" << filenameWordVect << "\"" << endl;
    exit(EXIT_FAILURE);
  }

  for (vector<string>::const_iterator word = wordVect.begin();
       word != wordVect.end(); ++word)
    fileWordVect << *word << endl;

  fileWordVect.close();


  ofstream fileIdsVect(filenameIdsVect.c_str(), ios::out | ios::binary);  
  if (!fileIdsVect) {
    cerr << "can't open output file \"" << filenameIdsVect << "\"" << endl;
    exit(EXIT_FAILURE);
  }

  for (vector<Ids>::const_iterator ids = idsVect.begin();
       ids != idsVect.end(); ++ids) {
    unsigned size = ids->size();
    fileIdsVect.write(reinterpret_cast<const char*>(&size), sizeof(unsigned));
    for (Ids::const_iterator id = ids->begin();
         id != ids->end(); ++id)
      fileIdsVect.write(reinterpret_cast<const char*>(&*id), sizeof(unsigned));
  }
  
  fileIdsVect.close();

  cerr << "OK" << endl;
}

void WordIndex::load(const string &filename, vector<string> &wordVect,
                     vector<Ids> &idsVect, WordKey &wordPosMap) 
{
  const string
    filenameWordVect = filename + filenameExtWordVect, 
    filenameIdsVect = filename + filenameExtIdsVect;
  
  cerr << "read from \"" << filenameWordVect << "\" and" << endl
       << "          \"" << filenameIdsVect  << "\"...";
  cerr.flush();

  readString(wordVect, filenameWordVect);

  ifstream fileIdsVect(filenameIdsVect.c_str(), ios::in | ios::binary);  
  if (!fileIdsVect) {
    cerr << "can't open input file \"" << filenameIdsVect << "\"" << endl;
    exit(EXIT_FAILURE);
  }
  
  for (unsigned i = 0; i < wordVect.size(); i++) {    
    unsigned size;
    fileIdsVect.read(reinterpret_cast<char*>(&size), sizeof(unsigned));

    Ids ids;
    unsigned el;
    for (unsigned j = 0; j < size; j++) {
      fileIdsVect.read(reinterpret_cast<char*>(&el), sizeof(unsigned));
      ids.insert(el);
    }
    idsVect.push_back(ids);

    wordPosMap[wordVect[i]] = i;
  }
  
  fileIdsVect.close();

  cerr << "OK" << endl;
}
