#include "Package.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
Package::Package( 
    galois::GaloisFieldPolynomial outputPoly,
    unsigned int length) {

  std::stringstream outputStr;

  int i;
  for (i=0; i<=outputPoly.deg(); i++) {
    outputStr << outputPoly[i];
    if (i < length-1) outputStr << ",";
  }
  if (i < length) {
    for (; i < length; i++) {
      outputStr << "0";
      if (i < length-1) outputStr << ",";
    }
  }

  outPolyStr = outputStr.str();

}

string Package::getOutPolyStr() {
  return outPolyStr;
}
