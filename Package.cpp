#include "Package.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
Package::Package( 
    galois::GaloisFieldPolynomial outputPoly,
    unsigned int length,
    string filename) {
  int i;
  ofstream outfile(filename);
  for (i=0; i<=outputPoly.deg(); i++) {
    outfile << outputPoly[i];
    if (i < length-1) outfile << ",";
  }
  if (i < length) {
    for (; i < length; i++) {
      outfile << "0";
      if (i < length-1) outfile << ",";
    }
  }
  outfile.close();

}
