#include "Parse.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

Parse::Parse(
    string gfe, string primpoly, string genpoly, string msg)
{
  //galois field size
  this->gfe = gfe.at(0) - '0';

  //primitive polynomial
  for (int i=0; i<primpoly.length(); i++) {
      this->primpoly.push_back(primpoly.at(i) - '0');
  }

  //generator polynomial
  this->genpoly = parseDelim(genpoly);

  //message
  this->msg = parseDelim(msg);
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
