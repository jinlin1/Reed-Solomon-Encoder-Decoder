#ifndef INCLUDE_PACKAGE_H
#define INCLUDE_PACKAGE_H

#include <vector>
#include <string>
#include "GaloisFieldArithmetic/GaloisFieldPolynomial.h"
using namespace std;

class Package {
  public:
    Package(
        galois::GaloisFieldPolynomial outputPoly,
        unsigned int length,
        string filename);

};

#endif
