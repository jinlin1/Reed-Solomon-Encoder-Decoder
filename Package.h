/**
 * Package writes a galois field polynomial 
 * into a file
**/
#ifndef INCLUDE_PACKAGE_H
#define INCLUDE_PACKAGE_H

#include <vector>
#include "GaloisFieldArithmetic/GaloisFieldPolynomial.h"
using namespace std;

class Package {
  public:
    Package(
        galois::GaloisFieldPolynomial outputPoly,
        unsigned int length);
    string getOutPolyStr();

  private:
    string outPolyStr;

};

#endif
