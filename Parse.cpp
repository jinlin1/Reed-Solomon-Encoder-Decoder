#include "Parse.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

Parse::Parse(string filename)
{
  ifstream infile(filename);
  string temp;

  //galois field size
  if (getline(infile, temp)) gfe = temp.at(0) - '0';

  //primitive polynomial
  if (getline(infile, temp)) {
    for (int i=0; i<temp.length(); i++) {
      primpoly.push_back(temp.at(i) - '0');
    }
  }

  //generator polynomial
  getline(infile,temp);
  genpoly = parseDelim(temp);

  //message
  getline(infile,temp);
  msg = parseDelim(temp);

  infile.close();

}

unsigned int Parse::getGfe() {
  return gfe;
}

vector<unsigned int> Parse::getPrimpoly() {
  return primpoly;
}

vector<unsigned int> Parse::getGenpoly() {
  return genpoly;
}

vector<unsigned int> Parse::getMsg() {
  return msg;
}

vector<unsigned int> Parse::parseDelim(string instr) {
  vector<unsigned int> ret;
  stringstream ss(instr);
  string temp;
  while (ss.good()) {
    getline(ss,temp,',');
    ret.push_back(stoi(temp));
  }
  return ret;
}
